#ifndef EX3_THREADCONTEXT_H
#define EX3_THREADCONTEXT_H
#include "JobContext.h"

class JobContext;

class ThreadContext{
public:
    ThreadContext();
    ThreadContext(int, JobContext*);
    
    int thread_id;
    JobContext* job_context;
    IntermediateVec intermediate_vec;
};

#endif //EX3_THREADCONTEXT_H
