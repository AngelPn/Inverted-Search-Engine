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

    //add every word mentioned in the example to an entry list
    insert_BK_tree(index, "hell");
    insert_BK_tree(index, "help");
    insert_BK_tree(index, "fall");
    insert_BK_tree(index, "felt");
    insert_BK_tree(index, "fell");
    insert_BK_tree(index, "small");
    insert_BK_tree(index, "melt");

    char string[] = "hell-3\nhelp-1\nfell-0\nfall-2\nfelt-0\nmelt-0\nsmall-0\n";  //this is the expected tree
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

    //add every word mentioned in the example to an entry list
    insert_BK_tree(index, "book");
    insert_BK_tree(index, "look");
    insert_BK_tree(index, "lock");
    insert_BK_tree(index, "dock");
    insert_BK_tree(index, "duck");
    insert_BK_tree(index, "move");

    char string[] = "book-3\nlook-0\nlock-1\ndock-0\nduck-1\nmove-0\n";  //this is the expected tree
    char* string_res = malloc(strlen(string)+1);
    BK_tree_toString(index, string_res);
    TEST_CHECK(strcmp(string, string_res) == 0);
    TEST_MSG("expected:\n%s", string);
    TEST_MSG("produced:\n%s", string_res);

    free(string_res);
    destroy_BK_tree(&index);
}

void test_lookup_entry_index(){
    /***************** Test lookup with another tree (assignment's example) ****************/
    //add every word mentioned in the example to an entry list
    BK_tree index = create_BK_tree(HammingDistance);
    TEST_ASSERT(index != NULL);

    //add every word mentioned in the example to an entry list
    insert_BK_tree(index, "hell");
    insert_BK_tree(index, "help");
    insert_BK_tree(index, "fall");
    insert_BK_tree(index, "felt");
    insert_BK_tree(index, "fell");
    insert_BK_tree(index, "small");
    insert_BK_tree(index, "melt");

    //print_BK_tree(index);

    //printf("Find words similar to henn with max distance 2:\n");

     LinkedList res;
     lookup_entry_index("henn", index, 2, &res);

     TEST_ASSERT(get_number_items(res) == 2);
     entry res1 = pop_item(res), res2 = pop_item(res);
     TEST_CHECK((strcmp(get_entry_word(res1), "help") == 0)
                && (strcmp(get_entry_word(res2), "hell") == 0) );

     destroy_list(&res);
     destroy_BK_tree(&index);

}

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
        { "test_lookup_entry_index", test_lookup_entry_index },
        { "destroy BK tree", test_destroy_BK_tree },
        { NULL, NULL }
};
