#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "core.h"
#include "acutest.h"
#include "Query.h"

void test_create_query(void) {
    Query q = create_query(1);
    TEST_ASSERT(q != NULL);
    destroy_query(q);
}

void test_get_queryID(void) {
    Query q = create_query(1);
    int n = get_queryID(q);
    TEST_ASSERT(n == 1);
    destroy_query(q);
}

void test_found(void) {
    Query q = create_query(1);
    set_size(q, MAX_QUERY_WORDS);
    bool temp = 0;
    for(int i=0 ; i<MAX_QUERY_WORDS ; i++){
        found(q,i,&temp);
        TEST_ASSERT(get_index_found(q,i) == 1);
    }
    destroy_query(q);
}

void test_reset_found(void) {
    Query q = create_query(1);
    set_size(q, MAX_QUERY_WORDS);
    bool temp = 0;
    for(int i=0 ; i < MAX_QUERY_WORDS ; i++){
        found(q,i,&temp);
    }
    reset_found(q);
    for(int j=0 ; j < MAX_QUERY_WORDS ; j++){
        temp = get_index_found(q,j);
        TEST_ASSERT(temp == 0);
    }
    destroy_query(q);
}

TEST_LIST = {
        { "test_create_query", test_create_query },
        { "test_get_queryID", test_get_queryID },
        { "test_found", test_found },
        { "test_reset_found", test_reset_found },
        { NULL, NULL }
};

