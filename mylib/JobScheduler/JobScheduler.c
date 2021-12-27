#include <pthread.h>
#include <stdio.h>

#include "JobScheduler.h"
#include "LinkedList.h"
#include "Job.h"

typedef struct JobScheduler{
    int execution_threads;// number of execution threads
    LinkedList jobs; // a queue that holds submitted jobs / tasks
    p_thread_t* tids; // execution threads
    pthread_mutex_t job_mtx;
    pthread_mutex_t candidate_queries_mtx;
    pthread_mutex_t matched_queries_mtx;
    pthread_cond_t nonempty;
    bool quit;
} JobScheduler;

JobScheduler* initialize_scheduler(int execution_threads){
    int err;
    JobScheduler* js = malloc(sizeof(struct JobScheduler));
    js->execution_threads = execution_threads;
    create_list(&(js->q), destroy_job);
    js->quit = false;
    js->tids = malloc(sizeof(p_thread_t) * execution_threads);
    js->job_mtx = PTHREAD_MUTEX_INITIALIZER;
    js->candidate_queries_mtx = PTHREAD_MUTEX_INITIALIZER;
    js->matched_queries_mtx = PTHREAD_MUTEX_INITIALIZER;
    js->nonempty = PTHREAD_COND_INITIALIZER;
    for(int i = 0;i < js->execution_threads;i++){
        if ((err = pthread_create(js->tids[i], NULL, thread_code, js))) {
            /* Create a thread */
            perror("pthread_create:", err);
            exit(1);
        }
    }
    return js;
}

void* thread_code(JobScheduler* js){
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
    }
}

int submit_job(JobScheduler* sch, Job* j){
    pthread_mutex_lock(&(sch->job_mtx));
    ErrorCode err = add_item(sch->jobs, j);
    pthread_cond_signal(&(sch->nonempty), &(sch->job_mtx));
    pthread_mutex_unlock(&(sch->job_mtx));
    return err;
}

int execute_all_jobs(JobScheduler* sch){
    return 0;
}

int wait_all_tasks_finish(JobScheduler* sch){
    return 0;
}

int destroy_scheduler(JobScheduler* sch){
    js->quit = true;
    for (int i=0 ; i<NUM_THREADS ; i++) {
        if ((err = pthread_join(sch->tids[i], NULL))) {
            /* Wait for thread termination */
            perror("pthread_join:", err);
            exit(1);
        }
    }
    pthread_cond_destroy(sch->nonempty);
    pthread_mutex_destroy(sch->job_mtx);
    pthread_mutex_destroy(sch->candidate_queries_mtx);
    pthread_mutex_destroy(sch->matched_queries_mtx);
    free(sch->tids);
    destroy_list(&(sch->jobs));
    free(sch);
    sch = NULL;
    return 0;
}
