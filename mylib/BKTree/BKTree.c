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
    int (*distance_function)(const char* word1, const char* word2);
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

/* distance functions */
int compare_words(const char* word1, const char* word2);
int HammingDistance(const char* a, const char* b);
int EditDistance(const char* a, const char* b);

/* Inserts new node to BK tree */
ErrorCode BK_tree_insert(BK_tree ix, BK_treenode* root, BK_treenode new_node) {

    BK_treenode temp = *root;
    if(temp == NULL) { /* If root is empty */
        *root = new_node;
        return EC_SUCCESS;
    }
    else {
        /* edit distance function */
        int dist = ix->distance_function(get_entry_word((*root)->item), get_entry_word(new_node->item));
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
                } else { /* Root already has a child with the same cost -> Insert the new node in the child's children */
                    BK_tree_insert(ix, &temp, new_node);
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

    if (type == MT_EDIT_DIST) {
        (*ix)->distance_function = EditDistance;
    }
    else if (type == MT_HAMMING_DIST) {
        (*ix)->distance_function = HammingDistance;
    }

    /* Insert all entry_list's items to the tree iteratively */
    ListNode current = get_first_node(el);
    for(int i = 0; i < (*ix)->size; i++) {
        if (BK_tree_insert(*ix, &((*ix)->root), make_treenode(get_node_item(current))) == EC_FAIL)
            return EC_FAIL;
        current = get_next_node(current);
    }
    return EC_SUCCESS;
}

ErrorCode lookup_entry_index(const char* w, BK_tree ix, int threshold, entry_list* result) {
    entry_list candidate_words = NULL; /* actually keeps tree nodes so we can get their children and cost */
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
        int dist = ix->distance_function(get_entry_word(candidate->item), w);
        if (dist <= threshold) { /* if distance is smaller than the threshold add word to found words */
            add_entry(found_words, candidate->item);
        }

        /* Step 3: Add to candidate words list all children of this node that have distance from parent node in (d-n, d+n) */
        BK_treenode current = candidate->child;
        while (current!=NULL) {
//            printf("[%d,%d] Current cost %d\n", dist - threshold, dist + threshold, current->cost);
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
ErrorCode destroy_entry_index(BK_tree ix) {
    destroy_tree(ix->root);
    free(ix);
    ix = NULL;
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

// Computes edit distance between a null-terminated string "a" with length "na"
//  and a null-terminated string "b" with length "nb"
int EditDistance(const char* a, const char* b)
{
    int na = strlen(a);
    int nb = strlen(b);
    int oo=0x7FFFFFFF;

    static int T[2][MAX_WORD_LENGTH+1];

    int ia, ib;

    int cur=0;

    for(ib=0;ib<=nb;ib++)
        T[cur][ib]=ib;

    cur=1-cur;

    for(ia=1;ia<=na;ia++)
    {
        for(ib=0;ib<=nb;ib++)
            T[cur][ib]=oo;

        int ib_st=0;
        int ib_en=nb;

        if(ib_st==0)
        {
            ib=0;
            T[cur][ib]=ia;
            ib_st++;
        }

        for(ib=ib_st;ib<=ib_en;ib++)
        {
            int ret=oo;

            int d1=T[1-cur][ib]+1;
            int d2=T[cur][ib-1]+1;
            int d3=T[1-cur][ib-1]; if(a[ia-1]!=b[ib-1]) d3++;

            if(d1<ret) ret=d1;
            if(d2<ret) ret=d2;
            if(d3<ret) ret=d3;

            T[cur][ib]=ret;
        }

        cur=1-cur;
    }

    int ret=T[1-cur][nb];

    return ret;
}

// Computes Hamming distance between a null-terminated string "a" with length "na"
//  and a null-terminated string "b" with length "nb"
int HammingDistance(const char* a, const char* b)
{
    int na = strlen(a);
    int nb = strlen(b);
    int j, oo=0x7FFFFFFF;
    if(na!=nb) return oo;

    unsigned int num_mismatches=0;
    for(j=0;j<na;j++) if(a[j]!=b[j]) num_mismatches++;

    return num_mismatches;
}