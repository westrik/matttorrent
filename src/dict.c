#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "dict.h"

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

    dict->map = calloc(size, sizeof(b_dict_element*));
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
    b_dict_element** current;

    if (NULL == dict || NULL == el)
    {
        return false;
    }

    if (NULL != dict_find(dict, el->key))
    {
        return false;
    }

    index = __dict_key_hash(el->key, dict->size);
    if (index < 0)
    {
        return false;
    }

    current = &dict->map[index];
    while (true)
    {
        if (*current == NULL)
        {
            el->next = NULL;
            *current = el;
            break;
        }
        else if ((*current)->next == NULL)
        {
            el->next = NULL;
            (*current)->next = el;
            break;
        }
        else
        {
            current = &((*current)->next);
        }
    }

    return true;
}

/**
 * Find element
 * @param b_dict* dictionary to search in
 * @param char* key of element
 * @return b_dict_element*
 */
b_dict_element* dict_find(b_dict* dict, char* key)
{
    int index;
    b_dict_element** current;

    if (NULL==dict || NULL==key)
    {
        return NULL;
    }

    index = __dict_key_hash(key, dict->size);

    current = &(dict->map[index]);

    while (true)
    {
        if (*current == NULL)
        {
            return NULL;
        }
        else if (0==strcmp((*current)->key,key))
        {
            return *current;
        }
        else
        {
            current = &((*current)->next);
        }
    }
}

/**
 * Free all memory associated with a hashmap
 * @param b_dict* dict: hashmap to be freed
 * @return NULL, for easy clearing of pointers
 */
b_dict* dict_destroy(b_dict* dict)
{
    if (dict==NULL)
        return NULL;

    for (int i = 0; i < dict->size; i++)
    {
        __free_linked_list(dict->map[i]);
    }

    free(dict);

    return NULL;
}

/**
 * Recursively frees an element linked list
 * @param b_dict_element* el: Head of list 
 */
void __free_linked_list(b_dict_element* el)
{
    if (NULL == el)
    {
        return;
    }

    /* Start freeing at end of list, work backwards */
    if (NULL != el->next)
    {
        __free_linked_list(el->next);
    }

    /* Free data contained by node */
    if (el->type == DICT)
    {
        dict_destroy(el->element.d);
    }
    else if (el->type == LIST)
    {
        __free_linked_list(el->element.l);
    }
    else if (el->type == STRING)
    {
        if (NULL != el->element.c)
        {
            free(el->element.c);
        }
    }

    free(el->key);

    free(el);
}


/**
 * Count length of linked list
 * @param b_dict_element* el: Head of list 
 */
int count_linked_list(b_dict_element* el)
{
    if (NULL == el)
        return 0;

    else if (NULL == el->next)
        return 1;

    else
        return count_linked_list(el->next) + 1;
}

/**
 * Generate hash index for given key
 * @param char* key: key
 * @param int dict_size: size of hashmap
 * @return int index, or -1 if error
 */

int __dict_key_hash(char* key, int dict_size)
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
 * Return string representation of enum data type
 * @return const char*
 */
const char* get_data_type(b_dict_el_t t)
{
    switch (t) 
    {
        case DICT:      
            return "DICT";  
        case STRING:    
            return "STRING"; 
        case INT:       
            return "INT";   
        case LIST:      
            return "LIST";
        default:
            return "UNIMPL";
    }
}
