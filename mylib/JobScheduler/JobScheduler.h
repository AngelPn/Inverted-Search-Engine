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
    pthread_mutex_t candidate_queries_mtx;
    pthread_mutex_t matched_queries_mtx;
    pthread_cond_t nonempty;
    pthread_barrier_t barrier;
    bool quit;
} JobScheduler;

void initialize_scheduler(JobScheduler *js, int execution_threads);
int submit_job(JobScheduler* js, Job* j);
int execute_all_jobs(JobScheduler* js);
int wait_all_tasks_finish(JobScheduler* js);
int destroy_scheduler(JobScheduler* js);

#endif