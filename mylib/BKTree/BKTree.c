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
    DistFunc distance;
};

BK_tree create_BK_tree(DistFunc distance) {
    BK_tree new_tree = (BK_tree)malloc(sizeof(struct tree));
    new_tree->size = 0;
    new_tree->root = NULL;
    new_tree->distance = distance;
    return new_tree;
}

/* Creates and returns BK_treenode */
BK_treenode new_treenode(entry e) {
    BK_treenode new_node = (BK_treenode)malloc(sizeof(struct treenode));
    new_node->item = e;
    new_node->next = NULL;
    new_node->child = NULL;
    new_node->cost = 0;
    new_node->no_child = 0;
    return new_node;
}

entry insert_node(BK_tree ix, BK_treenode node, char *b) {
    char *a = get_entry_word((node)->item);
    int dist = ix->distance(a, strlen(a), b, strlen(b));
    /* Rejecting duplicate words */
    if (dist == 0)
        return (node)->item;
    BK_treenode temp = (node)->child;
    BK_treenode prev = (node);
    /* if node doesn't have any children */
    if (temp == NULL) {
        entry e; create_entry(b,&e);
        BK_treenode new_node = new_treenode(e);
        (node)->child = new_node;
        new_node->cost = dist;
        (node)->no_child++;
        return e;
    } else {
        /*  Looping the node's children while holding a pointer to the previous node,
            in case we find a node with bigger distance and we must insert the new node in the previous position 
        */
        while (temp != NULL) {
            if (dist > temp->cost) {
                prev = temp;
                temp = temp->next;
                continue;
            } else if (dist < temp->cost) {
                entry e; create_entry(b,&e);
                BK_treenode new_node = new_treenode(e);
                /* inserting in the previous position */
                new_node->next = temp;
                /* If temp is the first child of our root then we make new_node the new first child */
                if (prev == (node))
                    prev->child = new_node;
                else
                    prev->next = new_node;
                new_node->cost = dist;
                (node)->no_child++;
                return e;
            } else /* Root already has a child with the same cost -> Insert entry in the child's children */
                return insert_node(ix, temp, b);
        }
        /* If we reached the end of the loop it means new_node must be placed in the last position of the list */
        entry e; create_entry(b,&e);
        BK_treenode new_node = new_treenode(e);
        new_node->cost = dist;
        prev->next = new_node;
        (node)->no_child++;
        return e;
    }
}

entry insert_BK_tree(BK_tree ix, char* b) {
    if (ix->root == NULL) {
        entry e; create_entry(b, &e);
        ix->root = new_treenode(e);
        return e;
    } else {
        return insert_node(ix, ix->root, b);
    }
}

BK_treenode get_root(BK_tree tree){
    return tree->root;
}

entry get_BK_treenode_entry(BK_treenode n){
    return n->item;
}

int get_BK_treenode_cost(BK_treenode n){
    return n->cost;
}

ErrorCode lookup_entry_index(char* w, BK_tree ix, int threshold, LinkedList* result) {
    LinkedList candidate_words = NULL; /* actually keeps tree nodes so we can get their children and cost */
    if (create_list(&candidate_words, NULL) == EC_FAIL) {
        printf("Error! Create entry list failed\n");
        return EC_FAIL;
    }

    LinkedList found_words = NULL; /* this list is the result and doesn't need the whole node's info */
    if (create_list(&found_words, NULL) == EC_FAIL) {
        printf("Error! Create entry list failed\n");
        return EC_FAIL;
    }

    /* Step 1: Add root to candidate words */
    add_item(candidate_words, ix->root);

    BK_treenode candidate = NULL;
    while ((candidate = pop_item(candidate_words)) != NULL){

        /* Step 2: Pop a word from candidate words' list and find the distance between this and query's word */
//        printf("candidate %s cost %d\n", get_entry_word(candidate->item), candidate->cost);
        char *a = get_entry_word(candidate->item);
        char *b = w;
        int dist = ix->distance(a, strlen(a), b, strlen(b));
        if (dist <= threshold) { /* if distance is smaller than the threshold add word to found words */
            add_item(found_words, candidate->item);
        }

        /* Step 3: Add to candidate words list all children of this node that have distance from parent node in (d-n, d+n) */
        BK_treenode current = candidate->child;
        while (current!=NULL) {
//            printf("[%d,%d] Current cost %d\n", dist - threshold, dist + threshold, current->cost);
            if (current->cost >= dist - threshold && current->cost <= dist + threshold) {
                add_item(candidate_words, current);
            }
            current = current->next;
        }

    }
    *result = found_words;
    destroy_list(&candidate_words);
    return EC_SUCCESS;
}

ErrorCode lookup_BKtree(char* w, BK_tree ix, int threshold, HashT* candidate_queries, LinkedList candidates, LinkedList matched_queries) {
    LinkedList candidate_nodes = NULL; /* actually keeps tree nodes so we can get their children and cost */
    if (create_list(&candidate_nodes, NULL) == EC_FAIL) {
        printf("Error! Create entry list failed\n");
        return EC_FAIL;
    }

    /* Step 1: Add root to candidate entries */
    add_item(candidate_nodes, ix->root);

    BK_treenode candidate = NULL;
    while ((candidate = pop_item(candidate_nodes)) != NULL){

        /* Step 2: Pop a word from candidate words' list and find the distance between this and query's word */
//        printf("candidate %s cost %d\n", get_entry_word(candidate->item), candidate->cost);
        char *a = get_entry_word(candidate->item);
        char *b = w;
        int dist = ix->distance(a, strlen(a), b, strlen(b));
        if (dist <= threshold) { /* if distance is smaller than the threshold add word to found words */
            // add_item(found_words, candidate->item);
            update_payload(candidate->item, threshold-1, candidate_queries, candidates, matched_queries);
        }

        /* Step 3: Add to candidate words list all children of this node that have distance from parent node in (d-n, d+n) */
        BK_treenode current = candidate->child;
        while (current!=NULL) {
//            printf("[%d,%d] Current cost %d\n", dist - threshold, dist + threshold, current->cost);
            if (current->cost >= dist - threshold && current->cost <= dist + threshold) {
                add_item(candidate_nodes, current);
            }
            current = current->next;
        }

    }
    destroy_list(&candidate_nodes);
    return EC_SUCCESS;
}

void print_BK_tree_helper(BK_treenode root) {
    printf("%s-%d\n", get_entry_word(root->item), root->no_child);
    if (root->child != NULL)
        print_BK_tree_helper(root->child);
    if (root->next != NULL)
        print_BK_tree_helper(root->next);
    return;
}

void print_BK_tree(BK_tree tree) {
    printf("Printing BK tree:\n");
    if (tree->root != NULL)
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
    if(root->item)
        destroy_entry((void**)&(root->item));
    free(root);
    root = NULL;
    return EC_SUCCESS;
}

/* Delete function that deletes the tree */
ErrorCode destroy_BK_tree(BK_tree* ix) {
    if((*ix)->root)
        destroy_tree((*ix)->root);
    free(*ix);
    *ix = NULL;
    return EC_SUCCESS;
}
