#ifndef ENTRY_H
#define ENTRY_H

#include <stdbool.h>
#include "core.h"

typedef struct entry_struct *entry;
typedef char word;

/* Creates entry */
ErrorCode create_entry(const word *w, entry *e);

/* Returns the word of entry */
word *get_entry_word(entry e);

/* Prints the word of entry */
void print_entry_word(void *e);

/* Deallocates memory of entry */
ErrorCode destroy_entry(void *e);

#endif
