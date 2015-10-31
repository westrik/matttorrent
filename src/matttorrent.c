#include <stdio.h>
#include <stdlib.h>

#include "bencode.h"
#include "torrent.h"

int main(int argc, char** argv)
{
    FILE *torrentfile = NULL;
    b_dict *metainfo = NULL,
           *tracker_response = NULL;

    if (argc == 2)
    {
        torrentfile = fopen(argv[1],"r");
        if (!torrentfile)
        {
            printf("Could not open '%s'", argv[1]);
        }
    }
    else
    {
        printf("Usage: %s torrent_file", argv[0]);
    }

    // Initialize
    metainfo = parse_torrent_file(torrentfile);


    // Get peers from tracker
    tracker_response = tracker_request(metainfo, torrentfile);
    fclose(torrentfile);

    // Connect to peers, start downloading
        // verify each block

    // Finish, clean up
}
