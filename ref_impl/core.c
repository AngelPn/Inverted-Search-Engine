/*
 * core.cpp version 1.0
 * Copyright (c) 2013 KAUST - InfoCloud Group (All Rights Reserved)
 * Author: Amin Allam
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "core.h"
#include "ed.h"
#include "Index.h"
#include "Document.h"

// Global structs used for the search machine
Index superdex; /* superdex = super + index, it is our super index */

ErrorCode InitializeIndex() {
    return init_index(&superdex);
}

ErrorCode DestroyIndex() {
	return destroy_index(&superdex);
}

ErrorCode StartQuery(QueryID query_id, const char* query_str, MatchType match_type, unsigned int match_dist) {
    /* Tokenize query_str */
    char* new_query_str = (char*)malloc(strlen(query_str) + 1);
    strcpy(new_query_str, query_str);
    char* token = strtok(new_query_str, " ");

    /* Check if query is already in active set of queries */
    /* If not, insert query in active set of queries */
    Query query = NULL;
    if ((query = HashT_get(superdex.Queries, &query_id)) == NULL) {
        query = create_query(query_id);
        HashT_insert(superdex.Queries, &query_id, query);
    }

    int query_words = 0; /* number of words in query */
    ErrorCode state = EC_SUCCESS;
    /* For each word in query, insert it to index */
    while (token != NULL && state == EC_SUCCESS) {
        state = insert_index(&superdex, token, match_type, match_dist, query, query_words);
        token = strtok(NULL, " \n");
        query_words++;
    }
    /* Set the number of words in query */
    set_size(query, query_words + 1);
    // print_BK_tree(superdex.EditDist);
    // print_HammingTree(superdex.HammingDist);
    free(new_query_str);
	return state;
}

ErrorCode EndQuery(QueryID query_id)
{
    Query query = NULL;
    if ((query = HashT_get(superdex.Queries, &query_id)) == NULL) {
        return EC_FAIL;
    } else {
        return end_query(query);
    }
}

//We insert each word one by one in a big hashtable and at the same time we check if it already exists in the hashtable
//if it already exists it means it is a duplicate word so it is not in the result string that it is returned
char* deduplication(char* text){
    HashT* HT = HashT_init(string,1000,NULL);
    const char space[2] = " ";
    //allocating new char array because strtok() cant be applied on string literals
    char* new_txt= malloc(strlen(text)+1);
    strcpy(new_txt,text);
    //getting the first word from the text
    char* token = strtok(new_txt,space);
    //this is the string that we will write the text word by word without the duplicates
    char* new_str = malloc(strlen(text)+1);
    new_str[0] = '\0';
    while(token != NULL){
        //we insert each word in the hashtable and if it already exists then it is not written in our result string
        if(HashT_insert(HT,token,NULL)) {
            strcat(new_str, space);
            strcat(new_str, token);
        }
        //getting the next character
        token = strtok(NULL,space);
        //printf("token: %s\n",token);
    }
    HashT_delete(HT);
    free(new_txt);
    //NOTE: the returned string must be freed
    return new_str;
}

ErrorCode MatchDocument(DocID doc_id, const char* doc_str)
{
    // Create LinkedList of candidate_queries (queries that possibly match the document)
    HashT* candidate_queries = HashT_init(integer, 50, NULL);
    // Create LinkedList of matched_queries (queries that match the document)
    LinkedList matched_queries;
    create_list(&matched_queries, NULL);

    // Deduplication of doc_str
    char* doc_str_copy = malloc(strlen(doc_str)+1);
    strcpy(doc_str_copy, doc_str);
    char* ded_doc = deduplication(doc_str_copy);
    free(doc_str_copy);
    // Tokenization of deduplicated doc_str

    // For each token of deduplicated doc_str, search token in Index:
        // search ExactMatch
        // search EditDist
        // search HammingDist
        // and fill in the "found" field in Query from info_struct
        // Insert Query in LinkedList of candidate_queries because we have changed its "found" field
        // If Query's "found" field is all true -> add Query to matched_queries

    char* token = strtok(ded_doc, " ");
    ErrorCode state = EC_SUCCESS;
    while (token != NULL && state == EC_SUCCESS) {
        state = lookup_index(&superdex, token, candidate_queries, matched_queries);
        token = strtok(NULL, " \n");
    }
    // Traverse candidate_queries and reset "found" field of queries
    HashT_entry* curr_hash_node = NULL, *next_hash_node = NULL;
    int bucket = 0;

    Query q = NULL;
    do {
        q = HashT_parse(candidate_queries, curr_hash_node, &next_hash_node, &bucket);
        reset_found(q);
        curr_hash_node = next_hash_node;
    } while (next_hash_node != NULL);

    // Create Document struct
    Document d = create_document(doc_id);
    // Call match_document given created Document and matched_queries
    match_document(d, matched_queries);
    // free candidate_queries and matced_queries
    destroy_list(&matched_queries);
    HashT_delete(candidate_queries);
	return EC_SUCCESS;
}


