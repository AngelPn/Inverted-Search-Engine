#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "core.h"
#include "./EntryList/EntryList.h"
#include "./BKTree/BKTree.h"
#include "./Entry/Entry.h"
#include "./HammingTree/HammingTree.h"

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
}

