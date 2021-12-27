#ifndef INVERTED_SEARCH_ENGINE_JOB_H
#define INVERTED_SEARCH_ENGINE_JOB_H

typedef enum{
    HashT_get,
    lookup_BKtree,
    lookup_HammingTree
} job_type;

typedef struct Job* Job;

void destroy_job(void* j);

#endif
