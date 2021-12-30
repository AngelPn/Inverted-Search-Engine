#include <stdio.h>
#include <stdlib.h>

#include "JobScheduler.h"

void* thread_code(void *arg);

void initialize_scheduler(JobScheduler *js, int execution_threads) {
    js->execution_threads = execution_threads;
    create_list(&(js->jobs), destroy_job);
    js->quit = false;
    js->tids = malloc(sizeof(pthread_t) * execution_threads);
    js->job_mtx = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
    js->candidate_queries_mtx = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
    js->matched_queries_mtx = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
    js->nonempty = (pthread_cond_t)PTHREAD_COND_INITIALIZER;
    
    js->em_mtx = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
    js->ed_mtx = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
    js->hd_mtx = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
    
    pthread_barrier_init(&(js->barrier), NULL, 3);

    for (int i = 0; i < js->execution_threads; i++) {
        /* Create threads */
        int err;
        if ((err = pthread_create(&(js->tids[i]), NULL, thread_code, js))) {
            perror("pthread_create:");
            exit(1);
        }
    }
}

void* thread_code(void *arg){
    JobScheduler* js = arg;
    Job job = NULL;
    while(1){
        pthread_mutex_lock(&(js->job_mtx));
        while(get_number_items(js->jobs) == 0 && !(js->quit)) {
            pthread_cond_wait(&(js->nonempty), &(js->job_mtx)); //if jobs Q is empty wait for the non-empty signal
        }
        if(js->quit){
            pthread_mutex_unlock(&(js->job_mtx));
            break;
        }
        job = pop_item(js->jobs);

        pthread_mutex_unlock(&(js->job_mtx));
        run(job);
        destroy_job((void**)(&job)); //FIXME
    }

    return NULL;
}

int submit_job(JobScheduler* js, Job* j){
    pthread_mutex_lock(&(js->job_mtx));
    ErrorCode err = add_item(js->jobs, *j);
    pthread_cond_signal(&(js->nonempty));
    pthread_mutex_unlock(&(js->job_mtx));
    return err;
}

int execute_all_jobs(JobScheduler* js){
    return 0;
}

int wait_all_jobs_finish(JobScheduler* js){

    int n = get_number_items(js->jobs);


    return n;
}

int destroy_scheduler(JobScheduler* js){
    js->quit = true;
    pthread_cond_broadcast(&(js->nonempty));
    for (int i=0 ; i<NUM_THREADS ; i++) {
        int err;
        if ((err = pthread_join(js->tids[i], NULL))) {
            /* Wait for thread termination */
            perror("pthread_join:");
            exit(1);
        }
    }
    pthread_cond_destroy(&(js->nonempty));
    pthread_mutex_destroy(&(js->job_mtx));
    pthread_mutex_destroy(&(js->candidate_queries_mtx));
    pthread_mutex_destroy(&(js->matched_queries_mtx));
    
    pthread_barrier_destroy(&(js->barrier));

    free(js->tids);
    destroy_list(&(js->jobs));
    return 0;
}
