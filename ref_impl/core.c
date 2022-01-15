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
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "core.h"
#include "Index.h"
#include "Document.h"

bool start_q_flag = 0;

// Struct used for the search machine
static Index superdex; /* superdex = super + index, it is our super index */

ErrorCode InitializeIndex() {
    return init_index(&superdex);
}

ErrorCode DestroyIndex() {
    wait_all_jobs_finish(&job_scheduler);
	return destroy_index(&superdex);
}

ErrorCode StartQuery(QueryID query_id, const char* query_str, MatchType match_type, unsigned int match_dist) {
    wait_match_document_jobs_finish(&job_scheduler);
    void * args[4];

    int *query_ID = malloc(sizeof(int));
    *query_ID = query_id;
    args[0] = query_ID;

    char *doc_string = malloc(strlen(query_str) + 1);;
    strcpy(doc_string, query_str);
    args[1] = doc_string;

    MatchType *mt = malloc(sizeof(MatchType));
    *mt = match_type;
    args[2] = mt;

    int *md = malloc(sizeof(int));
    *md = match_dist;
    args[3] = md;

    Job j = create_job(START_Q, args);
    submit_job(&job_scheduler, &j);

    return EC_SUCCESS;
}

ErrorCode StartQuery_job(void *args[4]) {

    QueryID query_id = *(int*)(args[0]);
    char* query_str = args[1];
    MatchType match_type = *(MatchType*)args[2];
    unsigned int match_dist = *(int*)(args[3]);

    /* Tokenize query_str */
    char* new_query_str = (char*)malloc(strlen(query_str) + 1);
    strcpy(new_query_str, query_str);
    char* temp = new_query_str;
    char* token;

    /* Insert query in active set of queries */
    Query query = create_query(query_id);
    pthread_mutex_lock(&(job_scheduler.queries_mtx));
    HashT_insert(superdex.Queries, get_query_key(query), query);
    pthread_mutex_unlock(&(job_scheduler.queries_mtx));

    int query_words = 0; /* number of words in query */
    ErrorCode state = EC_SUCCESS;
    /* For each word in query, insert it to index */
    while ((token = strtok_r(new_query_str, " ", &new_query_str)) && state == EC_SUCCESS) {
        state = insert_index(&superdex, token, match_type, match_dist, query, query_words);
        query_words++;
    }
    /* Set the number of words in query */
    set_size(query, query_words);
    free(temp);
    free(args[0]);
    free(args[1]);
    free(args[2]);
    free(args[3]);
	return state;
}

ErrorCode EndQuery(QueryID query_id)
{
    wait_all_jobs_finish(&job_scheduler);
    Query query = NULL;
    if ((query = HashT_get(superdex.Queries, &query_id)) != NULL) {
        return end_query(query);
    } else {
        return EC_FAIL;
    }
}

ErrorCode MatchDocument(DocID doc_id, const char* doc_str)
{
    wait_insert_index_jobs_finish(&job_scheduler);

    void * args[5];

    int *doc_ID = malloc(sizeof(int));
    *doc_ID = doc_id;
    args[0] = doc_ID;

    char *doc_string = malloc(strlen(doc_str) + 1);;
    strcpy(doc_string, doc_str);
    args[1] = doc_string;

    Job j = create_job(MATCH_DOCUMENT, args);
    submit_job(&job_scheduler, &j);
    

	return EC_SUCCESS;
}

ErrorCode MatchDocument_job(void *args[4])
{
    DocID doc_id = *(int*)(args[0]);
    char* doc_str = args[1];

    LinkedList matched_queries = NULL;
    /* Create list of matched queries: queries that match the document */
    if (create_list(&matched_queries, NULL) == EC_FAIL) return EC_FAIL;

    HashT* candidate_queries = HashT_init(integer, 1000, destroy_query);

    /* Create document */
    Document d = create_document(doc_id);

    /* For each token of deduplicated doc_str, search token in Index */
    char* ded_doc = deduplicate_doc_str(d, doc_str);
    char* token;
    ErrorCode state = EC_SUCCESS;
    while ((token = strtok_r(ded_doc, " ", &ded_doc)) && state == EC_SUCCESS) {
        state = lookup_index(&superdex, token, candidate_queries, matched_queries);
    }

    /* Match queries with document */
    // printf("d: %d\n", *(int*)get_doc_id(d));
    match_document(d, matched_queries, candidate_queries);
    // printf("IM HERE\n");
    // print_list(matched_queries, print_query);

    /* Traverse candidate_queries and reset "found" field of queries */
    // reset_candidate_queries(d, candidate_queries);

    /* Insert document to list of documents */
    pthread_mutex_lock(&(job_scheduler.job_mtx));
    HashT_insert(superdex.Documents, get_doc_id(d), d); // eventually our only shared data
    pthread_mutex_unlock(&(job_scheduler.job_mtx));

    free(args[0]);
    free(doc_str);
	return state;
}

