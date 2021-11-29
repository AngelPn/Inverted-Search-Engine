#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "core.h"
#include "./EntryList/EntryList.h"
#include "./BKTree/BKTree.h"
#include "./Entry/Entry.h"
#include "./HammingTree/HammingTree.h"

#include "./HashTable/HashTable.h"

int main(void){
    HammingTree H = initialize_hamming_tree();
    hamming_tree_insert(H,"hell");
    hamming_tree_insert(H,"henn");
    hamming_tree_insert(H,"help");
    hamming_tree_insert(H,"felt");
    hamming_tree_insert(H,"athens");
    hamming_tree_insert(H,"thessaloniki");
    hamming_tree_insert(H,"whatever");
    hamming_tree_insert(H,"grapes");
    hamming_tree_insert(H,"honey");
    hamming_tree_insert(H,"informatics");
    hamming_tree_insert(H,"greece");
    hamming_tree_insert(H,"whatever");
    print_hamming_tree(H);
    destroy_hamming_tree(H);

    HashT* exact_matching = HashT_init(10, NULL);
    HashT_insert(exact_matching,"hell", NULL); /* Instead of NULL we will insert the payload */
    HashT_insert(exact_matching,"henn", NULL);
    HashT_insert(exact_matching,"help", NULL);
    HashT_insert(exact_matching,"felt", NULL);
    HashT_insert(exact_matching,"athens", NULL);
    HashT_insert(exact_matching,"thessaloniki", NULL);
    HashT_insert(exact_matching,"whatever", NULL);
    HashT_insert(exact_matching,"grapes", NULL);
    HashT_insert(exact_matching,"honey", NULL);
    HashT_insert(exact_matching,"informatics", NULL);
    HashT_insert(exact_matching,"greece", NULL);
    HashT_insert(exact_matching,"whatever", NULL);
    HashT_print(exact_matching, NULL);
    HashT_stats(exact_matching);

    printf("athens: %d\n", HashT_exists(exact_matching, "athens"));
    printf("whatever: %d\n", HashT_exists(exact_matching, "whatever"));
    printf("hello: %d\n", HashT_exists(exact_matching, "hello"));

    HashT_delete(exact_matching);
}

