#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "bencode.h"
#include "torrent.h"
#include "peer.h"
#include "chunk.h"

int main(int argc, char** argv)
{
    FILE *torrentfile = NULL,
         *output = NULL;
    t_conf *metainfo = NULL;
    b_dict *tracker_response = NULL;
    chunks *chunks = NULL;

    srand(time(NULL));

    // Open torrent file
    if (argc == 2)
    {
        torrentfile = fopen(argv[1],"r");
        if (!torrentfile)
        {
            printf("Could not open '%s'\n", argv[1]);
            exit(1);
        }
    }
    else
    {
        printf("Usage: %s torrent_file\n", argv[0]);
        exit(1);
    }

    // Initialize
    metainfo = parse_torrent_file(torrentfile);

    // Get peers from tracker
    printf("Contacting tracker... ");
    tracker_response = tracker_request(metainfo, torrentfile);
    printf("OK\nDownloading %s...\n\n", metainfo->name);

    // Connect to peers, start downloading
    chunks = download(tracker_response);     

    // Save downloaded chunks to designated file
    output = fopen(metainfo->name,"w+");
    if (output == NULL || !save_chunks(chunks, output))
    {
        printf("Failed to save download to %s\n", metainfo->name);
        exit(1);
    }

    // Close files, free memory
    fclose(torrentfile);
    fclose(output);
    // ...
}
