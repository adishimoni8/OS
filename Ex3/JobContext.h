#ifndef OS_EX3_CONTEXTCLASSES_H
#define OS_EX3_CONTEXTCLASSES_H

#include <pthread.h>
#include <atomic>
#include <semaphore.h>
#include "ThreadContext.h"
#include "MapReduceFramework.h"
#include "Barrier.h"

class ThreadContext;

/**
 * This class represents a job's context, and consists of all necessary information about the process.
 */
class JobContext
{
public:

    /**
     * A destructor.
     */
    ~JobContext();

    /**
     * A constructor used to create a single JobContext instance.
     * @param client demands from user.
     * @param input_vec Vector of input data.
     * @param output_vec Vector to store output data.
     * @param multi_thread_level Amount of threads.
     */
    JobContext(const MapReduceClient &client, const InputVec &input_vec, OutputVec &output_vec,
               int multi_thread_level);

    const MapReduceClient &client; //An object representing the DB and demands given by user
    const InputVec &input_vec; // Input vector consisting of elements given by user.
    OutputVec &output_vec;// Vector for output data.
    int multi_thread_level; // Amount of threads to run during the process.
    ThreadContext* contexts; // Vector consisting of thread's contexts.
    pthread_t *threads; // Vector of threads.
    std::atomic<uint64_t> atomic_var; // 2 bits stage - 31 bits keys to process - 31 bits already processed.
    Barrier *barrier; // A barrier object, manipulates and controls the threads at required times.
    sem_t shuffle_sem; // A semaphore used for the shuffling phase.
    std::vector<std::vector<IntermediatePair>> shuffle_vec; // vector of shuffled elements.

};


#endif //OS_EX3_CONTEXTCLASSES_H
