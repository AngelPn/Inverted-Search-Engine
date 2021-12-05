#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "Document.h"

struct document
{
	DocID doc_id;
	unsigned int num_res;
	QueryID* query_ids;
};

Document create_document(DocID doc_id) {
    Document d = (Document)malloc(sizeof(struct document));
    d->num_res = 0;
    d->query_ids = NULL;
    return d;
}

ErrorCode match_document(Document d, LinkedList queries) {
    d->num_res = get_number_items(queries);
    d->query_ids = (QueryID *)malloc((d->num_res) * sizeof(QueryID));

    ListNode node = get_first_node(queries);
    for (int i = 0; i < d->num_res; i++) {
        d->query_ids[i] = get_queryID(get_node_item(node));
        node = get_next_node(node);
    }
    return EC_SUCCESS;
}

void destroy_document(void *d) {
    Document dd = d;
    free(dd->query_ids);
    free(dd);
}
