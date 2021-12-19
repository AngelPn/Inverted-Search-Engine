#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "acutest.h"
#include "HashTable.h"


void str_delete(void* str){
    free(str);
    str = NULL;
}

void test_HashT_init(){
    HashT* ht = HashT_init(string, 10, NULL);
    TEST_ASSERT(ht!=NULL);
    TEST_CHECK(HashT_get_nbuckets(ht)>10);
    TEST_CHECK(HashT_get_nitems(ht)==0);
    HashT_delete(ht);
}

void test_HashT_insert_get(){
    HashT* ht = HashT_init(string, 1, str_delete);

    char* item1 = malloc(strlen("hello there")+1);
    strcpy(item1, "hello there");

    char* item2 = malloc(strlen("general kenobi")+1);
    strcpy(item2, "general kenobi");

    HashT_insert(ht,"henn", item1);
    TEST_CHECK(HashT_get_nitems(ht) == 1);
    TEST_CHECK(HashT_exists(ht, "henn") == true);
    TEST_CHECK(HashT_get(ht, "henn") == item1);

    HashT_insert(ht,"hello", item2);
    TEST_CHECK(HashT_get_nitems(ht) == 2);
    TEST_CHECK(HashT_exists(ht, "hello") == true);
    TEST_CHECK(HashT_get(ht, "hello") == item2);

    /* second insert with same key should not add */
    HashT_insert(ht,"hello", item2);
    TEST_CHECK(HashT_get_nitems(ht) == 2);
    TEST_CHECK(HashT_exists(ht, "hello") == true);

    /* big loop */
    char* key[100];
    for (int i=0; i<100; i++) {
        
        char* item = malloc(strlen("hello there")+1);
        strcpy(item, "hello there");
        
        key[i] = malloc(strlen("key")+4);
        strcpy(key[i], "key");
        char i_str[4];
        sprintf(i_str, "%d", i);
        strcat(key[i], i_str);

        TEST_ASSERT(HashT_insert(ht, key[i], item) == true);

        TEST_ASSERT(HashT_get_nitems(ht) == i+3);
        TEST_ASSERT(HashT_exists(ht, key[i]) == true);
        TEST_ASSERT(HashT_get(ht, key[i]) == item);
    }

    HashT_delete(ht); /* items & hash table structs are freed */

    for (int i=0; i<100; i++) { /* keys are not freed by the hash table */
        free(key[i]);
    }
    
}

void test_HashT_remove(){
    HashT* ht = HashT_init(string, 10, str_delete);

    TEST_CHECK(HashT_get_nitems(ht) == 0);
    char* item1 = malloc(strlen("hello there")+1);
    strcpy(item1, "hello there");

    char* item2 = malloc(strlen("general kenobi")+1);
    strcpy(item2, "general kenobi");

    HashT_insert(ht,"henn", item1);
    HashT_insert(ht,"hello", item2);
    TEST_CHECK(HashT_get_nitems(ht) == 2);

    HashT_remove(ht, "hello");
    TEST_CHECK(HashT_get_nitems(ht) == 1);
    TEST_CHECK(HashT_exists(ht, "hello") == false);
    TEST_CHECK(HashT_get(ht, "hello") == NULL);
    TEST_CHECK(HashT_exists(ht, "henn") == true);

    HashT_remove(ht, "henn");
    TEST_CHECK(HashT_get_nitems(ht) == 0);
    TEST_CHECK(HashT_exists(ht, "henn") == false);
    TEST_CHECK(HashT_get(ht, "henn") == NULL);

    HashT_delete(ht);
}

void test_HashT_extend(){
    HashT* ht = HashT_init(string, 4, str_delete);
    TEST_CHECK(HashT_get_nbuckets(ht)==4);

    /* big loop */
    char* key[100];
    for (int i=0; i<100; i++) {
        
        char* item = malloc(strlen("hello there")+1);
        strcpy(item, "hello there");
        
        key[i] = malloc(strlen("key")+3);
        strcpy(key[i], "key");
        char i_str[4];
        sprintf(i_str, "%d", i);
        strcat(key[i], i_str);

        HashT_insert(ht,key[i], item);
    }
    // printf("nbuckets is %d\n", HashT_get_nbuckets(ht));
    TEST_CHECK(HashT_get_nbuckets(ht)>4);

    HashT_delete(ht);

    for (int i=0; i<100; i++) { /* keys are not freed by the hash table */
        free(key[i]);
    }
}

/* For hash table delete test run these tests with valgrind */

TEST_LIST = {
        { "hash table create",  test_HashT_init},
        { "hash table insert and get item",  test_HashT_insert_get},
        { "hash table remove item",  test_HashT_remove},
        { "hash table extend",  test_HashT_extend},
        { NULL, NULL }
};