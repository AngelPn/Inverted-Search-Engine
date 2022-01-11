#include <stdio.h>
#include <stdlib.h>

#include "Job.h"
#include "HammingTree.h"
#include "common_types.h"

struct job_struct{
    /*void (*func)(void*);*/
    void* args[4];
    job_type jobType;
};

job_type get_job_type(Job j){
    return j->jobType;
}

Job create_job(job_type jt, void* args[4]) {
    Job j = (Job)malloc(sizeof(struct job_struct));
    j->jobType = jt;
    for(int i = 0; i < 4; i++){
        j->args[i] = args[i];
    }
    return j;
}

void run(Job j) {
    switch(j->jobType){
        case MATCH_DOCUMENT:
        {
            MatchDocument_job(j->args);
            break;
        }
        case START_Q:
        {
            StartQuery_job(j->args);
            break;
        }
    }
}

ErrorCode destroy_job(void **j) {
    Job jj = *j;
    free(jj);
    *j = NULL;
    return EC_SUCCESS;
}
