#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "acutest.h"
#include "LinkedList.h"
#include "Entry.h"

void test_create_list(void) {

    LinkedList el = NULL;
    if (create_list(&el, destroy_entry) == EC_FAIL)
        printf("Error! Create entry list failed\n");

    TEST_ASSERT(el!=NULL);
    TEST_CHECK(get_number_items(el) == 0);

    destroy_list(&el);
}

void testtest_get_number_items() {

    LinkedList el = NULL;
    if (create_list(&el, destroy_entry) == EC_FAIL)
        printf("Error! Create entry list failed\n");

    TEST_CHECK(get_number_items(el) == 0);

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
        if (add_item(el, e[i]) == EC_FAIL)
            printf("Error! Add entry failed\n");

        TEST_ASSERT(get_number_items(el) == (i+1));
    }

    destroy_list(&el);
}

void test_add_item(void) {

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

    LinkedList el = NULL;
    if (create_list(&el, destroy_entry) == EC_FAIL)
        printf("Error! Create entry list failed\n");

    for (int i=0; i<4; i++){
        if (add_item(el, e[i]) == EC_FAIL)
            printf("Error! Add entry failed\n");

        TEST_ASSERT(get_number_items(el) == (i+1));
//        entry e_temp = NULL;
//        e_temp = get_first(el);
        TEST_CHECK(get_first(el) == e[i]);
    }

    destroy_list(&el);

}

void test_pop_item(){
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

    LinkedList el = NULL;
    if (create_list(&el, destroy_entry) == EC_FAIL)
        printf("Error! Create entry list failed\n");

    for (int i=0; i<4; i++){
        if (add_item(el, e[i]) == EC_FAIL)
            printf("Error! Add entry failed\n");
    }

    entry temp = NULL;
    for (int i=0; i<4; i++){
        temp = pop_item(el);
        TEST_ASSERT(get_number_items(el) == (4-(i+1)));
        TEST_ASSERT(temp == e[4-(i+1)]);
        destroy_entry((void**)&temp);
    }

    destroy_list(&el);
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

    LinkedList el = NULL;
    if (create_list(&el, destroy_entry) == EC_FAIL)
        printf("Error! Create entry list failed\n");

    for (int i=0; i<4; i++){
        if (add_item(el, e[i]) == EC_FAIL)
            printf("Error! Add entry failed\n");

        TEST_CHECK(get_first(el) == e[i]);
    }

    destroy_list(&el);
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

    LinkedList el = NULL;
    if (create_list(&el, destroy_entry) == EC_FAIL)
        printf("Error! Create entry list failed\n");

    for (int i=0; i<4; i++){
        if (add_item(el, e[i]) == EC_FAIL)
            printf("Error! Add entry failed\n");
    }

    ListNode first_node = get_first_node(el);
    entry e_test = get_next(el, first_node);
    TEST_CHECK(e_test == e[2]);

    ListNode second_node = get_next_node(first_node);
    e_test = get_next(el, second_node);
    TEST_CHECK(e_test == e[1]);

    e_test = get_next(el, get_next_node(second_node));
    TEST_CHECK(e_test == e[0]);

    destroy_list(&el);
}

void test_destroy_list() {
    entry e[2];
    for (int i=0; i<2; i++) {
        e[i] = NULL;
    }

    if (create_entry("hell", &e[0]) == EC_FAIL)
        printf("Error! Create entry failed\n");
    if (create_entry("felt", &e[1]) == EC_FAIL)
        printf("Error! Create entry failed\n");

    LinkedList el = NULL;
    if (create_list(&el, destroy_entry) == EC_FAIL)
        printf("Error! Create entry list failed\n");

    for (int i=0; i<2; i++){
        if (add_item(el, e[i]) == EC_FAIL)
            printf("Error! Add entry failed\n");
    }

    entry temp = NULL;
    for (int i=0; i<2; i++){
        temp = pop_item(el);
        destroy_entry((void**)&temp);
    }
    destroy_list(&el);
    TEST_CHECK(el == NULL);
}

TEST_LIST = {
        { "create_list", test_create_list },
        { "get_number_items", testtest_get_number_items },
        { "add_item", test_add_item },
        { "pop_item", test_pop_item },
        { "get_first", test_get_first },
        { "get_next", test_get_next },
        { "destroy_list", test_destroy_list },
        { NULL, NULL }
};
