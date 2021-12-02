#ifndef HAMMINGTREE_H
#define HAMMINGTREE_H

#include "core.h"
#include "../BKTree/BKTree.h"
#include "../Entry/Entry.h"

typedef struct hamming_tree *HammingTree;

HammingTree initialize_hamming_tree();
ErrorCode hamming_tree_insert(HammingTree ht, char* w);
ErrorCode destroy_hamming_tree(HammingTree ix);
void print_hamming_tree(HammingTree ix);

#endif
