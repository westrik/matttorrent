#pragma once

#include "bencode.h"
#include "dict.h"

typedef struct _chunk
{
    int x;
}
chunk;

typedef struct _chunks
{
    chunk ** chunks;
    int num;
}
chunks;


/**
 * Combine a grouping of chunks, writing the result to a file
 * @return true if successful, false otherwise
 */
bool save_chunks(chunks *chunks, FILE *out);


/**
 * Free memory associated with file chunks
 */
void free_chunks(chunks *chunks);
