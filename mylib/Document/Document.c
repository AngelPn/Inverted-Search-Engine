#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "Document.h"
#include "Query.h"

struct document
{
	DocID doc_id;
	unsigned int num_res;
	QueryID* query_ids;
    // QueryID** query_ids;
};

unsigned int get_num_res(Document d){
    return d->num_res;
}

QueryID* get_query_ids(Document d){
    return d->query_ids;
}

void* get_doc_id(Document d){
    return &(d->doc_id);
}

Document create_document(DocID doc_id) {
    Document d = (Document)malloc(sizeof(struct document));
    d->doc_id = doc_id;
    d->num_res = 0;
    d->query_ids = NULL;
    return d;
}

int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

ErrorCode match_document(Document d, LinkedList queries) {
    d->num_res = get_number_items(queries);
    d->query_ids = (QueryID *)malloc((d->num_res) * sizeof(QueryID));

    ListNode node = get_first_node(queries);
    for (int i = 0; i < d->num_res; i++) {
        // d->query_ids[i] = get_queryID(get_node_item(node));
        // d->query_ids[i] = malloc(sizeof(int));
        memcpy(&(d->query_ids[i]), get_query_key(get_node_item(node)), sizeof(int));
        node = get_next_node(node);
    }
    qsort(d->query_ids, d->num_res, sizeof(QueryID), cmpfunc);

    return EC_SUCCESS;
}

void destroy_document(void *d) {
    Document dd = d;
    // for (int i = 0; i<dd->num_res; i++){
    //     free(dd->query_ids[i]);
    // }
    free(dd->query_ids);
    dd->query_ids = NULL;
    free(dd);
    dd=NULL;
}
