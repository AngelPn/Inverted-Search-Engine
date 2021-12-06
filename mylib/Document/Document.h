#ifndef DOC_H
#define DOC_H

#include <stdbool.h>
#include "core.h"
#include "LinkedList.h"

typedef struct document *Document;

/* Creates document */
Document create_document(DocID doc_id);

ErrorCode match_document(Document d, LinkedList queries);

void destroy_document(void *d);

unsigned int get_num_res(Document d);
QueryID* get_query_ids(Document d);
void* get_doc_id(Document d);

#endif