#pragma once

#include "dict.h"
#include "bencode.h"

#define PEER_ID "-M30001-"
#define BUFFER 512

/* ==========================================
 * Data structure declarations
 * ========================================== */

typedef struct _torrent_config
{
    char* announce;
    char* name;
    char* pieces;
    int64_t piece_len;
}
t_conf;


/* ==========================================
 * Function declarations
 * ========================================== */
char* dump_file_to_string(FILE* file);
t_conf* parse_torrent_file(FILE* torrent_f);
b_dict* tracker_request(t_conf* metainfo, FILE* torrent_f);

char* info_hash(FILE* torrent_f);
