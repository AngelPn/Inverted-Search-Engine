#include <stdio.h>
#include <stdlib.h>

#include "JobScheduler.h"

void* thread_code(void *arg);

void initialize_scheduler(JobScheduler *js, int execution_threads) {
    js->execution_threads = execution_threads;
    create_list(&(js->jobs), destroy_job);
    js->quit = false;
    js->job_counter = 0;
    js->tids = malloc(sizeof(pthread_t) * execution_threads);

    js->job_mtx = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
    js->queries_mtx = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;

    js->nonempty = (pthread_cond_t)PTHREAD_COND_INITIALIZER;
    js->empty = (pthread_cond_t)PTHREAD_COND_INITIALIZER;
    js->job_type_cond[0] = (pthread_cond_t)PTHREAD_COND_INITIALIZER;
    js->job_type_cond[1] = (pthread_cond_t)PTHREAD_COND_INITIALIZER;
    
    js->em_mtx = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
    js->ed_mtx = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
    js->hd_mtx = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;

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

        while((get_number_items(js->jobs) == 0) && !(js->quit)) {
            pthread_cond_wait(&(js->nonempty), &(js->job_mtx)); //if jobs Q is empty wait for the non-empty signal
        }
        if(js->quit){
            pthread_mutex_unlock(&(js->job_mtx));
            break;
        }
        job = pop_item(js->jobs);

        pthread_mutex_unlock(&(js->job_mtx));

        run(job);

        // printf("before lock job count mtx in thread code\n");
        pthread_mutex_lock(&(js->job_mtx));
        (js->job_counter)--;
        (js->job_type_counter[get_job_type(job)])--;
        // printf("THREAD CODE job_counter: %d\n", js->job_counter);
        if (js->job_counter == 0) {
            pthread_cond_signal(&(js->empty));
        }
        if (js->job_type_counter[get_job_type(job)] == 0) {
            pthread_cond_signal(&(js->job_type_cond[get_job_type(job)]));
        }
        pthread_mutex_unlock(&(js->job_mtx));
        
        destroy_job((void**)(&job));
    }

    return NULL;
}

int submit_job(JobScheduler* js, Job* j){

    pthread_mutex_lock(&(js->job_mtx));
    ErrorCode err = add_item(js->jobs, *j);

    // printf("job count mtx is locked\n");
    // pthread_mutex_lock(&(js->job_count_mtx));
    (js->job_counter)++;
    (js->job_type_counter[get_job_type(*j)])++;
    
    // printf("SUB job_counter: %d\n", js->job_counter);
    // pthread_mutex_unlock(&(js->job_count_mtx));

    pthread_cond_signal(&(js->nonempty));
    pthread_mutex_unlock(&(js->job_mtx));

    return err;
}

int execute_all_jobs(JobScheduler* js){
    return 0;
}

int wait_all_jobs_finish(JobScheduler* js){
    // printf("before lock job count mtx in wait\n");
    pthread_mutex_lock(&(js->job_mtx));
    while (js->job_counter != 0) {
        pthread_cond_wait(&(js->empty), &(js->job_mtx));
    }
    pthread_mutex_unlock(&(js->job_mtx));
    // printf("WAIT! job counter = 0\n");
    return 1;
}

int wait_match_document_jobs_finish(JobScheduler* js){
    // printf("before lock job count mtx in wait\n");
    pthread_mutex_lock(&(js->job_mtx));
    while (js->job_type_counter[MATCH_DOCUMENT] != 0) {
        pthread_cond_wait(&(js->job_type_cond[MATCH_DOCUMENT]), &(js->job_mtx));
    }
    pthread_mutex_unlock(&(js->job_mtx));
    // printf("WAIT! job counter = 0\n");
    return 1;
}

int wait_insert_index_jobs_finish(JobScheduler* js){
    // printf("before lock job count mtx in wait\n");
    pthread_mutex_lock(&(js->job_mtx));
    while (js->job_type_counter[START_Q] != 0) {
        pthread_cond_wait(&(js->job_type_cond[START_Q]), &(js->job_mtx));
    }
    pthread_mutex_unlock(&(js->job_mtx));
    // printf("WAIT! job counter = 0\n");
    return 1;
}

int destroy_scheduler(JobScheduler* js){
    js->quit = true;
    pthread_mutex_lock(&(js->job_mtx));
    pthread_cond_broadcast(&(js->nonempty));
    pthread_mutex_unlock(&(js->job_mtx));
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
    pthread_mutex_destroy(&(js->queries_mtx));

    free(js->tids);
    destroy_list(&(js->jobs));
    return 0;
}