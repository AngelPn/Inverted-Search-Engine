#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "Document.h"
#include "Query.h"

struct document
{
	DocID doc_id;
    LinkedList matched_queries;
};

void* get_doc_id(Document d){
    return &(d->doc_id);
}

Document create_document(DocID doc_id) {
    Document d = (Document)malloc(sizeof(struct document));
    d->doc_id = doc_id;
    d->matched_queries = NULL;
    return d;
}

void match_document(Document d, LinkedList matched_queries) {
    d->matched_queries = matched_queries;
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
    destroy_list(&(dd->matched_queries));
    free(dd);
    dd=NULL;
}
