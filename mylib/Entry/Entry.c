#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "Entry.h"
#include "LinkedList.h"
#include "Query.h"

struct entry_struct
{
    char *w;
    LinkedList payload[3]; /* index indicates match_dist of query */
};

struct info_struct {
    Query q;
    int index;
};

ErrorCode create_entry(const char *w, entry *e){

    if (((*e) = (entry)malloc(sizeof(struct entry_struct))) == NULL)
        return EC_FAIL;

    if (((*e)->w = (char *)malloc(sizeof(char)*strlen(w)+1)) == NULL)
        return EC_FAIL;
    strcpy((*e)->w, w);

    for (int i = 0; i < 3; i++)
        (*e)->payload[i] = NULL;
    
    return EC_SUCCESS;
}

ErrorCode update_entry_payload(entry *e,info n){
    // return add_entry((*e)->pl,n);
    return EC_SUCCESS;
}

char *get_entry_word(entry e){
    return e->w;
}

void print_entry_word(void *e){
    entry ce = e;
    printf("%s\n", ce->w);
}

ErrorCode destroy_entry(void **e) {
    entry ce = *e;
    free(ce->w);
    free(ce);
    ce = NULL;
    *e = NULL;
    return EC_SUCCESS;
}
