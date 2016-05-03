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
    int i;

    for(i= 0; i<100; i++)
    {
        printf("\rDownloading %d%%", i);
        fflush(stdout);


        usleep(250000);
    }
    printf("\n");

    return 0;
}

/*peer_resp *handshake(char *peer_loc)
{

}
*/
