#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "core.h"
#include "acutest.h"
#include "BKTree.h"
#include "HammingTree.h"

//All HammingTree functions except create call the BKtree functions to work so if they pass the test we are good

void test_create_HammingTree(void) {
    HammingTree H_T = create_HammingTree(HammingDistance);
    TEST_ASSERT(H_T != NULL);
    for(int i = 0 ; i < 28 ; i++) {
        TEST_ASSERT(H_T->TreeArray[i] != NULL);
    }
    destroy_HammingTree(H_T);
}

TEST_LIST = {
        { "test_create_HammingTree", test_create_HammingTree },
        { NULL, NULL }
};

