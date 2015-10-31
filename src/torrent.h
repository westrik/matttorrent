#ifndef MA3T_TORRENT_H
#define MA3T_TORRENT_H

#include "dict.h"

b_dict* parse_torrent_file(FILE* torrent);
b_dict* tracker_request(b_dict* metainfo, FILE* torrent);



#endif //MA3T_TORRENT_H
