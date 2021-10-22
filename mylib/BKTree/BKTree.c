#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int compare_words(const char* word1, const char* word2);

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
