#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "bencode.h"

/**
 * Parse bencoded files
 * @param   FILE* input input file to be parsed
 * @return  b_dict
 */
b_dict bencode_parse(FILE* input)
{
    char buffer[BUFFER_SIZE];

    for (;;) {
        size_t n = fread(buffer, 1, BUFFER_SIZE, input);

        printf("%s\n\n",buffer);
        if (n < BUFFER_SIZE) { break; }
    }
}
