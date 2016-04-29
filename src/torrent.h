#pragma once

#include "dict.h"
#include "bencode.h"

#define PEER_ID "-M30001-"
#define BUFFER 512
#define DEFAULT_PORT 6886

/* ==========================================
 * Data structure declarations
 * ========================================== */

typedef struct _torrent_config
{
    char *announce;
    char *name;
    char *pieces;
    int64_t piece_len;
}
t_conf;


typedef struct _response_chunk
{
    char *memory;
    size_t size;
}
response_chunk;


/* ==========================================
 * Function declarations
 * ========================================== */
char *dump_file_to_string(FILE* file);
t_conf *parse_torrent_file(FILE* torrent_f);
b_dict *tracker_request(t_conf* metainfo, FILE* torrent_f);

char *info_hash(FILE* torrent_f);
char *generate_peer_id();

static size_t write_mem_cb(void *contents, size_t size, size_t nmemb, void *userp);
