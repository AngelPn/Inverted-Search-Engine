#include "../../include/core.h"
#include "../EntryList/EntryList.h"
#include "../Entry/Entry.h"

typedef struct treenode* BK_treenode;
typedef struct tree* BK_tree;

ErrorCode build_entry_index(const entry_list* el,MatchType type, BK_tree* ix);
BK_treenode make_treenode(const entry e);
ErrorCode BK_tree_insert(BK_treenode* tree,BK_treenode node);
void print_BK_tree(BK_treenode root);
int compare_words(const char* word1, const char* word2);
ErrorCode destroy_entry_index(BK_tree ix);
ErrorCode destroy_tree(BK_treenode root);

ErrorCode lookup_entry_index(const word* w, BK_tree ix, int threshold, entry_list* result);
