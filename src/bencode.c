#include <stdio.h>
#include <stdlib.h>

#include "bencode.h"

/**
 * Parse bencoded files
 * @param   FILE* input input file to be parsed
 * @return  b_dict
 */
b_dict* bencode_parse(FILE* input)
{
    char buffer[BUFFER_SIZE];
    b_dict* dict = NULL;

    int i;

    for (;;) {
        size_t n = fread(buffer, 1, BUFFER_SIZE, input);

        for (i=0;i<BUFFER_SIZE;i++)
        {
        }
        if (n < BUFFER_SIZE) { break; }
    }

    return dict;
}
