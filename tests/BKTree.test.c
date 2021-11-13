#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "acutest.h"
#include "BKTree.h"

void test_build_entry_index(void) {

    /* add every word mentioned in the example to an entry list */
    entry e = NULL;
    entry_list entryList; create_entry_list(&entryList, destroy_entry);
    create_entry("melt", &e); add_entry(entryList, e);
    create_entry("small", &e); add_entry(entryList, e);
    create_entry("fell", &e); add_entry(entryList, e);
    create_entry("felt", &e); add_entry(entryList, e);
    create_entry("fall", &e); add_entry(entryList, e);
    create_entry("help", &e); add_entry(entryList, e);
    create_entry("hell", &e); add_entry(entryList, e);

    /* build a BK tree */
    BK_tree index = NULL;
    build_entry_index(entryList, MT_EDIT_DIST, &index);

    char string[] = "hell-3\nhelp-1\nfell-0\nfall-2\nfelt-0\nmelt-0\nsmall-0\n"; /* this is the expected tree */
    char* string_res = malloc(strlen(string)+1);
    BK_tree_toString(index, string_res);
    TEST_CHECK(strcmp(string, string_res) == 0);
    TEST_MSG("expected:\n%s", string);
    TEST_MSG("produced:\n%s", string_res);

    free(string_res);
    
    destroy_entry_index(&index);
    destroy_entry_list(&entryList);
}

void test_lookup_entry_index(){

    entry e1 = NULL, e2 = NULL, e3 = NULL, e4 = NULL;
    if (create_entry("hell", &e1) == EC_FAIL)
        printf("Error! Create entry failed\n");
    if (create_entry("felt", &e2) == EC_FAIL)
        printf("Error! Create entry failed\n");
    if (create_entry("henn", &e3) == EC_FAIL)
        printf("Error! Create entry failed\n");
    if (create_entry("ken", &e4) == EC_FAIL)
        printf("Error! Create entry failed\n");

    entry_list el = NULL;
    if (create_entry_list(&el, destroy_entry) == EC_FAIL)
        printf("Error! Create entry list failed\n");

    if (add_entry(el, e1) == EC_FAIL)
        printf("Error! Add entry failed\n");
    if (add_entry(el, e2) == EC_FAIL)
        printf("Error! Add entry failed\n");
    if (add_entry(el, e3) == EC_FAIL)
        printf("Error! Add entry failed\n");
    if (add_entry(el, e4) == EC_FAIL)
        printf("Error! Add entry failed\n");

//    print_entry_list(el, print_entry_word);

    //BK_Tree test
    BK_tree ix = NULL;
    build_entry_index(el, MT_EDIT_DIST, &ix);
//    print_BK_tree(ix);

//    TEST_MSG("Find words similar to henn with max distance 2:\n");

    entry_list result;
    lookup_entry_index("henn", ix, 2, &result);
//    print_entry_list(result, print_entry_word);
    TEST_ASSERT(get_number_entries(result) == 3);
    entry result1 = pop_entry(result), result2 = pop_entry(result), result3 = pop_entry(result);
    TEST_CHECK((strcmp(get_entry_word(result1), "henn") == 0)
               && (strcmp(get_entry_word(result2), "hell") == 0)
               && (strcmp(get_entry_word(result3), "ken") == 0) );

    destroy_entry_list(&result);
    destroy_entry_index(&ix);

    if (destroy_entry_list(&el) == EC_FAIL)
        printf("Error! Destroy entry list failed\n");

    /***************** Test lookup with another tree (assignment's example) *****************/
    /* add every word mentioned in the example to an entry list */
    entry e = NULL;
    entry_list entryList; create_entry_list(&entryList, destroy_entry);
    create_entry("melt", &e); add_entry(entryList, e);
    create_entry("small", &e); add_entry(entryList, e);
    create_entry("fell", &e); add_entry(entryList, e);
    create_entry("felt", &e); add_entry(entryList, e);
    create_entry("fall", &e); add_entry(entryList, e);
    create_entry("help", &e); add_entry(entryList, e);
    create_entry("hell", &e); add_entry(entryList, e);

    /* build a BK tree */
    BK_tree index = NULL;
    build_entry_index(entryList, MT_EDIT_DIST, &index);
//    print_BK_tree(index);

//    printf("Find words similar to henn with max distance 2:\n");

    entry_list res;
    lookup_entry_index("henn", index, 2, &res);

    TEST_ASSERT(get_number_entries(res) == 2);
    entry res1 = pop_entry(res), res2 = pop_entry(res);
    TEST_CHECK((strcmp(get_entry_word(res1), "help") == 0)
               && (strcmp(get_entry_word(res2), "hell") == 0) );

    destroy_entry_list(&res);
    destroy_entry_list(&entryList);
    destroy_entry_index(&index);

}

void test_destroy_index(){
    entry e1 = NULL, e2 = NULL, e3 = NULL, e4 = NULL;
    if (create_entry("hell", &e1) == EC_FAIL)
        printf("Error! Create entry failed\n");
    if (create_entry("felt", &e2) == EC_FAIL)
        printf("Error! Create entry failed\n");
    if (create_entry("henn", &e3) == EC_FAIL)
        printf("Error! Create entry failed\n");
    if (create_entry("ken", &e4) == EC_FAIL)
        printf("Error! Create entry failed\n");

    entry_list el = NULL;
    if (create_entry_list(&el, destroy_entry) == EC_FAIL)
        printf("Error! Create entry list failed\n");

    if (add_entry(el, e1) == EC_FAIL)
        printf("Error! Add entry failed\n");
    if (add_entry(el, e2) == EC_FAIL)
        printf("Error! Add entry failed\n");
    if (add_entry(el, e3) == EC_FAIL)
        printf("Error! Add entry failed\n");
    if (add_entry(el, e4) == EC_FAIL)
        printf("Error! Add entry failed\n");

    //BK_Tree test
    BK_tree ix = NULL;
    build_entry_index(el,MT_EDIT_DIST,&ix);

    entry_list result;
    lookup_entry_index("henn", ix, 2, &result);

    destroy_entry_list(&result);
    destroy_entry_index(&ix);
    TEST_CHECK(ix == NULL);

    if (destroy_entry_list(&el) == EC_FAIL)
        printf("Error! Destroy entry list failed\n");

}

TEST_LIST = {
        { "test build entry index", test_build_entry_index },
        { "test lookup entry index", test_lookup_entry_index },
        { "test destroy_index", test_destroy_index },
        { NULL, NULL }
};
