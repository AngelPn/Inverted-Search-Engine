#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "Query.h"

struct query_struct
{
	QueryID query_id;
    // MatchType match_type;
	// unsigned int match_dist;
    int size;                       /* number of words in query */
	// char *words[MAX_QUERY_WORDS];   /* pointers to words of query */
    bool found[MAX_QUERY_WORDS];    /* true if indexed word is found */
    int counter; /* counting found query words */
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

QueryID get_queryID(Query q) {
    return q->query_id;
}

void found(Query q, int index) {
    if(q->found[index] == false) {
        q->counter++;
        q->found[index] = true;
    }
}

void reset_found(Query q) {
    for (int i = 0; i < MAX_QUERY_WORDS; i++)
        q->found[i] = false;
}

bool returnQuery(Query q) {
    if(q->size == q->counter)
        return true;
    else
        return false;
}

ErrorCode destroy_query(void **q) {
    Query cq = *q;
    free(cq);
    *q = NULL;
    return EC_SUCCESS;
}
