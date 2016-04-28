#include <stdio.h>
#include <stdlib.h>

#include "bencode.h"
#include "torrent.h"

int main(int argc, char** argv)
{
    FILE *torrentfile = NULL;
    t_conf *metainfo = NULL;
    b_dict *tracker_response = NULL;

    if (argc == 2)
    {
        torrentfile = fopen(argv[1],"r");
        if (!torrentfile)
        {
            printf("Could not open '%s'\n", argv[1]);
        }
    }
    else
    {
        printf("Usage: %s torrent_file\n", argv[0]);
    }

    // Initialize
    metainfo = parse_torrent_file(torrentfile);

    printf("name:\n%s\n",metainfo->name);
    printf("tracker:\n%s\n",metainfo->announce);
    printf("piece_len:\n%lld\n",metainfo->piece_len);
    printf("pieces:\n%s\n",metainfo->pieces);


    // Get peers from tracker
    tracker_response = tracker_request(metainfo, torrentfile);
    fclose(torrentfile);

    // Connect to peers, start downloading
        // verify each block

    // Finish, clean up
}
