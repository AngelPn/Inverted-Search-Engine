/*  Hash table with extendible hashing */
/*  This hash table does not allocate or deallocate the keys. Items must be allocated when given to insert and can be 
    deallocated by setting their destroy function in HashT_init destroy_item paramenter */

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdbool.h>
typedef struct HashT HashT;
typedef struct HashT_entry HashT_entry;
typedef enum key_type {integer, string} key_type;

/*nbuckets is the number of buckets (It must be a power of 2 for the extendible hashing so if the given value is not a power of 2 it calcualtes the nexrt higher one), destroy_item is the function to destroy the items to which hash table entries point */
HashT* HashT_init(key_type kt, unsigned int nbuckets, void (*destroy_item)(void *));

void HashT_delete(HashT* hash_table);

/* key and item must be allocated */
bool HashT_insert(HashT* hash_table, void* key, void* item); 
void HashT_remove(HashT* hash_table, void* key);

bool HashT_exists(HashT* hash_table, void* key);
void* HashT_get(HashT* hash_table, void* key); /* returns NULL if key is not found*/

void HashT_print(HashT* hash_table, void (*print)(void*));
void* HashT_parse(HashT* hash_table, HashT_entry* prev, HashT_entry** next, int* bucket);

/* prints number of items, number of buckets and load factor */
void HashT_stats(HashT* hash_table); 

int HashT_get_nbuckets(HashT* hash_table);
int HashT_get_nitems(HashT* hash_table);

#endif