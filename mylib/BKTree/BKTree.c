#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "BKTree.h"


struct treenode{
    char* word;
    /* cost is the distance between the current word and the parent's one */
    int cost;
    /*Child is the list with all child nodes of the current node*/
    BK_treenode child;
    /* Next is the next element of parent node's children*/
    BK_treenode next;
};

struct tree{
    BK_treenode root;
    /*Size is the total number of nodes*/
    int size;
};

int compare_words(const char* word1, const char* word2){
    /* using edit distance */

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
