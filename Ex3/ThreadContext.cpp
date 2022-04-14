#include "ThreadContext.h"

ThreadContext::ThreadContext() : thread_id(-1), job_context(nullptr) {};

ThreadContext::ThreadContext(int thread_id, JobContext* job_context)
                            :thread_id(thread_id), job_context(job_context) {}