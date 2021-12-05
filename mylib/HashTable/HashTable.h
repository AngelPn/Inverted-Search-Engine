/* Hash table */
#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdbool.h>
typedef struct HashT HashT;
typedef struct HashT_entry HashT_entry;
typedef enum key_type {integer, string} key_type;

HashT* HashT_init(key_type kt, unsigned int nbuckets, void (*destroy_item)(void *)); /*nbuckets is the number of buckets, destroy_item is the function to destroy the items to which hash table entries point */
void HashT_delete(HashT* hash_table);

bool HashT_insert(HashT* hash_table, void* key, void* item); /* key and item must be allocated */
void HashT_remove(HashT* hash_table, void* key);

bool HashT_exists(HashT* hash_table, void* key);
void* HashT_get(HashT* hash_table, void* key); /* returns NULL if key is not found*/

void HashT_print(HashT* hash_table, void (*print)(void*));
void* HashT_getNextEntry(HashT* hash_table); /*First call with a pointer to hash table to get 1st item. For next item call with NULL. If there are no more elements it returns NULL*/
void* HashT_parse(HashT* hash_table, HashT_entry* prev, HashT_entry** next, int* bucket);

void HashT_stats(HashT* hash_table); /* prints number of items, number of buckets and load factor */

#endif