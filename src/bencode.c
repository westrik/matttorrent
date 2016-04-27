#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

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
    char* last_key = 0;

    while (input[*position] != 0)
    {
        if (isdigit(input[*position]))
        {
            // Element key
            __parse_string(input, position);
        }
        else if (input[*position] == 'd')
        {
            // Element: dictionary
            __parse_dict(input, position);
        }
        else if (input[*position] == 'i')
        {
            // Element: int
            __parse_int(input, position);
        }
        else if (input[*position] == 'l')
        {
            // Element: list
            __parse_list(input, position);
        }
        else if (input[*position] == 'e')
        {
            printf("END\n"); 
        }
        else
        {
            printf("%d\n",*position);
            printf("%c\n",input[*position]);
            printf("Syntax error\n");
            exit(1);
        }

        (*position)++;
    }

    return result;
}

int __parse_int (char* input, int* position)
{
    // int format: i12341234e
    // iterate from i to e, parsing what's in between 

}

char* __parse_string (char* input, int* position)
{
    // str format: <string length b10 ascii>:<string>
    // parse string length (until :), then iterate over string

}

b_dict_element* __parse_list (char* input, int* position)
{
    // list format: l<values>e

}
