#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "acutest.h"
#include "EntryList.h"
#include "Entry.h"

void test_create_entry_list(void) {

    entry_list el = NULL;
    if (create_entry_list(&el, destroy_entry) == EC_FAIL)
        printf("Error! Create entry list failed\n");

    TEST_ASSERT(el!=NULL);
    TEST_CHECK(get_number_entries(el) == 0);

    destroy_entry_list(el);
}

void test_get_number_entries() {

    entry_list el = NULL;
    if (create_entry_list(&el, destroy_entry) == EC_FAIL)
        printf("Error! Create entry list failed\n");

    TEST_CHECK(get_number_entries(el) == 0);

    entry e[3];
    for (int i=0; i<3; i++) {
        e[i] = NULL;
    }

    if (create_entry("hell", &e[0]) == EC_FAIL)
        printf("Error! Create entry failed\n");
    if (create_entry("felt", &e[1]) == EC_FAIL)
        printf("Error! Create entry failed\n");
    if (create_entry("henn", &e[2]) == EC_FAIL)
        printf("Error! Create entry failed\n");

    for (int i=0; i<3; i++){
        if (add_entry(el, e[i]) == EC_FAIL)
            printf("Error! Add entry failed\n");

        TEST_ASSERT(get_number_entries(el) == (i+1));
    }

    destroy_entry_list(el);
}

void test_add_entry(void) {

    entry e[4];
    for (int i=0; i<4; i++) {
        e[i] = NULL;
    }

    if (create_entry("hell", &e[0]) == EC_FAIL)
        printf("Error! Create entry failed\n");
    if (create_entry("felt", &e[1]) == EC_FAIL)
        printf("Error! Create entry failed\n");
    if (create_entry("henn", &e[2]) == EC_FAIL)
        printf("Error! Create entry failed\n");
    if (create_entry("ken", &e[3]) == EC_FAIL)
        printf("Error! Create entry failed\n");

    entry_list el = NULL;
    if (create_entry_list(&el, destroy_entry) == EC_FAIL)
        printf("Error! Create entry list failed\n");

    for (int i=0; i<4; i++){
        if (add_entry(el, e[i]) == EC_FAIL)
            printf("Error! Add entry failed\n");

        TEST_ASSERT(get_number_entries(el) == (i+1));
//        entry e_temp = NULL;
//        e_temp = get_first(el);
        TEST_CHECK(get_first(el) == e[i]);
    }

    destroy_entry_list(el);

}

void test_pop_entry(){
    entry e[4];
    for (int i=0; i<4; i++) {
        e[i] = NULL;
    }

    if (create_entry("hell", &e[0]) == EC_FAIL)
        printf("Error! Create entry failed\n");
    if (create_entry("felt", &e[1]) == EC_FAIL)
        printf("Error! Create entry failed\n");
    if (create_entry("henn", &e[2]) == EC_FAIL)
        printf("Error! Create entry failed\n");
    if (create_entry("ken", &e[3]) == EC_FAIL)
        printf("Error! Create entry failed\n");

    entry_list el = NULL;
    if (create_entry_list(&el, destroy_entry) == EC_FAIL)
        printf("Error! Create entry list failed\n");

    for (int i=0; i<4; i++){
        if (add_entry(el, e[i]) == EC_FAIL)
            printf("Error! Add entry failed\n");
    }

    entry temp = NULL;
    for (int i=0; i<4; i++){
        temp = pop_entry(el);
        TEST_ASSERT(get_number_entries(el) == (4-(i+1)));
        TEST_ASSERT(temp == e[4-(i+1)]);
        destroy_entry(temp);
    }

    destroy_entry_list(el);
}

void test_get_first() {

    entry e[4];
    for (int i=0; i<4; i++) {
        e[i] = NULL;
    }

    if (create_entry("hell", &e[0]) == EC_FAIL)
        printf("Error! Create entry failed\n");
    if (create_entry("felt", &e[1]) == EC_FAIL)
        printf("Error! Create entry failed\n");
    if (create_entry("henn", &e[2]) == EC_FAIL)
        printf("Error! Create entry failed\n");
    if (create_entry("ken", &e[3]) == EC_FAIL)
        printf("Error! Create entry failed\n");

    entry_list el = NULL;
    if (create_entry_list(&el, destroy_entry) == EC_FAIL)
        printf("Error! Create entry list failed\n");

    for (int i=0; i<4; i++){
        if (add_entry(el, e[i]) == EC_FAIL)
            printf("Error! Add entry failed\n");

        TEST_CHECK(get_first(el) == e[i]);
    }

    destroy_entry_list(el);
}

void test_get_next() {

    entry e[4];
    for (int i=0; i<4; i++) {
        e[i] = NULL;
    }

    if (create_entry("hell", &e[0]) == EC_FAIL)
        printf("Error! Create entry failed\n");
    if (create_entry("felt", &e[1]) == EC_FAIL)
        printf("Error! Create entry failed\n");
    if (create_entry("henn", &e[2]) == EC_FAIL)
        printf("Error! Create entry failed\n");
    if (create_entry("ken", &e[3]) == EC_FAIL)
        printf("Error! Create entry failed\n");

    entry_list el = NULL;
    if (create_entry_list(&el, destroy_entry) == EC_FAIL)
        printf("Error! Create entry list failed\n");

    for (int i=0; i<4; i++){
        if (add_entry(el, e[i]) == EC_FAIL)
            printf("Error! Add entry failed\n");
    }

    entry e_test = get_next(el, get_first_node(el));
    TEST_CHECK(e_test == e[3]);
    e_test = get_next(el, get_next_node(el));
    TEST_CHECK(e_test == e[2]);

    destroy_entry_list(el);
}

void test_destroy_entry_list() {
    entry e[2];
    for (int i=0; i<2; i++) {
        e[i] = NULL;
    }

    if (create_entry("hell", &e[0]) == EC_FAIL)
        printf("Error! Create entry failed\n");
    if (create_entry("felt", &e[1]) == EC_FAIL)
        printf("Error! Create entry failed\n");

    entry_list el = NULL;
    if (create_entry_list(&el, destroy_entry) == EC_FAIL)
        printf("Error! Create entry list failed\n");

    for (int i=0; i<2; i++){
        if (add_entry(el, e[i]) == EC_FAIL)
            printf("Error! Add entry failed\n");
    }

    entry temp = NULL;
    for (int i=0; i<2; i++){
        temp = pop_entry(el);
        destroy_entry(temp);
    }
    destroy_entry_list(el);
    TEST_CHECK(el == NULL);
}

TEST_LIST = {
        { "create_entry_list", test_create_entry_list },
        { "get_number_entries", test_get_number_entries },
        { "add_entry", test_add_entry },
        { "pop_entry", test_pop_entry },
        { "get_first", test_get_first },
        { "get_next", test_get_next },
        { "destroy_entry_list", test_destroy_entry_list },
        { NULL, NULL }
};
