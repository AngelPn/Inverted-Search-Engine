#include <stdio.h>
#include <stdlib.h>

#include "Index.h"
#include "Document.h"
#include "JobScheduler.h"

ErrorCode init_index(Index *index) {
    initialize_scheduler(&job_scheduler, NUM_THREADS);
    if ((index->ExactMatch = HashT_init(string, 1000, destroy_entry_void)) == NULL) return EC_FAIL;
    else if ((index->EditDist = create_BK_tree(EditDistance)) == NULL) return EC_FAIL;
    else if ((index->HammingDist = create_HammingTree(HammingDistance)) == NULL) return EC_FAIL;
    else if ((index->Queries = HashT_init(integer, 1000, destroy_query)) == NULL) return EC_FAIL;
    else if ((index->Documents = HashT_init(integer, 1000, destroy_document)) == NULL) return EC_FAIL;
    index->curr_doc = 1;
    return EC_SUCCESS;
}

ErrorCode insert_ExactMatch(Index *index, char *token, unsigned int match_dist, Query query, int token_index) {

    ErrorCode ec;
    pthread_mutex_lock(&(job_scheduler.em_mtx));
    entry e = HashT_get(index->ExactMatch, token);
    if (e == NULL) {
        create_entry(token, &e);
        HashT_insert(index->ExactMatch, get_entry_word(e), e);
        ec = insert_info_payload(e, match_dist, query, token_index);
    } else {
        ec = insert_info_payload(e, match_dist, query, token_index);
    }
    pthread_mutex_unlock(&(job_scheduler.em_mtx));

    return ec;
}

ErrorCode insert_EditDist(Index *index, char *token, unsigned int match_dist, Query query, int token_index) {

    ErrorCode ec;
    pthread_mutex_lock(&(job_scheduler.ed_mtx));
    entry e = insert_BK_tree(index->EditDist, token);
    ec = insert_info_payload(e, match_dist-1, query, token_index);
    pthread_mutex_unlock(&(job_scheduler.ed_mtx));

    return ec;
}

ErrorCode insert_HammingDist(Index *index, char *token, unsigned int match_dist, Query query, int token_index) {
    ErrorCode ec;
    pthread_mutex_lock(&(job_scheduler.hd_mtx));
    ec = insert_info_payload(insert_HammingTree(index->HammingDist, token), match_dist-1, query, token_index);
    pthread_mutex_unlock(&(job_scheduler.hd_mtx));

    return ec;
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
	destroy_scheduler(&job_scheduler);
    if (destroy_BK_tree(&(index->EditDist)) == EC_FAIL) return EC_FAIL;
    else if (destroy_HammingTree(index->HammingDist) == EC_FAIL) return EC_FAIL;
    else return EC_SUCCESS;
}
