#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <uv.h>

#include "peer.h"

// start libuv eg code
#define DEFAULT_BACKLOG 128

uv_loop_t *loop;
struct sockaddr_in addr;

void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
    buf->base = (char*) malloc(suggested_size);
    buf->len = suggested_size;
}

void echo_write(uv_write_t *req, int status) {
    if (status) {
        fprintf(stderr, "Write error %s\n", uv_strerror(status));
    }
    free(req);
}

void echo_read(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf) {
    if (nread < 0) {
        if (nread != UV_EOF)
            fprintf(stderr, "Read error %s\n", uv_err_name(nread));
        uv_close((uv_handle_t*) client, NULL);
    } else if (nread > 0) {
        uv_write_t *req = (uv_write_t *) malloc(sizeof(uv_write_t));
        uv_buf_t wrbuf = uv_buf_init(buf->base, nread);
        uv_write(req, client, &wrbuf, 1, echo_write);
    }

    if (buf->base)
        free(buf->base);
}

void on_new_connection(uv_stream_t *server, int status) {
    if (status < 0) {
        fprintf(stderr, "New connection error %s\n", uv_strerror(status));
        // error!
        return;
    }

    uv_tcp_t *client = (uv_tcp_t*) malloc(sizeof(uv_tcp_t));
    uv_tcp_init(loop, client);
    if (uv_accept(server, (uv_stream_t*) client) == 0) {
        uv_read_start((uv_stream_t*) client, alloc_buffer, echo_read);
    }
    else {
        uv_close((uv_handle_t*) client, NULL);
    }
}

// end libuv eg code

/**
 * Take list of peers from tracker,
 * start communicating with them and downloading chunks.
 * @return chunks struct (all file chunks)
 */
chunks *download(b_dict *tracker_response)
{
    peer peers[MAX_PEER];

    parse_peers(tracker_response, peers);

    for (int i = 0; i<MAX_PEER; i++)
    {
        if (peers[i].ipv4[0] != 0)
        {
        printf("%d.%d.%d.%d:%d\n",
            peers[i].ipv4[0],
            peers[i].ipv4[1],
            peers[i].ipv4[2],
            peers[i].ipv4[3],
            peers[i].port);

            //handshake with peer
            //do some message passing
            //etc
        }


    }

    // using list of peers, handshake with them all
    // then do some message passing to get chunks

    loop = uv_default_loop();

    uv_tcp_t server;
    uv_tcp_init(loop, &server);

    uv_ip4_addr("0.0.0.0", DEFAULT_PORT, &addr);

    uv_tcp_bind(&server, (const struct sockaddr*)&addr, 0);
    int r = uv_listen((uv_stream_t*) &server, DEFAULT_BACKLOG, on_new_connection);
    if (r) {
        fprintf(stderr, "Listen error %s\n", uv_strerror(r));
		exit(1);
    }
    uv_run(loop, UV_RUN_DEFAULT);


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
