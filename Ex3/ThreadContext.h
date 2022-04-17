#ifndef EX3_THREADCONTEXT_H
#define EX3_THREADCONTEXT_H
#include "JobContext.h"

class JobContext;

/**
 * This class represents a thread's context, and consists of all necessary information about the thread.
 */
class ThreadContext{
public:

    /**
     * A default constructor, used to create databases of thread contexts.
     */
    ThreadContext();

    /**
     * A constructor, receiving Desired thread's ID and the job context, used to create a single thread context.
     */

    /**
     * A constructor used to create a single thread context.
     * @param thread_id Thread's ID.
     * @param job_context The job context.
     */
    ThreadContext(int thread_id, JobContext* job_context);
    
    int thread_id; // ID of thread.
    JobContext* job_context; // job context.
    IntermediateVec intermediate_vec; // Vector consisting of unfinished data. on which the thread is working on.
};

#endif //EX3_THREADCONTEXT_H
