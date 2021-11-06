#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "Entry.h"

struct entry_struct
{
    word *w;
    void *payload;
};

ErrorCode create_entry(const word *w, entry *e){

    if (((*e) = (entry)malloc(sizeof(struct entry_struct))) == NULL)
        return EC_FAIL;

    if (((*e)->w = (char *)malloc(sizeof(char)*strlen(w)+1)) == NULL)
        return EC_FAIL;
    strcpy((*e)->w, w);

    (*e)->payload = NULL;
    
    return EC_SUCCESS;
}

word *get_entry_word(entry e){
    return e->w;
}

void print_entry_word(void *e){
    entry ce = e;
    printf("%s\n", ce->w);
}

ErrorCode destroy_entry(void *e){
    entry ce = e;
    free(ce->w);
    free(ce);
    ce = NULL;
    return EC_SUCCESS;
}
