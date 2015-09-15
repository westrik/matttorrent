#ifndef MA3T_BENCODE_H
#define MA3T_BENCODE_H

#define BUFFER_SIZE 256


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
    b_dict_el_t type;
    union b_dict_el element; 
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

b_dict bencode_parse(FILE* input);





#endif //MA3T_BENCODE_H
