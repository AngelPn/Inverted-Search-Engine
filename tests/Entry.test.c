#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "acutest.h"
#include "Entry.h"

// #define macro TEST_NO_MAIN

void test_example(void) {

    char *w = "hell";
    entry e1 = NULL;
    if (create_entry(w, &e1) == EC_FAIL)
        printf("Error! Create entry failed\n");

    TEST_CHECK(strcmp(w, get_entry_word(e1)) == 0);

    void* mem;

    mem = malloc(10);
    TEST_CHECK(mem != NULL);

    mem = realloc(mem, 20);
    TEST_CHECK(mem != NULL);

    int a, b;

    /* This condition is designed to fail so you can see what the failed test
     * output looks like. */
    a = 1;
    b = 4;
    TEST_CHECK(a + b == 5);

    /* Here is TEST_CHECK_ in action. */
    TEST_CHECK_(a + b == 5, "%d + %d == 5", a, b);

    /* We may also show more information about the failure. */
    if(!TEST_CHECK(a + b == 5)) {
        TEST_MSG("a: %d", a);
        TEST_MSG("b: %d", b);
    }

}

TEST_LIST = {
    { "example", test_example },
    { NULL, NULL }
};
