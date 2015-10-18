#ifndef MA3T_BENCODE_H
#define MA3T_BENCODE_H

#define BUFFER_SIZE 256
#include "dict.h"

/* ========================================
 * Data structure declarations
 * ======================================== */

/* ========================================
 * Function declarations
 * ======================================== */

b_dict* bencode_parse(FILE* input);

#endif //MA3T_BENCODE_H
