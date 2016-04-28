#pragma once

#include <stdint.h>
typedef enum {false, true} bool;

/* ========================================
 * Data structure declarations
 * ======================================== */

struct _b_dict; // forward declaration
struct _b_dict_element; // forward declaration

/**
 * Union of possible element types
 */
union b_dict_el
{
    struct _b_dict* d;
    char* c;
    int64_t i;
    struct _b_dict_element* l;
};
typedef enum {DICT, STRING, INT, LIST} b_dict_el_t;

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

b_dict* dict_init(unsigned int size);

bool dict_insert(b_dict* dict, b_dict_element* el);

int __dict_key_hash(char* key, int dict_size);

bool __dict_fill_slot(b_dict_element* el, b_dict_element** map_el);

b_dict_element* dict_find(b_dict* dict, char* key);

b_dict* dict_destroy(b_dict* dict);

void __free_linked_list(b_dict_element* el);

const char* get_data_type(b_dict_el_t t);
