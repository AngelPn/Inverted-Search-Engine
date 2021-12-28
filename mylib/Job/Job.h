#ifndef JOB_H
#define JOB_H

#include "core.h"

typedef enum {
    HASH_TABLE_GET,
    LOOKUP_BKTREE,
    LOOKUP_HAMMING_TREE
} job_type;

typedef struct job_struct *Job;

void run(Job j);

ErrorCode destroy_job(void **j);

#endif
