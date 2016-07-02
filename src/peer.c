#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "peer.h"

/**
 * Take list of peers from tracker,
 * start communicating with them and downloading chunks.
 * @return chunks struct (all file chunks)
 */
chunks *download(b_dict *tracker_response)
{
    b_dict_element *peers = dict_find(tracker_response, "peers");

    if (peers->type == DICT)
    {
        printf("DICT peers\n");
    }
    else if (peers->type == STRING)
    {
        printf("\n\nSTRING peers\n");

        printf("%c\n",peers->element.c[0]);
        printf("%c\n",peers->element.c[1]);
        printf("%c\n",peers->element.c[2]);
        printf("%c\n",peers->element.c[3]);
        printf("%c\n",peers->element.c[4]);
        printf("%c\n",peers->element.c[5]);
    }


    // using list of peers, handshake with them all
    // then do some message passing to get chunks

    // set timer to re-request peer list (tracker response dictates time
    // interval to do this in)

    return 0;

    for(int i= 0; i<100; i++)
    {
        printf("\rDownloading %d%%", i);
        fflush(stdout);


        usleep(250000);
    }
    printf("\n");

}

/*peer_resp *handshake(char *peer_loc)
{

}
*/
