#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
 * http://tracker.archlinux.org:6969/announce?info_hash=%24%60%d6%c8%af%f3%fe%e6s0%ab7%25%c4%a1%e5%d8J%7d%c8&peer_id=-TR2840-n93spnhfafc0&port=65474&uploaded=0&downloaded=0&left=689963008&numwant=80&key=15c3e853&compact=1&supportcrypto=1&event=started
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





}
