#ifndef ENTRY_LIST_H
#define ENTRY_LIST_H

#include <stdbool.h>
#include "core.h"

/* pointer to function that destroys an item */
typedef ErrorCode (*DestroyFunc)(void *item);
/* pointer to function that prints an item */
typedef void (*PrintItem)(void *item);

typedef struct list *entry_list;
typedef struct list_node *ListNode;

/* Creates an entry list */
ErrorCode create_entry_list(entry_list *el, DestroyFunc destroy_item);

/* Returns number of entries in entry list */
unsigned int get_number_entries(const entry_list el);

/* Returns the first and the next item of list */
void *get_first(const entry_list el);
void *get_next(const entry_list el, ListNode node);

/* Returns the item of the node */
void *get_node_item(ListNode node);

/* Returns the first and the next node of list */
ListNode get_first_node(const entry_list el);
ListNode get_next_node(ListNode node);

/* Add item as first item to entry list */
ErrorCode add_entry(entry_list el, void *e);

/* Sets or changes the destroy item */
void list_set_destroy_item(entry_list el, DestroyFunc destroy_item);

/* Destroys list */
ErrorCode destroy_entry_list(entry_list el);

/* Prints items of list */
void print_entry_list(entry_list el, PrintItem print);

#endif
