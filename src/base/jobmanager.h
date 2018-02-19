#pragma once

typedef void (*JobFunc_t)(void*);

struct Job {
    int id;
    JobFunc_t func;
    void* data;
};

void jm_setup();
bool jm_add_job(int id, JobFunc_t func, void* data);
Job* jm_get_job();
void jm_job_done();
