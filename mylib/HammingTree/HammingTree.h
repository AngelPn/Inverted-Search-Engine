#ifndef HAMMINGTREE_H
#define HAMMINGTREE_H

#include "core.h"
#include "BKTree.h"
#include "Entry.h"

struct hamming_tree {
    BK_tree TreeArray[28];
};

typedef struct hamming_tree *HammingTree;

HammingTree create_HammingTree(DistFunc distance);
entry insert_HammingTree(HammingTree ht, char* w);
ErrorCode lookup_HammingTree(HammingTree ix, char* word, int threshold, LinkedList candidate_queries, LinkedList matched_queries);
ErrorCode destroy_HammingTree(HammingTree ix);
void print_HammingTree(HammingTree ix);

#endif
