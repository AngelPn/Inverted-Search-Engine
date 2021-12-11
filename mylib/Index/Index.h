#ifndef INDEX_H
#define INDEX_H

#include "HashTable.h"
#include "HammingTree.h"
#include "Query.h"

typedef struct index
{
    HashT* ExactMatch;
    BK_tree EditDist;
    HammingTree HammingDist;

    HashT* Queries;
    HashT* Docs;
    int cur_doc;
} Index;

ErrorCode init_index(Index *index);

ErrorCode insert_index(Index *index, char *token, MatchType match_type, unsigned int match_dist, Query query, int token_index);
ErrorCode lookup_index(Index *index, char* token, HashT *candidate_queries, LinkedList candidates, LinkedList matched_queries);
ErrorCode destroy_index(Index *index);

#endif
