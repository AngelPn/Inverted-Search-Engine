#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "core.h"
#include "acutest.h"
#include "BKTree.h"

void test_create_BK_tree(void) {
    BK_tree index = create_BK_tree(EditDistance);
    TEST_ASSERT(index != NULL);
    destroy_BK_tree(&index);
}


void test_insert_BK_tree_EditDistance(void) {

    BK_tree index = create_BK_tree(EditDistance);
    TEST_ASSERT(index != NULL);

    /* add every word mentioned in the example to an entry list */
    entry e = NULL;
    create_entry("hell", &e); BK_tree_insert(index, get_root_double_p(index), e);
    create_entry("help", &e); BK_tree_insert(index, get_root_double_p(index), e);
    create_entry("fall", &e); BK_tree_insert(index, get_root_double_p(index), e);
    create_entry("felt", &e); BK_tree_insert(index, get_root_double_p(index), e);
    create_entry("fell", &e); BK_tree_insert(index, get_root_double_p(index), e);
    create_entry("small", &e); BK_tree_insert(index, get_root_double_p(index), e);
    create_entry("melt", &e); BK_tree_insert(index, get_root_double_p(index), e);

    char string[] = "hell-3\nhelp-1\nfell-0\nfall-2\nfelt-0\nmelt-0\nsmall-0\n"; /* this is the expected tree */
    char* string_res = malloc(strlen(string)+1);
    BK_tree_toString(index, string_res);
    TEST_CHECK(strcmp(string, string_res) == 0);
    TEST_MSG("expected:\n%s", string);
    TEST_MSG("produced:\n%s", string_res);

    free(string_res);
    destroy_BK_tree(&index);
}


void test_insert_BK_tree_HammingDistance(void) {

    BK_tree index = create_BK_tree(HammingDistance);
    TEST_ASSERT(index != NULL);

    /* add every word mentioned in the example to an entry list */
    entry e = NULL;
    create_entry("book", &e); BK_tree_insert(index, get_root_double_p(index), e);
    create_entry("look", &e); BK_tree_insert(index, get_root_double_p(index), e);
    create_entry("lock", &e); BK_tree_insert(index, get_root_double_p(index), e);
    create_entry("dock", &e); BK_tree_insert(index, get_root_double_p(index), e);
    create_entry("duck", &e); BK_tree_insert(index, get_root_double_p(index), e);
    create_entry("move", &e); BK_tree_insert(index, get_root_double_p(index), e);

    char string[] = "book-3\nlook-0\nlock-1\ndock-0\nduck-1\nmove-0\n"; /* this is the expected tree */
    char* string_res = malloc(strlen(string)+1);
    BK_tree_toString(index, string_res);
    TEST_CHECK(strcmp(string, string_res) == 0);
    TEST_MSG("expected:\n%s", string);
    TEST_MSG("produced:\n%s", string_res);

    free(string_res);
    destroy_BK_tree(&index);
}

// void test_lookup_entry_index(){

//     entry e1 = NULL, e2 = NULL, e3 = NULL, e4 = NULL;
//     if (create_entry("hell", &e1) == EC_FAIL)
//         printf("Error! Create entry failed\n");
//     if (create_entry("felt", &e2) == EC_FAIL)
//         printf("Error! Create entry failed\n");
//     if (create_entry("henn", &e3) == EC_FAIL)
//         printf("Error! Create entry failed\n");
//     if (create_entry("ken", &e4) == EC_FAIL)
//         printf("Error! Create entry failed\n");

//     LinkedList el = NULL;
//     if (create_list(&el, NULL) == EC_FAIL)
//         printf("Error! Create entry list failed\n");

//     if (add_item(el, e1) == EC_FAIL)
//         printf("Error! Add entry failed\n");
//     if (add_item(el, e2) == EC_FAIL)
//         printf("Error! Add entry failed\n");
//     if (add_item(el, e3) == EC_FAIL)
//         printf("Error! Add entry failed\n");
//     if (add_item(el, e4) == EC_FAIL)
//         printf("Error! Add entry failed\n");

// //    print_list(el, print_entry_word);

//     //BK_Tree test
//     BK_tree ix = NULL;
//     build_entry_index(el, MT_EDIT_DIST, &ix);
// //    print_BK_tree(ix);

// //    TEST_MSG("Find words similar to henn with max distance 2:\n");

//     LinkedList result;
//     lookup_entry_index("henn", ix, 2, &result);
// //    print_list(result, print_entry_word);
//     TEST_ASSERT(get_number_items(result) == 3);
//     entry result1 = pop_item(result), result2 = pop_item(result), result3 = pop_item(result);
//     TEST_CHECK((strcmp(get_entry_word(result1), "henn") == 0)
//                && (strcmp(get_entry_word(result2), "hell") == 0)
//                && (strcmp(get_entry_word(result3), "ken") == 0) );

//     destroy_list(&result);
//     destroy_BK_tree(&ix);

//     if (destroy_list(&el) == EC_FAIL)
//         printf("Error! Destroy entry list failed\n");

//     /***************** Test lookup with another tree (assignment's example) *****************/
//     /* add every word mentioned in the example to an entry list */
//     entry e = NULL;
//     LinkedList entryList; create_list(&entryList, NULL);
//     create_entry("melt", &e); add_item(entryList, e);
//     create_entry("small", &e); add_item(entryList, e);
//     create_entry("fell", &e); add_item(entryList, e);
//     create_entry("felt", &e); add_item(entryList, e);
//     create_entry("fall", &e); add_item(entryList, e);
//     create_entry("help", &e); add_item(entryList, e);
//     create_entry("hell", &e); add_item(entryList, e);

//     /* build a BK tree */
//     BK_tree index = NULL;
//     build_entry_index(entryList, MT_EDIT_DIST, &index);
// //    print_BK_tree(index);

// //    printf("Find words similar to henn with max distance 2:\n");

//     LinkedList res;
//     lookup_entry_index("henn", index, 2, &res);

//     TEST_ASSERT(get_number_items(res) == 2);
//     entry res1 = pop_item(res), res2 = pop_item(res);
//     TEST_CHECK((strcmp(get_entry_word(res1), "help") == 0)
//                && (strcmp(get_entry_word(res2), "hell") == 0) );

//     destroy_list(&res);
//     destroy_list(&entryList);
//     destroy_BK_tree(&index);

// }

void test_destroy_BK_tree(){
    BK_tree index = create_BK_tree(EditDistance);
    TEST_ASSERT(index != NULL);
    destroy_BK_tree(&index);
    TEST_ASSERT(index == NULL);
}

TEST_LIST = {
        { "test_create_BK_tree", test_create_BK_tree },
        { "test_insert_BK_tree_EditDistance", test_insert_BK_tree_EditDistance },
        { "test_insert_BK_tree_HammingDistance", test_insert_BK_tree_HammingDistance },
        { "destroy BK tree", test_destroy_BK_tree },
        { NULL, NULL }
};
