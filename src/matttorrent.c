/* ======================================\
 | matttorrent                           |
 | Simple event-driven BitTorrent client |
 | Matthew Westrik                       |
 \ =====================================*/

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
void initialize
    (
    int argc, 
    char** argv, 
    FILE **torrentfile, 
    t_conf **metainfo
    )
{
    // Open torrent file
    if (argc == 2)
    {
        *torrentfile = fopen(argv[1],"r");
        if (!(*torrentfile))
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
    *metainfo = parse_torrent_file(*torrentfile);
}

/**
 * Contact the torrent tracker specified in the config file,
 * parse and save response in a dictionary,
 * print file name and number of peers returned by tracker.
 */
b_dict *contact_tracker(t_conf *metainfo, FILE *torrentfile)
{
    b_dict* tracker_response;
    b_dict_element* peers_d;
    char* peers_c;
    int peer_count;

    // Get peers from tracker
    printf("Contacting tracker... "); fflush(stdout);
    tracker_response = tracker_request(metainfo, torrentfile);

    // Count number of peers
    peers_d = dict_find(tracker_response, "peers");
    if (peers_d->type == STRING)
    {
        peers_c = peers_d->element.c;
        peer_count = strlen(peers_c) / 6;
    }
    else
    {
        peer_count = count_linked_list(peers_d);
    }

    // Print info about torrent (name, num peers)
    printf("OK\nDownloading %s with %d peers\n\n",
            metainfo->name,
            peer_count
          );
    
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

    free(metainfo->announce);
    free(metainfo->name);
    free(metainfo->pieces);

    dict_destroy(tracker_response);

    free_chunks(chunks);
}


/* =================================== 
                 RUN 
   =================================== */

int main(int argc, char** argv)
{
    FILE   *torrentfile = NULL,
           *output = NULL;
    t_conf *metainfo = NULL;
    b_dict *tracker_response = NULL;
    chunks *chunks = NULL;

    srand(time(NULL));

    initialize(argc, argv, &torrentfile, &metainfo);
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
