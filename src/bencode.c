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
    b_dict* result = calloc(1,sizeof(b_dict));

    while (input[*position] != 0)
    {
        if (input[*position] == 'd')
        {
            __parse_dict(input, position);
        }
        else if (input[*position] == 'i')
        {
            __parse_int(input, position);
        }
        else if (input[*position] == 'l')
        {
            printf("LIST");
        }
        else if (isdigit(input[*position]))
        {

        }
        else if (input[*position] == 'e')
        {
            printf("END\n"); 
        }
        else
        {
            printf("Syntax error\n");
            exit(1);
        }

        (*position)++;
    }

    return result;
}

int __parse_int (char* input, int* position)
{

}
