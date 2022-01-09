#ifndef JOB_H
#define JOB_H

#include "core.h"

typedef enum {
    MATCH_DOCUMENT,
    START_Q
} job_type;

typedef struct job_struct *Job;

Job create_job(job_type jt, void* args[4]);
void run(Job j);
ErrorCode destroy_job(void **j);

#endif
