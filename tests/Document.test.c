#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "core.h"
#include "acutest.h"
#include "Document.h"

void test_create_document(void) {
    Document d = create_document(1);
    TEST_ASSERT(d != NULL);
    destroy_document(d);
}

void test_deduplication(void) {
    Document d = create_document(1);
    char* test = "hello hello world world this is a a project project";
    char* answer = "hello world this is a project";
    char* dedupl = deduplicate_doc_str(d,test);
    int result = strcmp(dedupl,answer);
    TEST_ASSERT(result == 0);
    destroy_document(d);
}

TEST_LIST = {
        { "test_create_document", test_create_document },
        { "test_deduplication", test_deduplication },
        { NULL, NULL }
};

