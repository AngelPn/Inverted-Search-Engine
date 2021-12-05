#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "Query.h"

struct query_struct
{
	QueryID query_id;
    int size;                           /* number of words in query */
    LinkedList lists[MAX_QUERY_WORDS];  /* pointers to payload lists of query */
	ListNode nodes[MAX_QUERY_WORDS];    /* pointers to payload nodes of query */
    bool found[MAX_QUERY_WORDS];        /* true if indexed word is found */
    int counter;                        /* counting found query words */
};

Query create_query(QueryID query_id) {
    Query q = (Query)malloc(sizeof(struct query_struct));
    q->query_id = query_id;
    q->counter = 0;
    for (int i = 0; i < MAX_QUERY_WORDS; i++)
        q->found[i] = false;
    return q;
}

void set_size(Query q,int s){
    q->size = s;
}

void set_info_words(Query q, int index, LinkedList list, ListNode node) {
    q->lists[index] = list;
    q->nodes[index] = node;
}

QueryID get_queryID(Query q) {
    return q->query_id;
}

bool found(Query q, int index) {
    if(q->found[index] == false) {
        q->counter++;
        q->found[index] = true;

        if(q->size == q->counter)
            return true;
        else
            return false;
    }
    return false;
}

void reset_found(Query q) {
    for (int i = 0; i < MAX_QUERY_WORDS; i++)
        q->found[i] = false;
    q->counter = 0;
}

ErrorCode end_query(Query q) {
    for (int i = 0; i < q->size; i++) {
        list_remove(q->lists[i], q->nodes[i]);
    }
    return EC_SUCCESS;
}

bool returnQuery(Query q) {
    if(q->size == q->counter)
        return true;
    else
        return false;
}

void destroy_query(void *q) {
    Query cq = q;
    if(cq != NULL) {
        free(cq);
        cq = NULL;
    }
}
