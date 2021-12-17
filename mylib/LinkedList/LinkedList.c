#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "LinkedList.h"

struct list 
{
	ListNode dummy;
    ListNode last;
	unsigned int number_entries;
    DestroyFunc destroy_item;
};

struct list_node
{
	void *item;
	ListNode next;
    ListNode prev; /* double linked-list */
};

ErrorCode create_list(LinkedList *el, DestroyFunc destroy_item) {

    if (((*el) = (LinkedList)malloc(sizeof(struct list))) == NULL)
        return EC_FAIL;

    if (((*el)->dummy = (ListNode)malloc(sizeof(struct list_node))) == NULL)
        return EC_FAIL;
    (*el)->dummy->next = NULL;
    (*el)->dummy->prev = NULL;
    (*el)->last = (*el)->dummy;

    (*el)->number_entries = 0;
    (*el)->destroy_item = destroy_item;

    return EC_SUCCESS;
}

unsigned int get_number_items(const LinkedList el) {
    return el->number_entries;
}

void *get_first(const LinkedList el) {
    ListNode first_node = el->dummy->next;
    assert(first_node != NULL);
    return first_node->item;
}

void *get_next(const LinkedList el, ListNode node) {
    assert(node != NULL);
    ListNode next_node = node->next;
    return next_node->item;
}

void *get_node_item(ListNode node) {
    assert(node != NULL);
    return node->item;
}

ListNode get_first_node(const LinkedList el) {
    return el->dummy->next;
}

ListNode get_dummy_node(const LinkedList el) {
    return el->dummy;
}

ListNode get_next_node(ListNode node) {
    return node->next;
}

void *list_insert_next(LinkedList el, ListNode node, void *item) {

    ListNode new_node = NULL;
    if ((new_node = (ListNode)malloc(sizeof(struct list_node))) == NULL) {
        return NULL;
    }
    
	new_node->item = item;
	new_node->next = node->next;
    new_node->prev = node;
	node->next = new_node;
    if (el->last == node) {
        el->last = new_node;
    }

	el->number_entries++;

	return new_node;
}

ErrorCode add_item(LinkedList el, void *e) {
    if (list_insert_next(el, el->dummy, e) != NULL)
        return EC_SUCCESS;
    else return EC_FAIL;
}

void add_item_last(LinkedList el, void *item) {
    list_insert_next(el, el->last, item);
}

void *push_item(LinkedList el, void *item) {
    ListNode new_node = NULL;
    if ((new_node = (ListNode)malloc(sizeof(struct list_node))) == NULL) {
        return NULL;
    }
    
	new_node->item = item;
	new_node->next = el->dummy->next;

    if (el->dummy->next != NULL)
        el->dummy->next->prev = new_node;

    new_node->prev = el->dummy;
	el->dummy->next = new_node;

    if (el->last == NULL) {
        el->last = new_node;
    }

	el->number_entries++;

	return new_node;
}

void *pop_item(LinkedList el) {
    ListNode pop = NULL;
    if ((pop = el->dummy->next) == NULL)
        return NULL;

    el->dummy->next = pop->next;
    if (pop->next != NULL)
        pop->next->prev = el->dummy;
    void *item = pop->item;
	free(pop);

	(el->number_entries)--;

    return item;
}

void list_remove_next(LinkedList el, ListNode node) {
    if (node == NULL) {
        node = el->dummy;
    }

	ListNode removed = node->next;
	assert(removed != NULL);

	if (el->destroy_item != NULL)
		el->destroy_item(&(removed->item));

	node->next = removed->next;
    if (removed->next != NULL) {
        removed->next->prev = node;
    }
    if (el->last == removed) {
        el->last = node;
    }

	(el->number_entries)--;
    free(removed);
}

void list_remove(LinkedList el, ListNode node) {
    list_remove_next(el, node->prev);
}

void list_set_destroy_item(LinkedList el, DestroyFunc destroy_item) {
	el->destroy_item = destroy_item;
}

ErrorCode destroy_list(LinkedList *el) {
    ListNode node = (*el)->dummy;
	while (node != NULL){
		ListNode next = node->next;

		if (node != (*el)->dummy && (*el)->destroy_item != NULL) {
            if ((*el)->destroy_item(&(node->item)) == EC_FAIL)
                return EC_FAIL;
        }

		free(node);
		node = next;
	}
	free((*el));
    *el = NULL;
    return EC_SUCCESS;
}

void print_list(LinkedList el, PrintItem print) {
    for (ListNode node = el->dummy->next; node != NULL; node = node->next)
		print(node->item);
}
