#ifndef MA3T_BENCODE_H
#define MA3T_BENCODE_H

#define BUFFER_SIZE 256
#include <stdint.h>

typedef enum {false, true} bool;

/* ========================================
 * Data structure declarations
 * ======================================== */

struct _b_dict; // forward declaration

/**
 * Union of possible element types
 */
union b_dict_el
{
    struct _b_dict* d;
    char* c;
    int64_t i;
};
typedef enum {DICT, STRING, INT} b_dict_el_t;

/**
 * Bencoded item
 */
typedef struct _b_dict_element
{
    char* key;
    b_dict_el_t type;
    union b_dict_el element; 

    struct _b_dict_element* next;
}
b_dict_element;

/**
 * Hashmap containing bencoded items
 */
typedef struct _b_dict
{
    unsigned int size;
    b_dict_element** map;
}
b_dict;



/* ========================================
 * Function declarations
 * ======================================== */

b_dict* bencode_parse(FILE* input);

b_dict* dict_init(unsigned int size);

bool dict_insert(b_dict* dict, b_dict_element* el);

int dict_key_hash(char* key, int dict_size);

bool dict_fill_slot(b_dict_element* el, b_dict_element** map_el);

b_dict_element* dict_find_element(b_dict* dict, char* key);

#endif //MA3T_BENCODE_H

