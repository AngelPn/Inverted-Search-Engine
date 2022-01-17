#ifndef JOBSCHEDULER_H
#define JOBSCHEDULER_H

#include <pthread.h>
#include <stdbool.h>

#include "LinkedList.h"
#include "Job.h"

typedef struct job_scheduler
{
    int execution_threads;// number of execution threads
    LinkedList jobs; // a queue that holds submitted jobs / tasks
    pthread_t* tids; // execution threads

    pthread_mutex_t job_mtx;
    pthread_mutex_t queries_mtx;

    pthread_mutex_t em_mtx;
    pthread_mutex_t ed_mtx;
    pthread_mutex_t hd_mtx;

    pthread_cond_t nonempty;
    pthread_cond_t empty;
    pthread_cond_t job_type_cond[2];

    int job_counter;
    int job_type_counter[2];
    bool quit;
} JobScheduler;

void initialize_scheduler(JobScheduler *js, int execution_threads);
int submit_job(JobScheduler* js, Job* j);
int execute_all_jobs(JobScheduler* js);
int wait_all_jobs_finish(JobScheduler* js);
int wait_match_document_jobs_finish(JobScheduler* js);
int wait_insert_index_jobs_finish(JobScheduler* js);
int destroy_scheduler(JobScheduler* js);

#endif