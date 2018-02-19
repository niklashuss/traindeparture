#include "jobmanager.h"
#include <memory.h>
#include <stdio.h>


static const int MAX_JOB_COUNT = 5;

Job jobs[MAX_JOB_COUNT];

struct Queue {
    int first_free = 0;
    int last_free = 0;
} queue;

void jm_setup() {
    memset(&jobs[0], 0, sizeof(jobs));
    queue.first_free = 0;
    queue.last_free = MAX_JOB_COUNT - 1;
}

static inline int get_next(int x) {
    return (x + 1) % MAX_JOB_COUNT;
}
Job* jm_get_job() {
    int job_index = get_next(queue.last_free);
    if (job_index == queue.first_free) {
        printf("Empty queue.\n");
        return nullptr;
    }
    Job* job = &jobs[job_index];
    return job;
}

bool jm_add_job(int id, JobFunc_t func, void* data) {
    if (queue.first_free == queue.last_free) {
        printf("Warning: job queue is full\n");
        return false;
    }
    jobs[queue.first_free].id = id;
    jobs[queue.first_free].func = func;
    jobs[queue.first_free].data = data;
    queue.first_free = get_next(queue.first_free);

    return true;
}

void jm_job_done() {
    queue.last_free = get_next(queue.last_free);
}

