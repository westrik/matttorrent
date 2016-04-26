#pragma once

#include "dict.h"

b_dict* parse_torrent_file(FILE* torrent);
b_dict* tracker_request(b_dict* metainfo, FILE* torrent);



