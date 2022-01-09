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
    for(int i=0 ; i<MAX_QUERY_WORDS ; i++){
        found(q,i);
        TEST_ASSERT(get_index_found(q,i) == 1);
    }
    destroy_query(q);
}

TEST_LIST = {
        { "test_create_query", test_create_query },
        { "test_get_queryID", test_get_queryID },
        { "test_found", test_found },
        { NULL, NULL }
};

