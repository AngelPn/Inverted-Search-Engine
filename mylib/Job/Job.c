#include <stdio.h>

#include "Job.h"
#include "core.h"
#include "HashTable.h"

struct Job{
    /*void (*func)(void*);*/
    void* args[5];
    job_type jobType;
};

Job create_job(job_type jt, void* args[5]){
    Job j = malloc(sizeof(struct Job));
    for(int i = 0;i < 5;i++){
        j->args[i] = args[i];
    }
    return j;
}

void run(Job j){
    switch(j->jobType){
        case HashT_get:
            HashT_get(j->args[0],j->args[1]);
            break;
        case lookup_BKtree:
            lookup_BKtree(j->args[0],j->args[1],j->args[2],j->args[3],j->args[4]);
            break;
        case lookup_HammingTree:
            lookup_HammingTree(j->args[0],j->args[1],j->args[2],j->args[3],j->args[4]);
            break;
    }
}

void destroy_job(void* j){
    free(j);
}
