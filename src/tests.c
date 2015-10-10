#include <stdio.h>
#include <assert.h>

#include "bencode.h"

b_dict* bencode_dict_create()
{
    b_dict* dict = dict_init(5);

    assert(dict != NULL);
    assert(dict->size == 5);

    return dict;
}

int main()
{
    printf("Create dict\n");
    b_dict* dict = bencode_dict_create();

    printf("Tests complete\n");
    return 0;
}
