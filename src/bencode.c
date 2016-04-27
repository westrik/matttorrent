#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "bencode.h"

/**
 * Parse bencoded files
 * @param   char* input input string
 * @return  b_dict
 */
b_dict* parse_bencode_dict(char* input)
{
    b_dict* result;
    int* position;

    if (input[0] != 'd')
    {
        return NULL;
    }
        
    position = malloc(sizeof(int));
    *position = 1;

    result = __parse_dict(input, position);

    if (position)
    {
        free(position);
    }

    return result;
}

/* 
 * Parse a dictionary
 * @return b_dict Resulting dictionary
 */
b_dict* __parse_dict (char* input, int* position)
{
    // Iterate over input string, parsing element keys then elements
    // and inserting those into our dict adt
 
    b_dict* result = calloc(1,sizeof(b_dict));

    char* key = 0;
    bool parsing_element = false;

    union b_dict_el el;
    b_dict_el_t el_type;
    b_dict_element* dict_el;

    while (input[*position] != 0)
    {
        if (isdigit(input[*position]))
        {
            if (!parsing_element)
            {
                // Element key
                key = __parse_string(input, position);
            }
            else
            {
                // or Element: string
                el.c = __parse_string(input, position);
                el_type = STRING;
            }
        }
        else if (input[*position] == 'd')
        {
            // Element: dictionary
            el.d = __parse_dict(input, position);
            el_type = DICT;
        }
        else if (input[*position] == 'i')
        {
            // Element: int
            el.i = __parse_int(input, position);
            el_type = INT;
        }
        else if (input[*position] == 'l')
        {
            // Element: list
            el.l = __parse_list(input, position);
            el_type = LIST;
        }
        else if (input[*position] == 'e')
        {
            printf("END\n"); 
        }
        else
        {
            printf("%d\n",*position);
            printf("%c\n",input[*position]);
            ERR("Syntax error");
        }

        // Add new elements to dict 
        if (parsing_element)
        {
            dict_el = calloc(1, sizeof(b_dict_element));
            dict_el->key = key;
            dict_el->type = el_type;
            dict_el->element = el;

            dict_insert(result, dict_el);
        }
        parsing_element = !parsing_element;

        (*position)++;
    }

    return result;
}

int __parse_int (char* input, int* position)
{
    // int format: i<integer>e
    // iterate from i to e, parse what's in between as base10 int
    
    char buffer[BUFFER_SIZE];
    int i = 0;
    
    while (input[*position] != 'e')
    {
        if (input[*position] != 'i' 
                && (isdigit(input[*position]) || input[*position] == '-'))
        {
            buffer[i] = input[*position];
        }

        if (++i == BUFFER_SIZE)
        {
            ERR("Integer length exceeds buffer size");
        }

        (*position)++;
    }
    buffer[i] = '\0';

    return atoi(buffer);

}

char* __parse_string (char* input, int* position)
{
    // str format: <string length b10 ascii>:<string>
    
    int string_len = 0;
    char buffer[2048]; // standard buffer size seems too small
    int i = 0;

    char* result;
    
    // parse string length (until :)
    while (true)
    {
        if (input[*position] == ':')
        {
            buffer[i] = '\0';
            string_len = atoi(buffer);
            break;
        }
        else if (isdigit(input[*position]))
        {
            buffer[i] = input[*position];
        }
        else
        {
            ERR("Invalid string length");
        }

        if (++i == 2048)
        {
            ERR("Integer length exceeds buffer size");
        }

        (*position)++;
    }

    // then iterate over actual string
    i = 0;
    while (i < string_len)
    {
        buffer[i] = input[*position];
        
        if (++i == 2048)
        {
            ERR("Integer length exceeds buffer size");
        }
        (*position)++;
    }
    buffer[i] = '\0';

    result = calloc(strlen(buffer)+1,sizeof(char));
    strcpy(result, buffer);

    return result;
}


b_dict_element* __parse_list (char* input, int* position)
{
    // list format: l<values>e
    
    b_dict_element* el = calloc(1,sizeof(b_dict_element));
    b_dict_element* pos = el;

    while (input[*position] != 'e')
    {
        if (isdigit(input[*position]))
        {
            // Element: string
            pos->element.c = __parse_string(input, position);
            pos->type = STRING;
        }
        else if (input[*position] == 'd')
        {
            // Element: dictionary
            pos->element.d = __parse_dict(input, position);
            pos->type = DICT;
        }
        else if (input[*position] == 'i')
        {
            // Element: int
            pos->element.i = __parse_int(input, position);
            pos->type = INT;
        }
        else if (input[*position] == 'l')
        {
            // Element: list
            pos->element.l = __parse_list(input, position);
            pos->type = LIST;
        }
        else
        {
            printf("%d\n",*position);
            printf("%c\n",input[*position]);
            ERR("Syntax error");
        }
        
        (*position)++;

        if (input[*position] != 'e')
        {
            pos->next = calloc(1,sizeof(b_dict_element));
            pos = pos->next;
        }
    }

}
