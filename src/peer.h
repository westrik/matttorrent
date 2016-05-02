#pragma once

#include "torrent.h"
#include "chunk.h"

/**
 * Take list of peers from tracker,
 * start communicating with them and downloading chunks.
 * @return chunks struct (all file chunks)
 */
chunks *download(b_dict *tracker_response);

//peer_resp *handshake(char *peer_loc);
