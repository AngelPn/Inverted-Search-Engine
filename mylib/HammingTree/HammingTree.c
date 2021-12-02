#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "HammingTree.h"
#include "../DistanceFunctions.h"
#include "../BKTree/BKTree.h"

struct hamming_tree {
    BK_tree TreeArray[28];
};

HammingTree initialize_hamming_tree() {
    HammingTree res = malloc(sizeof(struct hamming_tree));
    for(int i=0 ; i<28 ; i++){
        res->TreeArray[i] = initialize_BK_tree(hammingDistance);
    }
    return res;
}

ErrorCode hamming_tree_insert(HammingTree ht, char* w) {
    entry e;
    ErrorCode er;
    if (create_entry(w, &e) == EC_FAIL)
        return EC_FAIL;
    BK_treenode treend = make_treenode(e);
    //because the trees array are from index 0 to 27 we substract 4 from the length
    er = BK_tree_insert(ht->TreeArray[strlen(w) - 4], get_root_double_p(ht->TreeArray[strlen(w) - 4]), treend);
    if (er == EC_FAIL){
        destroy_entry((void**)&e);
        free(treend);
        return EC_FAIL;
    }
    return EC_SUCCESS;
}

ErrorCode destroy_hamming_tree(HammingTree ix) {
    for(int i = 0 ; i < 28 ; i++) {
        //using the recursive BK_Tree destroy function for each of the 27 trees
        destroy_entry_index(&(ix->TreeArray[i]));
    }
    free(ix);
    return EC_SUCCESS;
}

void print_hamming_tree(HammingTree ix) {
    for(int i=0 ; i<28 ; i++) {
        printf("Words of length %d:\n",i+4);
        if(get_root(ix->TreeArray[i]) != NULL)
            print_BK_tree(ix->TreeArray[i]);
        else
            printf("(null)\n");
    }
}
