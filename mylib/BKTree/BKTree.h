#ifndef BK_TREE_H
#define BK_TREE_H

#include "core.h"
#include "EntryList.h"
#include "Entry.h"

typedef struct treenode* BK_treenode;
typedef struct tree* BK_tree;

/* Builds BK tree from given entry list */
ErrorCode build_entry_index(const entry_list el, MatchType type, BK_tree* ix);

/* Returns an entry list that matches given word */
ErrorCode lookup_entry_index(const char* w, BK_tree ix, int threshold, entry_list* result);

/* Prints BK tree: (word of entry) - (number of children) */
void print_BK_tree(BK_tree tree);

/* Items of BK tree as string */
void BK_tree_toString(BK_tree tree, char *string);

/* Deallocates of memory of BK tree */
ErrorCode destroy_entry_index(BK_tree* ix);

BK_treenode make_treenode(const entry e);
ErrorCode BK_tree_insert(BK_tree ix, BK_treenode* root, BK_treenode new_node);
int HammingDistance(const char* a, const char* b);
int EditDistance(const char* a, const char* b);
#endif
