#ifndef ENTRY_H
#define ENTRY_H

#include <stdbool.h>
#include "core.h"
#include "Query.h"

typedef struct entry_struct *entry;
typedef struct info_struct *info;

/* Creates entry */
ErrorCode create_entry(const char *w, entry *e);

/* Returns the word of entry */
char *get_entry_word(entry e);

/* Prints the word of entry */
void print_entry_word(void *e);

void remove_query_info(info inf);

/* Deallocates memory of entry */
ErrorCode destroy_entry(void **e);

ErrorCode update_entry_payload(entry e,unsigned int match_dist, Query q, int index);

#endif
