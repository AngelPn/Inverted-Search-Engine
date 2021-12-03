#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "core.h"
#include "BKTree.h"
#include "Entry.h"
#include "HammingTree.h"
#include "HashTable.h"

//We insert each word one by one in a big hashtable and at the same time we check if it already exists in the hashtable
//if it already exists it means it is a duplicate word so it is not in the result string that it is returned
char* deduplication(char* text){
    HashT* HT = HashT_init(1000,NULL);
    const char space[2] = " ";
    //allocating new char array because strtok() cant be applied on string literals
    char* new_txt= malloc(strlen(text)+1);
    strcpy(new_txt,text);
    //getting the first word from the text
    char* token = strtok(new_txt,space);
    //this is the string that we will write the text word by word without the duplicates
    char* new_str = malloc(strlen(text)+1);
    new_str[0] = '\0';
    while(token != NULL){
        //we insert each word in the hashtable and if it already exists then it is not written in our result string
        if(HashT_insert(HT,token,NULL)) {
            strcat(new_str, space);
            strcat(new_str, token);
        }
        //getting the next character
        token = strtok(NULL,space);
        printf("token: %s\n",token);
    }
    HashT_delete(HT);
    free(new_txt);
    //NOTE: the returned string must be freed
    return new_str;
}

int main(void){
    HammingTree H = create_HammingTree(HammingDistance);
    insert_HammingTree(H,"hell");
    insert_HammingTree(H,"henn");
    insert_HammingTree(H,"help");
    insert_HammingTree(H,"felt");
    insert_HammingTree(H,"athens");
    insert_HammingTree(H,"thessaloniki");
    insert_HammingTree(H,"whatever");
    insert_HammingTree(H,"grapes");
    insert_HammingTree(H,"honey");
    insert_HammingTree(H,"informatics");
    insert_HammingTree(H,"greece");
    insert_HammingTree(H,"whatever");
    print_HammingTree(H);
    destroy_HammingTree(H);

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

    char* test_0 = "Hello                test";
    printf("\nBefore deduplication: %s\n",test_0);
    char* test = deduplication(test_0);
    printf("After deduplication: %s\n",test);
    free(test);
}

