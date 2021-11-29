#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "HammingTree.h"

struct hamming_tree {
    BK_tree TreeArray[28];
};

//The same BK_tree struct from BKTree.c
struct tree
{
    BK_treenode root;
    int size; /* the total number of nodes */
    int (*distance_function)(const char* word1, const char* word2);
};

HammingTree initialize_hamming_tree() {
    HammingTree res = malloc(sizeof(struct hamming_tree));
    for(int i=0 ; i<28 ; i++){
        res->TreeArray[i] = (BK_tree)malloc(sizeof(struct tree));
        res->TreeArray[i]->root = NULL;
        res->TreeArray[i]->size = 0;
        res->TreeArray[i]->distance_function = HammingDistance;
    }
    return res;
}

ErrorCode hamming_tree_insert(HammingTree ht, char* w) {
    entry e;
    ErrorCode er;
    if (create_entry(w, &e) == EC_FAIL)
        return EC_FAIL;
    BK_treenode treend = make_treenode(e);
    er = BK_tree_insert(ht->TreeArray[strlen(w) - 4], &(ht->TreeArray[strlen(w) - 4]->root), treend);
    if (er == EC_FAIL){
        destroy_entry((void**)&e);
        free(treend);
    }
    return EC_SUCCESS;
}

ErrorCode destroy_hamming_tree(HammingTree ix) {
    for(int i=0 ; i<28 ; i++) {
        destroy_entry_index(&(ix->TreeArray[i]));
    }
    free(ix);
    return EC_SUCCESS;
}

void print_hamming_tree(HammingTree ix) {
    for(int i=0 ; i<28 ; i++) {
        printf("Words of length %d:\n",i+4);
        if(ix->TreeArray[i]->root!=NULL)
            print_BK_tree(ix->TreeArray[i]);
        else
            printf("(null)\n");
    }
}
