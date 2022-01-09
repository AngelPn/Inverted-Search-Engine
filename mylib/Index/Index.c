#include <stdio.h>
#include <stdlib.h>

#include "Index.h"
#include "Document.h"
#include "JobScheduler.h"
#include "common_types.h"

ErrorCode init_index(Index *index) {
    if ((index->ExactMatch = HashT_init(string, 1000, destroy_entry_void)) == NULL) return EC_FAIL;
    else if ((index->EditDist = create_BK_tree(EditDistance)) == NULL) return EC_FAIL;
    else if ((index->HammingDist = create_HammingTree(HammingDistance)) == NULL) return EC_FAIL;
    else if ((index->Queries = HashT_init(integer, 1000, destroy_query)) == NULL) return EC_FAIL;
    else if ((index->Documents = HashT_init(integer, 1000, destroy_document)) == NULL) return EC_FAIL;
    index->curr_doc = 1;
    return EC_SUCCESS;
}

ErrorCode insert_ExactMatch(Index *index, char *token, unsigned int match_dist, Query query, int token_index) {
    entry e = HashT_get(index->ExactMatch, token);
    if (e == NULL) {
        create_entry(token, &e);
        HashT_insert(index->ExactMatch, get_entry_word(e), e);
        return insert_info_payload(e, match_dist, query, token_index);
    } else {
        return insert_info_payload(e, match_dist, query, token_index);
    }
}

ErrorCode insert_EditDist(Index *index, char *token, unsigned int match_dist, Query query, int token_index) {
    entry e = insert_BK_tree(index->EditDist, token);
    return insert_info_payload(e, match_dist-1, query, token_index);
}

ErrorCode insert_HammingDist(Index *index, char *token, unsigned int match_dist, Query query, int token_index) {
    return insert_info_payload(insert_HammingTree(index->HammingDist, token), match_dist-1, query, token_index);
}

ErrorCode insert_index(Index *index, char *token, MatchType match_type, unsigned int match_dist, Query query, int token_index) {
    switch (match_type) {
        case MT_EXACT_MATCH:
            return insert_ExactMatch(index, token, match_dist, query, token_index);
        case MT_EDIT_DIST:
            return insert_EditDist(index, token, match_dist, query, token_index);
        case MT_HAMMING_DIST:
            return insert_HammingDist(index, token, match_dist, query, token_index);
        default:
            return EC_FAIL;
    }
}

ErrorCode lookup_index(Index *index, char* token, HashT* candidate_queries, LinkedList matched_queries){
    entry e = HashT_get(index->ExactMatch, token);
    if (e != NULL) {
        if (update_payload(e, 0, candidate_queries, matched_queries) == EC_FAIL)
            return EC_FAIL;
    }
    if (lookup_BKtree(token, index->EditDist, 3, candidate_queries, matched_queries) == EC_FAIL)
        return EC_FAIL;
    if (lookup_HammingTree(index->HammingDist, token, 3, candidate_queries, matched_queries) == EC_FAIL)
        return EC_FAIL;
    return EC_SUCCESS;
}

ErrorCode destroy_index(Index *index) {
    HashT_delete(index->ExactMatch);
    HashT_delete(index->Queries);
    HashT_delete(index->Documents);
    if (destroy_BK_tree(&(index->EditDist)) == EC_FAIL) return EC_FAIL;
    else if (destroy_HammingTree(index->HammingDist) == EC_FAIL) return EC_FAIL;
    else return EC_SUCCESS;
}
