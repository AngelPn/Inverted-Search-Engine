#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "Entry.h"
#include "LinkedList.h"

struct entry_struct
{
    char *w;
    LinkedList payload[3]; /* index indicates match_dist of query */
};

struct info_struct {
    Query q;
    int index;
    ListNode info_node;
    LinkedList head;
};

ErrorCode create_entry(const char *w, entry *e){

    if (((*e) = (entry)malloc(sizeof(struct entry_struct))) == NULL)
        return EC_FAIL;

    if (((*e)->w = (char *)malloc(sizeof(char)*strlen(w)+1)) == NULL)
        return EC_FAIL;
    strcpy((*e)->w, w);

    for (int i = 0; i < 3; i++)
        create_list(&((*e)->payload[i]), destroy_entry);
    
    return EC_SUCCESS;
}

info create_info(Query q, int i, LinkedList head) {
    info inf = malloc(sizeof(struct info_struct));
    inf->q = q;
    inf->index = i;
    inf->head = head;
    return inf;
}

ErrorCode update_entry_payload(entry e, unsigned int match_dist, Query q, int index){
    ListNode node = NULL;
    info inf = create_info(q, index, e->payload[match_dist]);
    if ((node = push_item(e->payload[match_dist], inf)) == NULL) {
        return EC_FAIL;
    } else {
        inf->info_node = node;
        set_info_words(q, index, inf);
        return EC_SUCCESS;
    }
}

void remove_query_info(info inf) {
    list_remove(inf->head, inf->info_node);
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
    for(int i=0 ; i<3 ; i++) {
        if(ce->payload[i]) {
            destroy_list(&(ce->payload[i]));
            ce->payload[i] = NULL;
        }
    }
    free(ce->w);
    free(ce);
    ce = NULL;
    *e = NULL;
    return EC_SUCCESS;
}
