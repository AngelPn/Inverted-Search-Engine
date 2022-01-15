#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

#include "Entry.h"
#include "Index.h"

struct entry_struct
{
    char *w;
    LinkedList payload[3]; /* payload's index indicates match_dist of query */
};

/* ------------------- Item of entry's payload's list ----------------------- */
typedef struct info_struct *info;

struct info_struct {
    Query q;    /* Point to query */
    int index;  /* Keep the order of the word in query's words */
};

info create_info(Query q, int index) {
    info inf = malloc(sizeof(struct info_struct));
    inf->q = q;
    inf->index = index;
    return inf;
}

ErrorCode destroy_info(void** inff) {
    info inf = *inff;
    free(inf);
    inf = NULL;
    return EC_SUCCESS;
}
/* -------------------------------------------------------------------------- */

ErrorCode create_entry(const char *w, entry *e){

    if (((*e) = (entry)malloc(sizeof(struct entry_struct))) == NULL)
        return EC_FAIL;

    if (((*e)->w = (char *)malloc(sizeof(char)*strlen(w)+1)) == NULL)
        return EC_FAIL;
    strcpy((*e)->w, w);

    /* Payload is a list of info struct */
    for (int i = 0; i < 3; i++)
        create_list(&((*e)->payload[i]), destroy_info);
    
    return EC_SUCCESS;
}

char *get_entry_word(entry e){
    return e->w;
}

void print_entry_word(void *e){
    entry ce = e;
    printf("%s\n", ce->w);
}

ErrorCode insert_info_payload(entry e, unsigned int match_dist, Query q, int index){
    /* Create info and push it to entry's payload's list */
    ListNode node = NULL;
    if ((node = push_item(e->payload[match_dist], create_info(q, index))) == NULL) {
        return EC_FAIL;
    } else {
        /* Get list and node and insert it to query */
        set_info_location(q, index, e->payload[match_dist], node);
        return EC_SUCCESS;
    }
}

ErrorCode update_payload(entry e, int threshold, LinkedList candidate_queries, LinkedList matched_queries){
    
    
    LinkedList l = e->payload[threshold];
    
    bool found_first_time = false;
    ListNode node = get_first_node(l);
    for (int i = 0; i < get_number_items(l); i++) {
        info f = get_node_item(node);
        /* If all words of query match to document's words, add query to document's matched_queries list */
        pthread_mutex_lock(&(job_scheduler.candidate_queries_mtx));

        // Query q = NULL
        // if ((q = HashT_get(candidate_queries, f->q->queryID)) == NULL) {
        //    q = create_query();
        //    HashT_insert(candidate_queries, q);            
        // }
        // found(q, f->index, &found_first_time);
        
        if (found(f->q, f->index, &found_first_time)){
            if (add_item(matched_queries, f->q) == EC_FAIL) return EC_FAIL;
        }
        /* Insert query to candidate_queries */
        if (found_first_time == true) {
            if (add_item(candidate_queries, f->q) == EC_FAIL) return EC_FAIL;
        }
        pthread_mutex_unlock(&(job_scheduler.candidate_queries_mtx));

        node = get_next_node(node);
    }

    
    return EC_SUCCESS;
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

void destroy_entry_void(void *e) {
    entry ce = e;
    for(int i=0 ; i<3 ; i++) {
        if(ce->payload[i]) {
            destroy_list(&(ce->payload[i]));
            ce->payload[i] = NULL;
        }
    }
    free(ce->w);
    free(ce);
    ce = NULL;
}
