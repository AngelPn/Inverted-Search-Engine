#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "BKTree.h"

struct treenode
{
    int no_child;       /* number of node's children */
    entry item;         /* item is of struct_entry* */
    int cost;           /* distance between the current word and the parent's one */ 
    BK_treenode child;  /* the list with all child nodes of the current node */
    BK_treenode next    /* the next element of parent node's children */;
};

struct tree
{
    BK_treenode root;
    int size; /* the total number of nodes */
};

/* Creates and returns BK_treenode */
BK_treenode make_treenode(const entry e) {
    BK_treenode new_node = (BK_treenode)malloc(sizeof(struct treenode));
    new_node->item = e;
    new_node->next = NULL;
    new_node->child = NULL;
    new_node->cost = 0;
    new_node->no_child = 0;
    return new_node;
}

int compare_words(const char* word1, const char* word2);

/* Inserts new node to BK tree */
ErrorCode BK_tree_insert(BK_treenode* root, BK_treenode new_node) {

    BK_treenode temp = *root;
    if(temp == NULL) { /* If root is empty */
        *root = new_node;
        return EC_SUCCESS;
    }
    else {
        /* edit distance function */
        int dist = compare_words(get_entry_word((*root)->item), get_entry_word(new_node->item));
        /* Rejecting duplicate words */
        if (dist == 0)
            return EC_FAIL;
        BK_treenode temp = (*root)->child;
        BK_treenode prev = (*root);
        /* if root doesn't have any children */
        if (temp == NULL) {
            (*root)->child = new_node;
            new_node->cost = dist;
            (*root)->no_child++;
            return EC_SUCCESS;
        } else {
            /*  Looping the root's children while holding a pointer to the previous node,
                in case we find a node with bigger distance and we must insert the new node in the previous position 
            */
            while (temp != NULL) {
                if (dist > temp->cost) {
                    prev = temp;
                    temp = temp->next;
                    continue;
                } else if (dist < temp->cost) {
                    /* inserting in the previous position */
                    new_node->next = temp;
                    /* If temp is the first child of our root then we make new_node the new first child */
                    if(prev == (*root))
                        prev->child = new_node;
                    else
                        prev->next = new_node;
                    new_node->cost = dist;
                    (*root)->no_child++;
                    return EC_SUCCESS;
                } else { /* Root already has a child with the same cost -> Insert the new node in the child's childer */              
                    BK_tree_insert(&temp, new_node);
                    return EC_SUCCESS;
                }
            }
            /* If we reached the end of the loop it means new_node must be placed in the last position of the list */
            new_node->cost = dist;
            prev->next = new_node;
            (*root)->no_child++;
            return EC_SUCCESS;
        }
    }
}

ErrorCode build_entry_index(const entry_list el, MatchType type, BK_tree* ix) {

    if (((*ix) = (BK_tree)malloc(sizeof(struct tree))) == NULL)
        return EC_FAIL;
    (*ix)->size = get_number_entries(el);
    (*ix)->root = NULL;

    /* Insert all entry_list's items to the tree iteratively */
    ListNode current = get_first_node(el);
    for(int i = 0; i < (*ix)->size; i++) {
        if (BK_tree_insert(&((*ix)->root), make_treenode(get_node_item(current))) == EC_FAIL)
            return EC_FAIL;
        current = get_next_node(current);
    }
    return EC_SUCCESS;
}

ErrorCode lookup_entry_index(const char* w, BK_tree ix, int threshold, entry_list* result) {
    entry_list candidate_words = NULL; /* actually keeps treenodes so we can get their children and cost */
    if (create_entry_list(&candidate_words, NULL) == EC_FAIL) {
        printf("Error! Create entry list failed\n");
        return EC_FAIL;
    }

    entry_list found_words = NULL; /* this list is the result and doesn't need the whole node's info */
    if (create_entry_list(&found_words, NULL) == EC_FAIL) {
        printf("Error! Create entry list failed\n");
        return EC_FAIL;
    }

    /* Step 1: Add root to candidate words */
    add_entry(candidate_words, ix->root);

    BK_treenode candidate = NULL;
    while ((candidate = pop_entry(candidate_words)) != NULL){

        /* Step 2: Pop a word from candidate words' list and find the distance between this and query's word */
//        printf("candidate %s cost %d\n", get_entry_word(candidate->item), candidate->cost);
        int dist = compare_words(get_entry_word(candidate->item), w);
        if (dist <= threshold) { /* if distance is smaller than the threshold add word to found words */
            add_entry(found_words, candidate->item);
        }

        /* Step 3: Add to candidate words list all children of this node that have distance from parent node in (d-n, d+n) */
        BK_treenode current = candidate->child;
        while (current!=NULL) {
//            printf("diasthma [%d,%d] Current cost %d\n", dist - threshold, dist + threshold, current->cost);
            if (current->cost >= dist - threshold && current->cost <= dist + threshold) {
                add_entry(candidate_words, current);
            }
            current = current->next;
        }

    }
    *result = found_words;
    destroy_entry_list(candidate_words);
    return EC_SUCCESS;
}

void print_BK_tree_helper(BK_treenode root) {
    printf("%s-%d\n", get_entry_word(root->item),root->no_child);
    if(root->child)
        print_BK_tree_helper(root->child);
    if(root->next)
        print_BK_tree_helper(root->next);
    return;
}

void print_BK_tree(BK_tree tree) {
    printf("Printing BK tree:\n");
    print_BK_tree_helper(tree->root);
}

void BK_tree_toString_helper(BK_treenode root, char *string) {
    sprintf(string, "%s-%d\n", get_entry_word(root->item),root->no_child);
    if(root->child)
        BK_tree_toString_helper(root->child, string+strlen(string));
    if(root->next)
        BK_tree_toString_helper(root->next, string+strlen(string));
}

void BK_tree_toString(BK_tree tree, char *string) {
    BK_tree_toString_helper(tree->root, string);
}

/* Recursive delete function for the tree nodes */
ErrorCode destroy_tree(BK_treenode root) {
    if(root->child)
        destroy_tree(root->child);
    if(root->next)
        destroy_tree(root->next);
    free(root);
    root = NULL;
    return EC_SUCCESS;
}

/* Delete function that deletes the tree */
ErrorCode destroy_entry_index(BK_tree* ix) {
    destroy_tree((*ix)->root);
    free(*ix);
    *ix = NULL;
    return EC_SUCCESS;
}

/**************************************************************/

int compare_words(const char* word1, const char* word2) {

    int distance = 0;

    /* points to current character */
    const char *curr1 = word1, *curr2 = word2;

    while (*curr1 && *curr2){
        if (*curr1 != *curr2){
            distance++;
        }
        curr1++;
        curr2++;
    }

    while (*curr1){
        distance++;
        curr1++;
    }

    while (*curr2){
        distance++;
        curr2++;
    }
    return distance;
}
