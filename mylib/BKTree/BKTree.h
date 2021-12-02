#ifndef BK_TREE_H
#define BK_TREE_H

#include "core.h"
#include "EntryList.h"
#include "Entry.h"

typedef struct treenode* BK_treenode;
typedef struct tree* BK_tree;

/* Returns an empty BK tree */
BK_tree initialize_BK_tree(int (*distance_func)(const char* word1, const char* word2));

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

BK_treenode get_root(BK_tree tree);
/* returns a double pointer to the root (used together with BK_tree_insert that needs double pointer) */
BK_treenode* get_root_double_p(BK_tree tree);
BK_treenode make_treenode(const entry e);
ErrorCode BK_tree_insert(BK_tree ix, BK_treenode* root, BK_treenode new_node);
int hammingDistance(const char* a, const char* b);
int editDistance(const char* a, const char* b);
#endif
