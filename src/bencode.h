#pragma once

#define BUFFER_SIZE 256
#define ERR(X) printf("ERR: %s\n",X);exit(1)

#include "dict.h"

/* ========================================
 * Function declarations
 * ======================================== */

b_dict* parse_bencode_dict(char* input);

b_dict* __parse_dict (char* input, int* position);
int64_t __parse_int (char* input, int* position);
char* __parse_string (char* input, int* position);
b_dict_element* __parse_list (char* input, int* position);
