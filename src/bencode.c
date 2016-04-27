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
    char* last_key = 0;

    while (input[*position] != 0)
    {
        if (isdigit(input[*position]))
        {
            // Element key
            last_key = __parse_string(input, position);
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
            printf("ERR: Integer length exceeds buffer size\n");
            exit(1);
        }

        (*position)++;
    }
    buffer[i] = '\0';

    return atoi(buffer);

}

char* __parse_string (char* input, int* position)
{
    // str format: <string length b10 ascii>:<string>
    // parse string length (until :), then iterate over string
    
    int string_len = 0;
    char buffer[2048]; // standard buffer size seems too small
    int i = 0;

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
            printf("ERR: Invalid string length\n");
            exit(1);
        }

        if (++i == 2048)
        {
            printf("ERR: Integer length exceeds buffer size\n");
            exit(1);
        }

        (*position)++;
    }

    i = 0;
    while (i < string_len)
    {
        buffer[i] = input[*position];
        
        if (++i == 2048)
        {
            printf("ERR: Integer length exceeds buffer size\n");
            exit(1);
        }
        (*position)++;
    }
    buffer[i] = '\0';

    char* result = calloc(strlen(buffer)+1,sizeof(char));
    strcpy(result, buffer);

    return result;
}


b_dict_element* __parse_list (char* input, int* position)
{
    // list format: l<values>e

}
