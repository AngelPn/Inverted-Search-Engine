#ifndef HAMMINGTREE_H
#define HAMMINGTREE_H

#include "core.h"
#include "BKTree.h"
#include "Entry.h"

typedef struct hamming_tree *HammingTree;

/* Returns an empty HammingTree */
HammingTree create_HammingTree(DistFunc distance);

BK_tree get_specific_BKTree(HammingTree H_T,int index);

/* Inserts word as entry to HammingTree and returns the entry */
entry insert_HammingTree(HammingTree ht, char* w);

/* Search in HammingTree given word */
ErrorCode lookup_HammingTree(HammingTree ix, char* word, int threshold, LinkedList candidate_queries, LinkedList matched_queries);

/* Prints HammingTree: BKtree index -> (word of entry) - (number of children) */
void print_HammingTree(HammingTree ix);

/* Deallocates of memory of HammingTree */
ErrorCode destroy_HammingTree(HammingTree ix);


#endif
