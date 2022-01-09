#ifndef ENTRY_INFO_H
#define ENTRY_INFO_H

#include "core.h"
#include "Query.h"
#include "HashTable.h"

typedef struct entry_struct *entry;

/* Creates entry struct */
ErrorCode create_entry(const char *w, entry *e);

/* Returns the word of entry */
char *get_entry_word(entry e);

/* Prints the word of entry */
void print_entry_word(void *e);

/* Inserts info to entry's payload */
ErrorCode insert_info_payload(entry e, unsigned int match_dist, Query q, int index);

ErrorCode update_payload(entry e, int threshold,  HashT* candidate_queries, LinkedList matched_queries);

/* Deallocates memory of entry */
ErrorCode destroy_entry(void **e);
void destroy_entry_void(void *e);

#endif
