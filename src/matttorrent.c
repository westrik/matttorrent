#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "bencode.h"
#include "torrent.h"
#include "peer.h"
#include "chunk.h"

/**
 * Open torrent config file,
 * parse it 
 */
void initialize(int argc, char** argv, FILE *torrentfile, t_conf *metainfo)
{
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
}

/**
 * Contact the torrent tracker specified in the config file,
 * parse and save response in a dictionary.
 */
b_dict *contact_tracker(t_conf *metainfo, FILE *torrentfile)
{
    b_dict* tracker_response;

    // Get peers from tracker
    printf("Contacting tracker... "); fflush(stdout);
    tracker_response = tracker_request(metainfo, torrentfile);

    printf("OK\nDownloading %s with %d peers\n\n",
            metainfo->name,
            count_linked_list(dict_find(tracker_response, "peers"))
          );

    char *x = dict_find(tracker_response, "peers")->element.c;
    for (int i=0; i<strlen(x); i++)
        printf("%.2x ", x[i]);
    
    return tracker_response;
}

/**
 * Save all downloaded chunks to the output file
 */
void save(t_conf *metainfo, chunks *chunks, FILE *output)
{
    // Save downloaded chunks to designated file
    output = fopen(metainfo->name,"w+");
    if (output == NULL || !save_chunks(chunks, output))
    {
        printf("Failed to save download to %s\n", metainfo->name);
        exit(1);
    }
}

/**
 * Close config and output files
 * Free allocated memory
 * - config struct
 * - tracker response dictionary 
 * - all downloaded chunks
 */
void cleanup
    (
    FILE *torrentfile, 
    FILE *output,
    t_conf *metainfo, 
    b_dict *tracker_response,
    chunks *chunks
    )
{

    fclose(torrentfile);
    fclose(output);
    // ...
}


/* ============================================================ */

int main(int argc, char** argv)
{
    FILE *torrentfile = NULL,
         *output = NULL;
    t_conf *metainfo = NULL;
    b_dict *tracker_response = NULL;
    chunks *chunks = NULL;

    srand(time(NULL));

    initialize(argc, argv, torrentfile, metainfo);
    tracker_response = contact_tracker(metainfo, torrentfile);
    chunks = download(tracker_response);
    save(metainfo, chunks, output);

    cleanup
    (
        torrentfile, output,
        metainfo, tracker_response,
        chunks
    );
}
