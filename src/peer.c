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

    for(i= 0; i<20; i++)
    {
        printf("\rIn progress %d", i);
        fflush(stdout);


        sleep(0.1);
    }

    return 0;
}

/*peer_resp *handshake(char *peer_loc)
{

}
*/
