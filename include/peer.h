#pragma once

#include "torrent.h"
#include "chunk.h"

typedef struct _peer
{
    int ipv4[4];
    int port;
} peer;

typedef struct 
{
    void* buffer;
} peer_resp;

/**
 * Take list of peers from tracker,
 * start communicating with them and downloading chunks.
 * @return chunks struct (all file chunks)
 */
chunks *download(b_dict *tracker_response);

void parse_peers(b_dict* tracker_response, peer peers[MAX_PEER]);

//peer_resp *handshake(char *peer_loc);
