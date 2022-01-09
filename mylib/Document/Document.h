#ifndef DOC_H
#define DOC_H

#include "core.h"
#include "LinkedList.h"
#include "HashTable.h"

typedef struct document *Document;

/* Creates document */
Document create_document(DocID doc_id);

/* Gets document's ID */
void* get_doc_id(Document d);

/* Deduplication of foc_str */
char *deduplicate_doc_str(Document d, const char* doc_str);

/* Sets matched queries to document */
void match_document(Document d, LinkedList matched_queries, HashT* candidate_queries);

/* Gets matched queries of document and returns query IDs sorted */
ErrorCode get_next_avail_result(Document d, DocID* p_doc_id, unsigned int* p_num_res, QueryID** p_query_ids);

/* Deallocated memory of document */
void destroy_document(void *d);

ErrorCode destroy_document_double(void **d);

#endif