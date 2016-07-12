#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#include "peer.h"

/**
 * Take list of peers from tracker,
 * start communicating with them and downloading chunks.
 * @return chunks struct (all file chunks)
 */
chunks *download(b_dict *tracker_response)
{
    peer peers[MAX_PEER];

    parse_peers(tracker_response, peers);

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

void parse_peers(b_dict *tracker_response, peer peers[MAX_PEER])
{
    b_dict_element *peers_d = dict_find(tracker_response, "peers");

    memset(peers, 0, MAX_PEER*sizeof(peer));

    // Parse peers, store in array 
    if (peers_d->type == LIST)
    {
        /*
          List of dicts, with each containing:
          peer id: peer's self-selected ID, 
              as described above for the
              tracker request (string)
          ip: peer's IP address either 
              IPv6 (hexed) or IPv4 (dotted quad) or
              DNS name (string)
          port: peer's port number (integer)
        */
        printf("Peers stored in list is unimplemented (todo)\n");
        exit(0);
    }
    else if (peers_d->type == STRING)
    {
        /*
          String consisting of multiples of 6 bytes. 
          First 4 bytes are the IP
          address and last 2 bytes are the port number. 
          All in network (big endian) notation.
        */

        char* peers_c = peers_d->element.c;

        int offset = 0, // 0-5 offset
            i = 0;      // index in peer array

        for (int j=0; j<strlen(peers_c);j++) 
        {
            unsigned int val = (unsigned char)peers_c[j];

            if (offset < 4)
            {
                peers[i].ipv4[offset] = val;
            }
            else if (offset == 4)
            {
                peers[i].port = 
                    ((unsigned char)peers_c[j] << 8)
                   + (unsigned char)peers_c[j+1];
            }

            offset++;
            if (offset == 6) 
            {
                offset = 0; 
                i++; if (i == MAX_PEER) break;
            }
        }

    }



}

/*peer_resp *handshake(char *peer_loc)
{

}
*/
