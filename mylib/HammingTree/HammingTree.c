#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "HammingTree.h"
#include "BKTree.h"

struct hamming_tree {
    BK_tree TreeArray[28];
};

HammingTree create_HammingTree(DistFunc distance) {
    HammingTree res = malloc(sizeof(struct hamming_tree));
    for(int i=0 ; i<28 ; i++){
        res->TreeArray[i] = create_BK_tree(distance);
    }
    return res;
}

entry insert_HammingTree(HammingTree ht, char* w) {
    //because the trees array are from index 0 to 27 we substract 4 from the length
    return BK_tree_insert(ht->TreeArray[strlen(w) - 4], get_root_double_p(ht->TreeArray[strlen(w) - 4]), w);
}

ErrorCode destroy_HammingTree(HammingTree ix) {
    for(int i = 0 ; i < 28 ; i++) {
        //using the recursive BK_Tree destroy function for each of the 27 trees
        destroy_BK_tree(&(ix->TreeArray[i]));
    }
    free(ix);
    return EC_SUCCESS;
}

void print_HammingTree(HammingTree ix) {
    for(int i=0 ; i<28 ; i++) {
        printf("Words of length %d:\n",i+4);
        if(get_root(ix->TreeArray[i]) != NULL)
            print_BK_tree(ix->TreeArray[i]);
        else
            printf("(null)\n");
    }
}
