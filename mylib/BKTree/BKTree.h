#ifndef BK_TREE_H
#define BK_TREE_H

#include "core.h"
#include "LinkedList.h"
#include "Entry.h"

typedef struct treenode* BK_treenode;
typedef struct tree* BK_tree;

/* Returns an empty BK tree */
BK_tree create_BK_tree(DistFunc distance);

entry BK_tree_insert(BK_tree ix, BK_treenode* root, char* b);

/* Returns an entry list that matches given word */
ErrorCode lookup_entry_index(char* w, BK_tree ix, int threshold, LinkedList* result);

/* Prints BK tree: (word of entry) - (number of children) */
void print_BK_tree(BK_tree tree);

/* Items of BK tree as string */
void BK_tree_toString(BK_tree tree, char *string);

/* Deallocates of memory of BK tree */
ErrorCode destroy_BK_tree(BK_tree* ix);

BK_treenode get_root(BK_tree tree);
/* returns a double pointer to the root (used together with BK_tree_insert that needs double pointer) */
BK_treenode* get_root_double_p(BK_tree tree);
int get_BK_treenode_cost(BK_treenode n);
entry get_BK_treenode_entry(BK_treenode n);

#endif
