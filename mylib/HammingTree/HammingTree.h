#ifndef HAMMINGTREE_H
#define HAMMINGTREE_H

#include "core.h"
#include "BKTree.h"
#include "Entry.h"

typedef struct hamming_tree *HammingTree;

HammingTree create_HammingTree(DistFunc distance);
entry insert_HammingTree(HammingTree ht, char* w);
ErrorCode lookup_HammingTree(HammingTree ix, char* word, int threshold, HashT* candidate_queries, LinkedList candidates, LinkedList matched_queries);
ErrorCode destroy_HammingTree(HammingTree ix);
void print_HammingTree(HammingTree ix);

#endif
