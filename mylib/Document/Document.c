#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "Document.h"
#include "Query.h"
#include "HashTable.h"

struct document
{
	DocID doc_id;
    char *ded_doc_str;      /* deduplicated doc string */
    HashT *deduplication;   /* hash table for deduplication */
    LinkedList candidate_queries;
    LinkedList matched_queries;
};

void* get_doc_id(Document d){
    return &(d->doc_id);
}

Document create_document(DocID doc_id) {
    Document d = (Document)malloc(sizeof(struct document));
    d->doc_id = doc_id;
    d->ded_doc_str = NULL;
    d->candidate_queries = NULL;
    d->matched_queries = NULL;
    return d;
}

char *deduplicate_doc_str(Document d, const char* doc_str) {
    d->deduplication = HashT_init(string, 1000, NULL);

    /* Allocating new char array because strtok() cant be applied on string literals */
    char* new_txt = malloc(strlen(doc_str) + 1);
    strcpy(new_txt, doc_str);

    d->ded_doc_str = malloc(strlen(doc_str) + 2);
    d->ded_doc_str[0] = '\0';

    char* token = strtok(new_txt, " ");
    while(token != NULL){
        /* Insert each unique word in the hashtable and in the ded_doc_str */
        /* If word found, get next token*/
        if (HashT_insert(d->deduplication, token, NULL)) {
            d->ded_doc_str = strcat(d->ded_doc_str, " ");
            d->ded_doc_str = strcat(d->ded_doc_str, token);
        }
        token = strtok(NULL, " ");
    }
    // HashT_delete(d->deduplication);
    return d->ded_doc_str;
}

void match_document(Document d, LinkedList matched_queries) {
    d->matched_queries = matched_queries;
}

void reset_candidate_queries(Document d, LinkedList candidate_queries) {
    d->candidate_queries = candidate_queries;
    ListNode node = get_first_node(candidate_queries);
    while (node != NULL) {
        reset_found(get_node_item(node));
        node = get_next_node(node);
    }   
}

int cmpfunc(const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

ErrorCode get_next_avail_result(Document d, DocID* p_doc_id, unsigned int* p_num_res, QueryID** p_query_ids) {
    *p_doc_id = d->doc_id;
    LinkedList matched_queries = d->matched_queries;
    *p_num_res = get_number_items(matched_queries);
    if ((*p_query_ids = (QueryID *)malloc((*p_num_res) * sizeof(QueryID))) == NULL) return EC_FAIL;

    ListNode node = get_first_node(matched_queries);
    for (int i = 0; i < (*p_num_res); i++) {
        (*p_query_ids)[i] = get_queryID(get_node_item(node));
        node = get_next_node(node);
    }
    qsort(*p_query_ids, *p_num_res, sizeof(QueryID), cmpfunc);
    return EC_SUCCESS;
}

void destroy_document(void *d) {
    Document dd = d;
    free(dd->ded_doc_str);
    HashT_delete(dd->deduplication);
    destroy_list(&(dd->candidate_queries));
    destroy_list(&(dd->matched_queries));
    free(dd);
    dd=NULL;
}
