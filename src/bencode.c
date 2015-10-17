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

/**
 * Initialize bencode dictionary
 * @param int size: size of key array
 *        (can be smaller than number of total elements)
 * @return b_dict*: new dict or NULL (malloc failure)
 */
b_dict* dict_init(unsigned int size)
{
    b_dict* dict = NULL;

    if (size < 1)
    {
        return dict;
    }

    dict = malloc(sizeof(b_dict));
    if (NULL == dict)
    {
        return NULL;
    }

    dict->map = malloc(sizeof(b_dict_element*)*size);
    if (NULL == dict->map)
    {
        free(dict);
        return NULL;
    }

    dict->size = size;

    return dict;
}

/**
 * Insert element into dictionary
 * @param b_dict* dict: dictionary
 * @param b_dict_element* el: element to be inserted
 * @return bool: status
 */
bool dict_insert(b_dict* dict, b_dict_element* el)
{
    int index;

    if (NULL == dict || NULL == el)
    {
        return false;
    }

    index = dict_key_hash(el->key, dict->size);
    if (index < 0)
    {
        return false;
    }

    // recursively add element
    dict_fill_slot(el, &dict->map[index]);

    return true;
}


/**
 * Add element to dictionary slot recursively
 * @param b_dict_element* el: element to be added
 * @param b_dict_element** map_el: dict slot
 */
bool dict_fill_slot(b_dict_element* el, b_dict_element** map_el)
{
    if (NULL==el)
    {
        return false;
    }
    if (NULL==*(map_el))
    {
        (*map_el) = el;
    }
    else if (NULL==(*map_el)->next)
    {
        (*map_el)->next = el;
    }
    else
    {
        dict_fill_slot(el, &(*map_el)->next);
    }
    return true;
}

/**
 * Generate hash index for given key
 * @param char* key: key
 * @param int dict_size: size of hashmap
 * @return int index, or -1 if error
 */

int dict_key_hash(char* key, int dict_size)
{
    int sum = 0;

    if (NULL == key || dict_size == 0)
    {
        return -1;
    }

    for (int i = 0; i < strlen(key); i++)
    {
        sum += (int)key[i];
    }
    return (sum % dict_size);
}

/**
 * Find element
 * @param b_dict* dictionary to search in
 * @param char* key of element
 * @return b_dict_element*
 */
b_dict_element* dict_find_element(b_dict* dict, char* key)
{
    int index;

    if (NULL==dict||NULL==key)
    {
        return NULL;
    }

    index = dict_key_hash(key, dict->size);

}
