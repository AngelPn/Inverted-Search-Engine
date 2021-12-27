#ifndef INVERTED_SEARCH_ENGINE_JOBSCHEDULER_H
#define INVERTED_SEARCH_ENGINE_JOBSCHEDULER_H

JobScheduler* initialize_scheduler(int execution_threads);
int submit_job(JobScheduler* sch, Job* j);
int execute_all_jobs(JobScheduler* sch);
int wait_all_tasks_finish(JobScheduler* sch);
int destroy_scheduler(JobScheduler* sch);

#endif
