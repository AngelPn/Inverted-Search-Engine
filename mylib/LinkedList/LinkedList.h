#ifndef LIST_H
#define LIST_H

#include <stdbool.h>
#include "core.h"

/* pointer to function that destroys an item */
typedef ErrorCode (*DestroyFunc)(void **item);
/* pointer to function that prints an item */
typedef void (*PrintItem)(void *item);

typedef struct list *LinkedList;
typedef struct list_node *ListNode;

/* Creates a list */
ErrorCode create_list(LinkedList *el, DestroyFunc destroy_item);

/* Returns number of items in list */
unsigned int get_number_items(const LinkedList el);

/* Returns the first and the next item of list */
void *get_first(const LinkedList el);
void *get_next(const LinkedList el, ListNode node);

/* Returns the item of the node */
void *get_node_item(ListNode node);

/* Returns the first and the next node of list */
ListNode get_first_node(const LinkedList el);
ListNode get_next_node(ListNode node);
ListNode get_dummy_node(const LinkedList el);

/* Adds item as first item to list */
ErrorCode add_item(LinkedList el, void *e);

void add_item_last(LinkedList el, void *item);

/* Pushes item to list and returns it */
void *push_item(LinkedList el, void *item);

/* Pops item from list and returns it */
void *pop_item(LinkedList el);

/* Removes the next node from ListNode node */
void list_remove(LinkedList el, ListNode node);

/* Sets or changes the destroy item */
void list_set_destroy_item(LinkedList el, DestroyFunc destroy_item);

/* Destroys list */
ErrorCode destroy_list(LinkedList *el);

/* Prints items of list */
void print_list(LinkedList el, PrintItem print);

#endif
