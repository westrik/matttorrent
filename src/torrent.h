#pragma once

#include "dict.h"
#include "bencode.h"

b_dict* parse_torrent_file(FILE* torrent_f);
b_dict* tracker_request(b_dict* metainfo, FILE* torrent_f);
