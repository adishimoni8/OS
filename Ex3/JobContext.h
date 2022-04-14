#ifndef OS_EX3_CONTEXTCLASSES_H
#define OS_EX3_CONTEXTCLASSES_H

#include <pthread.h>
#include <atomic>
#include <semaphore.h>
#include "ThreadContext.h"
#include "MapReduceFramework.h"
#include "Barrier.h"

class ThreadContext;

class JobContext
{
public:
    ~JobContext();

    JobContext(const MapReduceClient &client, const InputVec &input_vec, OutputVec &output_vec,
               int multi_thread_level);

    const MapReduceClient &client;
    const InputVec &input_vec;
    OutputVec &output_vec;
    int multi_thread_level;
    ThreadContext* contexts;
    pthread_t *threads;
    std::atomic<uint64_t> atomic_var; // 2 bits stage - 31 bits keys to process - 31 bits already processed.
    Barrier *barrier;
    sem_t shuffle_sem;
    std::vector<std::vector<IntermediatePair>> shuffle_vec;

};


#endif //OS_EX3_CONTEXTCLASSES_H
