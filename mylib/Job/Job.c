#include <stdio.h>
#include <stdlib.h>

#include "Job.h"
#include "HammingTree.h"

struct job_struct{
    /*void (*func)(void*);*/
    void* args[5];
    job_type jobType;
};

Job create_job(job_type jt, void* args[5]) {
    Job j = (Job)malloc(sizeof(struct job_struct));
    j->jobType = jt;
    for(int i = 0; i < 5; i++){
        j->args[i] = args[i];
    }
    return j;
}

void run(Job j) {
    switch(j->jobType){
        case HASH_TABLE_GET:
        {

            entry e = HashT_get(j->args[0], j->args[1]);
            if (e != NULL) {
                update_payload(e, 0, j->args[2], j->args[3]);
            }

            break;
        }
        case LOOKUP_BKTREE:
        {

            lookup_BKtree(j->args[0], j->args[1], 3, j->args[3], j->args[4]);

            break;
        }
        case LOOKUP_HAMMING_TREE:
        {

            lookup_HammingTree(j->args[0], j->args[1], 3, j->args[3], j->args[4]);

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
