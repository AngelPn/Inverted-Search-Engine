#ifndef QUERY_H
#define QUERY_H

#include <stdbool.h>
#include "core.h"
#include "LinkedList.h"

typedef struct query_struct *Query;

/* Creates query */
Query create_query(QueryID query_id);

/* Returns the query ID */
QueryID get_queryID(Query q);

bool found(Query q, int index);

void reset_found(Query q);

bool returnQuery(Query q);

/* Deallocates memory of query */
void destroy_query(void *q);

/* Size setter*/
void set_size(Query q,int s);

void set_info_words(Query q, int index, LinkedList list, ListNode node);

ErrorCode end_query(Query q);

#endif