#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "dict.h"
#include "bencode.h"

#define log(X) printf(X);fflush(stdout);

/** 
 * Tests for 
 * DICTIONARY ADT 
 *
 * dict.c
 */
b_dict* test_dict_create()
{
    log("- Create dict: ");

    b_dict* dict = dict_init(5);

    assert(dict != NULL);
    assert(dict->size == 5);

    log("pass\n");

    return dict;
}
void test_dict_insert_int(b_dict *dict)
{
    b_dict_element* el;
    log("\n  - Inserting int: ");

    el = calloc(1, sizeof(b_dict_element));
    assert(NULL != el);
    
    el->key = calloc(strlen("Test")+1, sizeof(char));
    strcpy(el->key, "Test");
    el->type = INT;
    el->element.i = 10;
    assert(NULL == el->next);
    assert(dict_insert(dict, el));
    log("pass\n");
}
void test_dict_insert_dupe(b_dict *dict)
{
    b_dict_element* el;

    log("  - Inserting element with same key twice should fail: ");

    el = calloc(1, sizeof(b_dict_element));
    assert(NULL != el);
    
    el->key = calloc(strlen("Test")+1, sizeof(char));
    strcpy(el->key, "Test");
    el->type = INT;
    el->element.i = 10;
    assert(NULL == el->next);
    assert(!dict_insert(dict, el));
    log("pass\n");
}
void test_dict_insert_string(b_dict *dict)
{
    b_dict_element* el;

    log("  - Inserting string: ");

    el = calloc(1, sizeof(b_dict_element));
    assert(NULL != el);

    el->key = calloc(strlen("Test2")+1, sizeof(char));
    strcpy(el->key, "Test2");
    el->type = STRING;
    el->element.c = calloc(strlen("Hello World\n")+1, sizeof(char));
    strcpy(el->element.c,"Hello World\n");
    assert(NULL == el->next);
    assert(dict_insert(dict, el));
    log("pass\n");
}
void test_dict_insert_dict(b_dict *dict)
{
    b_dict_element* el;

    log("  - Inserting dict: ");

    el = calloc(1, sizeof(b_dict_element));
    assert(NULL != el);
    
    el->key = calloc(strlen("Test3")+1, sizeof(char));
    strcpy(el->key, "Test3");
    el->type = DICT;
    el->element.d = dict_init(5);

    assert(NULL == el->next);
    assert(dict_insert(dict, el));
    log("pass\n");
}
void test_dict_insert(b_dict *dict)
{
    log("- Insertion");

    test_dict_insert_int(dict);
    test_dict_insert_dupe(dict);
    test_dict_insert_string(dict);
    test_dict_insert_dict(dict);
}
void test_dict_find(b_dict *dict)
{
    b_dict_element* el;

    log("- Retrieve previously inserted elements from dict: ");

    el = dict_find(dict, "Test");
    assert(el!=NULL);
    assert(el->element.i==10);

    el = dict_find(dict, "Test2");
    assert(el!=NULL);
    assert(0==strcmp(el->element.c,"Hello World\n"));

    el = dict_find(dict, "Test3");
    assert(el!=NULL);

    el = dict_find(dict, "DOES_NOT_EXIST");
    assert(el==NULL);

    log("pass\n");
}
void test_dict_destroy(b_dict* dict)
{
    log("- Dict and associated data can be freed: ");
    dict = dict_destroy(dict);
    assert(NULL == dict);
    log("pass\n");
}
void dict_tests()
{
    log("Dictionary ADT\n");
    b_dict* dict = test_dict_create();
    test_dict_insert(dict);
    test_dict_find(dict);
    test_dict_destroy(dict);
    log("\n\n");
}


/** 
 * Tests for 
 * BENCODING PARSER 
 *
 * bencode.c
 */

void bencode_tests()
{
    log("Bencoding parser\n");

    char* test_dict = malloc(sizeof(char)*25);
    strcpy(test_dict, "d3:cow3:moo4:spam4:eggse\0");

    b_dict* result = parse_bencode_dict(test_dict);
    free(test_dict);

    assert(result);
    log("\n\n");
}








int main()
{
    log("\n");

    dict_tests();
    bencode_tests();

    log("---- All tests passed ----\n");
    return 0;
}
