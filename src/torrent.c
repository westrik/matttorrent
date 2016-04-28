#include <stdio.h>
#include <stdlib.h>

#include "torrent.h"


/* Notes about tracker params
 *
 * Example tracker request
 * http://tracker.archlinux.org:6969/announce?info_hash=%24%60%d6%c8%af%f3%fe%e6s0%ab7%25%c4%a1%e5%d8J%7d%c8&peer_id=-TR2840-n93spnhfafc0&port=65474&uploaded=0&downloaded=0&left=689963008&numwant=80&key=15c3e853&compact=1&supportcrypto=1&event=started
 *
 * info_hash is SHA1 of info dictionary from torrent file, parse that out
 * peer_id: a 20-byte value (doesn't matter)
 * port: port number this client will be listening on 
 * uploaded: total amount uploaded (in base ten ASCII)
 * downloaded: total number of bytes downloaded since 'started' sent
 * left: number of bytes left to download (size of file from info)
 * compact: will this client accept a compact response (yes -> 1)
 * no_peer_id: don't bother sending peer_id for each peer in response (yes)
 * event: 'started' / 'stopped' / 'completed'
 * numwant: number of peers we want (transmission does 80, so we'll do that)
 * supportcrypto: support crypto? (not for time being -> 0)
 */


/*
 * Decode bencoded torrent file
 * @param FILE* torrent: file pointer of bencoded torrent file
 */
b_dict* parse_torrent_file(FILE* torrent_f)
{
    b_dict* torrent_config;
    char* file_buffer = 0;
    int length = 0;

    if (torrent_f)
    {
        fseek(torrent_f, 0, SEEK_END);

        length = ftell(torrent_f);
        file_buffer = malloc(length*sizeof(char));

        // don't use torrent files >4GB lol
        fseek(torrent_f, 0, SEEK_SET);

        if (file_buffer)
        {
            fread(file_buffer, 1, length, torrent_f);
        }
    }
    else
    {
        printf("invalid file pointer\n");
    }

    // remember to fclose torrent_f
    
    torrent_config = parse_bencode_dict(file_buffer);

    b_dict_element* x = dict_find(torrent_config, "announce");

    if (x)
        printf("%s\n",x->element.c);
    else
        printf("no announce url found\n");


    b_dict_element* y = dict_find(torrent_config, "info");
    b_dict* d = y->element.d;

    if (!y)
        printf("no info dict found\n");


    b_dict_element* z = dict_find(d, "piece length");

    if (z)
        printf("%lld\n",z->element.i);
    else
        printf("eek\n");

    b_dict_element* p = dict_find(d, "pieces");

    if (p)
        printf("%s\n",p->element.c);


    b_dict_element* n = dict_find(d, "name");
    if (n)
        printf("%s\n",n->element.c);
}



/**
 * Connect to tracker, get peers, parse response
 *
 * @param b_dict* metainfo: torrent metainfo dictionary
 * @param FILE* torrent: original file pointer (needed to calculate SHA1 hash of info dict)
 */
b_dict* tracker_request(b_dict* metainfo, FILE* torrent_f)
{

}