ErrorCode GetNextAvailRes(DocID* p_doc_id, unsigned int* p_num_res, QueryID** p_query_ids)
{
    wait_match_document_jobs_finish(&job_scheduler);
    Document d = NULL;
    if ((d = HashT_get(superdex.Documents, &superdex.curr_doc)) != NULL) {
        superdex.curr_doc++;
        return get_next_avail_result(d, p_doc_id, p_num_res, p_query_ids);
    } else {
        return EC_FAIL;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////

// Computes edit distance between a null-terminated string "a" with length "na"
//  and a null-terminated string "b" with length "nb"

/* Using bit-vector algorithm: https://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.142.1245&rep=rep1&type=pdf p.5 */
/* Instead of explicitly computing the values D[i,j] for i = {1...na} and j = {1...nb} */
/* the following length-(na) binary valued delta vectors are computed for j = {1...na} */
/* The vertical positive delta vector VP:   VP_i = 1 iff D[i,j] - D[i-1,j] = 1   */
/* The vertical negative delta vector VN:   VN_i = 1 iff D[i,j] - D[i-1,j] = -1  */
/* The horizontal positive delta vector HP: HP_i = 1 iff D[i,j] - D[i,j-1] = 1   */
/* The horizontal negative delta vector HN: HN_i = 1 iff D[i,j] - D[i,j-1] = -1  */
/* The diagonal zero delta vector D0:       D0_i = 1 iff D[i,j] = D[i-1,j-1]     */
/* The match vector X */
int EditDistance(char* a, int na, char* b, int nb)
{
    /* masks for each of the word length */
    /* For example: mask for a word with length = 4 is masks[3] = 0x0000000f = 0...01111 */
    /* For example: mask for a word with length = 5 is masks[4] = 0x0000001f = 0...011111 */
    unsigned int masks[MAX_WORD_LENGTH] = {
        0x00000001, 0x00000003, 0x00000007, 0x0000000f,
        0x0000001f, 0x0000003f, 0x0000007f, 0x000000ff,
        0x000001ff, 0x000003ff, 0x000007ff, 0x00000fff,
        0x00001fff, 0x00003fff, 0x00007fff, 0x0000ffff,
        0x0001ffff, 0x0003ffff, 0x0007ffff, 0x000fffff,
        0x001fffff, 0x003fffff, 0x007fffff, 0x00ffffff,
        0x01ffffff, 0x03ffffff, 0x07ffffff, 0x0fffffff,
        0x1fffffff, 0x3fffffff,
    };

    /* C uses char type to store characters and letters. */ 
    /* However, the char type is integer type because underneath C stores integer numbers instead of characters.*/
    /* In C, char values are stored in 1 byte in memory and value range from -128 to 127 or 0 to 255. */
    /* posbits (position bits) index is unsigned char value (range from 0 to 255) */
    unsigned int posbits[256] = {0};

    /* For each of char value in string `a` */
    for (int i = 0; i < na; i++){
        /* Bitwise OR on the operands: a[i] and (1 << i) */
        /* (1 << i): shift left 1 with the integer i -> (1*2^i) */
        posbits[(unsigned char)a[i]] |= 1 << i;
    }

    int dist = na;
    unsigned int mask = 1 << (na - 1);   /* mask is left shifting of 1 with (na-1) */
    unsigned int VP   = masks[na - 1];   /* VP initialized as mask of word length of `a` */
    unsigned int VN   = 0;

    for (int i = 0; i < nb; i++) { /* Computing the j-th column */
        /* Build the appropriate match vector into X */
        unsigned int y = posbits[(unsigned char)b[i]];  /* y is position bits of unsigned char value of b at index i */
        unsigned int X  = y | VN;                       /* X is bitwise OR of y and VN */
        unsigned int D0 = (((X & VP) + VP) ^ VP) | X;
        unsigned int HP = VN | ~(D0|VP);                /* HP is bitwise OR of VN and NOT(VP bitwise OR D0) */
        unsigned int HN = D0 & VP;                      /* HN is bitwise AND of VP and D0 */

        if (HP & mask) { dist++; }
        if (HN & mask) { dist--; }

        /* Update the appropriate cell value */
        VN = D0 & ((HP << 1) | 1);
        VP = (HN << 1) | ~(D0 | ((HP << 1) | 1));
    }
    return dist;
}

///////////////////////////////////////////////////////////////////////////////////////////////

// Computes Hamming distance between a null-terminated string "a" with length "na"
//  and a null-terminated string "b" with length "nb" 
int HammingDistance(char* a, int na, char* b, int nb)
{
	int j, oo=0x7FFFFFFF;
	if(na!=nb) return oo;
	
	int num_mismatches=0;

	for(j = 0; j < na; j++)
        num_mismatches += (a[j] ^ b[j]) > 0 ? 1 : 0;
	
	return num_mismatches;
}
