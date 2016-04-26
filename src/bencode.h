#pragma once

#define BUFFER_SIZE 256
#include "dict.h"

/* ========================================
 * Function declarations
 * ======================================== */

b_dict* parse_bencode_dict(char* input);

b_dict* __parse_dict (char* input, int* position);


