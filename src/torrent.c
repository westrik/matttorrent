#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <openssl/sha.h>

#include "torrent.h"

/**
 * Read file to string
 * Allocates char array size of file
 * Assumes file is closed externally
 */
char* dump_file_to_string(FILE* file)
{
    char* file_buffer = 0;
    int length = 0;

    if (file)
    {
        fseek(file, 0, SEEK_END);

        length = ftell(file);
        file_buffer = malloc(length*sizeof(char));

        // don't use torrent files >4GB lol
        fseek(file, 0, SEEK_SET);

        if (file_buffer)
        {
            fread(file_buffer, 1, length, file);
        }
        // nb: fclose(file) happens in main()
    }
    else
    {
        printf("invalid file pointer\n");
    }

    return file_buffer;
}


/*
 * Decode bencoded torrent file
 * @param FILE* torrent: file pointer of bencoded torrent file
 */
t_conf* parse_torrent_file(FILE* torrent_f)
{
    b_dict* torrent_config;
    t_conf* config = calloc(1,sizeof(t_conf));

    char* file_buffer = dump_file_to_string(torrent_f);

    // Parse torrent file
    torrent_config = parse_bencode_dict(file_buffer);

    // Extract relevant info
    char* announce = dict_find(torrent_config, "announce")->element.c;

    b_dict* info = dict_find(torrent_config, "info")->element.d;
    int64_t piece_len = dict_find(info, "piece length")->element.i;

    char* pieces = dict_find(info, "pieces")->element.c;

    char* name = dict_find(info, "name")->element.c;

    // Copy into handy new data structure
    config->announce = calloc(strlen(announce)+1,sizeof(char));
    config->name = calloc(strlen(name)+1,sizeof(char));
    config->pieces = calloc(strlen(pieces)+1,sizeof(char));
    config->piece_len = piece_len;

    strcpy(config->announce, announce);
    strcpy(config->name, name);
    strcpy(config->pieces, pieces);

    return config;
}



/* Notes about tracker params
 *
 * Example tracker request
 *http://thomasballinger.com:6969/announce?info_hash=%ef%5c%ce%17v%b19%14.F%b5%1dE%e7%edN%84%bc%dam&peer_id=-TR2920-bb4niiyk3cuw&port=65474&uploaded=0&downloaded=0&left=1751391&numwant=80&key=2aee37c0&compact=1&supportcrypto=1&event=started
 *
 * info_hash is SHA1 of info dictionary from torrent file, parse that out
 * peer_id: PEER_ID+12 random numbers
 * port: port number this client will be listening on (BT standard: between 6881-6889)
 * uploaded: total amount uploaded (in base ten ASCII)
 * downloaded: total number of bytes downloaded since 'started' sent
 * left: number of bytes left to download (size of file from info)
 * compact: will this client accept a compact response (yes -> 1)
 * no_peer_id: don't bother sending peer_id for each peer in response (yes)
 * event: 'started' / 'stopped' / 'completed'
 * numwant: number of peers we want (transmission does 80, so we'll do that)
 * supportcrypto: support crypto? (not for time being -> 0)
 */

/**
 * Connect to tracker, get peers, parse response
 *
 * @param b_dict* metainfo: torrent metainfo dictionary
 * @param FILE* torrent: original file pointer (needed to calculate SHA1 hash of info dict)
 */
b_dict* tracker_request(t_conf* metainfo, FILE* torrent_f)
{
    CURL *curl = curl_easy_init();
    CURLcode res;

    char url[BUFFER];

    char *_info_hash = curl_easy_escape(curl, info_hash(torrent_f), SHA_DIGEST_LENGTH);

    b_dict *response = NULL;

    response_chunk chunk;
    chunk.memory = malloc(1); // automatically grown
    chunk.size = 0;

    // Generate request URL
    snprintf(
        url, BUFFER,
            "http%s?info_hash=%s&peer_id=%s&port=%d&left=%lu"
            "&uploaded=0&downloaded=0supportcrypto=0"
            "&compact=1&event=started",
        strstr(metainfo->announce,"://"), // split on :// (so udp:// works)
        _info_hash,                       // SHA1 hash of info dictionary
        generate_peer_id(),               // partially random peer id
        DEFAULT_PORT,                     // port we're listening on
        strlen(metainfo->pieces)          // number of pieces we need
    );

    printf("%s",url);
    free(_info_hash);

    // Make request
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, &url);

        // send all data to this function 
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_mem_cb);

        // we pass our 'chunk' struct to the callback function 
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        curl_easy_setopt(curl, CURLOPT_USERAGENT, "matttorrent/1.0");
        
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);

        // Shoot off a GET req
        res = curl_easy_perform(curl);

        // Check for errors
        if(res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
            exit(1);
        }

        curl_easy_cleanup(curl);
    }

    // Parse bencoded response
    response = parse_bencode_dict((char*)chunk.memory);

    if (dict_find(response, "failure reason"))
    {
        printf("\nFailed, tracker says \"%s\"\n",dict_find(response, "failure reason")->element.c);
        exit(1);
    }

    return response;
}

/**
 * Find bencoded info dict in torrent file
 * then get SHA1 hash of this
 *
 * for data/test.torrent this should be info hash:
 * %ef%5c%ce%17v%b19%14.F%b5%1dE%e7%edN%84%bc%dam
 */
char* info_hash(FILE* torrent_f)
{
    char *file_buffer = dump_file_to_string(torrent_f),
         *info_dict_key = strstr(file_buffer, "4:info"),
         *info_dict = NULL;

    unsigned char _hash[SHA_DIGEST_LENGTH];
    char* hash = NULL;

    int position = 6; // Offset of dictionary from start of key

    int start, end;
    size_t len;

    // Find start and end of info dict in file
    if (info_dict_key != NULL)
    {
        start = position;

        __parse_dict(info_dict_key, &position);

        end = position;
    }
    else
    {
        return NULL;
    }

    // calculate SHA1 hash of info dict
    len = end-start;
    info_dict = malloc(sizeof(char)*len);
    memcpy((void*)info_dict, info_dict_key+6, len);

    SHA1((void*)info_dict, len, _hash);

    hash = malloc(sizeof(char)*SHA_DIGEST_LENGTH);
    memcpy(hash, _hash, SHA_DIGEST_LENGTH);

    free(file_buffer);
    free(info_dict);

    return hash;
}

/**
 * Generate partially random peer id
 * @returns char* 20 char peer id
 */
char* generate_peer_id()
{
    char* peer_id = calloc(21, sizeof(char));

    // copy default part
    strcat(peer_id, PEER_ID);

    // generate random part
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJK.0123456789";
    for (size_t n = strlen(peer_id); n < 20; n++) {
        int key = rand() % (int) (sizeof charset - 1);
        peer_id[n] = charset[key];
    }
    peer_id[20] = '\0';

    return peer_id;

}

/**
 * Helper function to write cURL responses to memory instead of stdout
 */
static size_t write_mem_cb (void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    response_chunk *mem = (response_chunk *)userp;

    mem->memory = realloc(mem->memory, mem->size + realsize + 1);
    if(mem->memory == NULL) {
        printf("Not enough memory (realloc returned NULL)\n");
        return 0;
    }

    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}
