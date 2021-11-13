#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "acutest.h"
#include "Entry.h"

//#define macro TEST_NO_MAIN

void test_create(void) {

    char *w = "hell";
    entry e1 = NULL;
    if (create_entry(w, &e1) == EC_FAIL)
        printf("Error! Create entry failed\n");
    TEST_ASSERT(e1 != NULL);
    TEST_CHECK(strcmp(w, get_entry_word(e1)) == 0);

    if (destroy_entry((void**)&e1) == EC_FAIL)
        printf("Error! Destroy entry failed\n");
}

void test_get_entry_word(void) {

    char *w = "hell";
    entry e1 = NULL;
    if (create_entry(w, &e1) == EC_FAIL)
        printf("Error! Create entry failed\n");

    TEST_ASSERT(get_entry_word(e1)!=NULL);
    TEST_CHECK(strcmp(w, get_entry_word(e1)) == 0);

    if (destroy_entry((void**)&e1) == EC_FAIL)
        printf("Error! Destroy entry failed\n");
}

void test_destroy_entry(void) {

    char *w = "hell";
    entry e1 = NULL;
    if (create_entry(w, &e1) == EC_FAIL)
        printf("Error! Create entry failed\n");

    if (destroy_entry((void**)&e1) == EC_FAIL)
        printf("Error! Destroy entry failed\n");
    TEST_CHECK(e1 == NULL);
}

TEST_LIST = {
    { "create entry", test_create },
    { "get_entry_word", test_get_entry_word },
    { "destroy_entry", test_destroy_entry },
    { NULL, NULL }
};
