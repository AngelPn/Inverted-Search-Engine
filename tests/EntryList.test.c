#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "acutest.h"
#include "EntryList.h"
#include "Entry.h"

//#define macro TEST_NO_MAIN

void test_create(void) {

    entry_list el = NULL;
    if (create_entry_list(&el, destroy_entry) == EC_FAIL)
        printf("Error! Create entry list failed\n");

    TEST_ASSERT(el!=NULL);
    TEST_CHECK(get_number_entries(el) == 0);

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

//void test_list_destroy_item(){
//    entry_list el = NULL;
//    if (create_entry_list(&el, NULL) == EC_FAIL)
//        printf("Error! Create entry list failed\n");
//
//    entry e = NULL;
//    if (create_entry("hell", &e) == EC_FAIL)
//        printf("Error! Create entry failed\n");
//    add_entry(el, e);
//    destroy_entry_list(el);
//    TEST_CHECK(e != NULL);
//
//
//    el = NULL;
//    if (create_entry_list(&el, destroy_entry) == EC_FAIL)
//        printf("Error! Create entry list failed\n");
//
//    if (e == NULL){
//        if (create_entry("hell", &e) == EC_FAIL)
//            printf("Error! Create entry failed\n");
//    }
//    add_entry(el, e);
//    destroy_entry_list(el);
//
//    TEST_CHECK(e == NULL);
//
//    if (e!=NULL){
//        destroy_entry(e);
//    }
//
//}
//

TEST_LIST = {
        { "create_entry_list", test_create },
        { "add_entry", test_add_entry },
        { "pop_entry", test_pop_entry },
//        { "destroy list items", test_list_destroy_item },
        { NULL, NULL }
};
