#include <stdio.h>
#include <stdlib.h>

#include "Job.h"
#include "HammingTree.h"

struct job_struct{
    void* args[4];
    job_type jobType;
};

Job create_job(job_type jt, void* args[4]) {
    Job j = (Job)malloc(sizeof(struct job_struct));
    j->jobType = jt;
    for(int i = 0; i < 4; i++){
        j->args[i] = args[i];
    }
    return j;
}

void run(Job j) {
    MatchDocument_job(j->args);
}

ErrorCode destroy_job(void **j) {
    Job jj = *j;
    free(jj);
    *j = NULL;
    return EC_SUCCESS;
}