ErrorCode GetNextAvailRes(DocID* p_doc_id, unsigned int* p_num_res, QueryID** p_query_ids)
{
    // Search hash table of documents and get Document with given doc_id
    // get the query_ids of Document
	return EC_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////

 static const unsigned long int masks[64] = {
     0x0000000000000001, 0x0000000000000003, 0x0000000000000007, 0x000000000000000f,
     0x000000000000001f, 0x000000000000003f, 0x000000000000007f, 0x00000000000000ff,
     0x00000000000001ff, 0x00000000000003ff, 0x00000000000007ff, 0x0000000000000fff,
     0x0000000000001fff, 0x0000000000003fff, 0x0000000000007fff, 0x000000000000ffff,
     0x000000000001ffff, 0x000000000003ffff, 0x000000000007ffff, 0x00000000000fffff,
     0x00000000001fffff, 0x00000000003fffff, 0x00000000007fffff, 0x0000000000ffffff,
     0x0000000001ffffff, 0x0000000003ffffff, 0x0000000007ffffff, 0x000000000fffffff,
     0x000000001fffffff, 0x000000003fffffff, 0x000000007fffffff, 0x00000000ffffffff,
     0x00000001ffffffff, 0x00000003ffffffff, 0x00000007ffffffff, 0x0000000fffffffff,
     0x0000001fffffffff, 0x0000003fffffffff, 0x0000007fffffffff, 0x000000ffffffffff,
     0x000001ffffffffff, 0x000003ffffffffff, 0x000007ffffffffff, 0x00000fffffffffff,
     0x00001fffffffffff, 0x00003fffffffffff, 0x00007fffffffffff, 0x0000ffffffffffff,
     0x0001ffffffffffff, 0x0003ffffffffffff, 0x0007ffffffffffff, 0x000fffffffffffff,
     0x001fffffffffffff, 0x003fffffffffffff, 0x007fffffffffffff, 0x00ffffffffffffff,
     0x01ffffffffffffff, 0x03ffffffffffffff, 0x07ffffffffffffff, 0x0fffffffffffffff,
     0x1fffffffffffffff, 0x3fffffffffffffff, 0x7fffffffffffffff, 0xffffffffffffffff,
 };

// Computes edit distance between a null-terminated string "a" with length "na"
//  and a null-terminated string "b" with length "nb"
int EditDistance(char* a, int na, char* b, int nb)
{


	 int len_a = na, len_b = nb;
	 if (len_a > 64) {
       return -1;
     }

     unsigned long int posbits[256] = {0};

     for (int i=0; i < len_a; i++){
         posbits[(unsigned char)a[i]] |= 1ull << i;
     }
    
     int dist = len_a;
     unsigned long int mask = 1ull << (len_a - 1);
     unsigned long int VP   = masks[len_a - 1];
     unsigned long int VN   = 0;

     for (int i=0; i < len_b; i++){
       unsigned long int y = posbits[(unsigned char)b[i]];
       unsigned long int X  = y | VN;
       unsigned long int D0 = ((VP + (X & VP)) ^ VP) | X;
       unsigned long int HN = VP & D0;
       unsigned long int HP = VN | ~(VP|D0);
       X  = (HP << 1) | 1;
       VN =  X & D0;
       VP = (HN << 1) | ~(X | D0);
       if (HP & mask) { dist++; }
       if (HN & mask) { dist--; }
     }
    return dist;

	//---------------------------------------------------------

	return editdist(a, na, b, nb);
}

///////////////////////////////////////////////////////////////////////////////////////////////

// Computes Hamming distance between a null-terminated string "a" with length "na"
//  and a null-terminated string "b" with length "nb" 
int HammingDistance(char* a, int na, char* b, int nb)
{
	int j, oo=0x7FFFFFFF;
	if(na!=nb) return oo;
	
	int num_mismatches=0;
	// for(j=0;j<na;j++) if(a[j]!=b[j]) num_mismatches++;

	for(j = 0; j < na; j++)
        num_mismatches += (a[j] ^ b[j]) > 0 ? 1 : 0;
	
	return num_mismatches;
}
