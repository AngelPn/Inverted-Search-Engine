#ifndef QUERY_H
#define QUERY_H

#include "core.h"
#include "LinkedList.h"

typedef struct query_struct *Query;

/* Creates query */
Query create_query(QueryID query_id);

/* Size setter*/
void set_size(Query q,int s);

/* Sets pointers to payload's list and node of list */
void set_info_location(Query q, int index, LinkedList list, ListNode node);

/* Returns the query ID */
QueryID get_queryID(Query q);
void* get_query_key(Query q);

int get_size(Query q);

/* Returns found[index] */
bool get_index_found(Query q, int index);

/* Sets found[index] to true and returns true if all words of query are found */
bool found(Query q, int index);

/* Sets found field to false */
void reset_found(Query q);

/* Removes the nodes of lists in query */
ErrorCode end_query(Query q);

void print_query(void *q);

/* Deallocates memory of query */
void destroy_query(void *q);

#endif