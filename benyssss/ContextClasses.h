#ifndef OS_EX3_CONTEXTCLASSES_H
#define OS_EX3_CONTEXTCLASSES_H

#include <pthread.h>
#include <atomic>
#include <semaphore.h>

#include "MapReduceFramework.h"

#include "Barrier.h"

typedef std::vector<std::vector<IntermediatePair>> ShuffleVector;
class JobContext;

class ThreadContext
{

public:
    int threadID;
    IntermediateVec intermediateVec;
    size_t numOfReducedPairs;
    JobContext *jobContext;
    ThreadContext(int threadId, JobContext *jobContext);
    ThreadContext();
};

class JobContext
{
public:
    ~JobContext();

    JobContext(const MapReduceClient &client, const InputVec &inputVec, OutputVec &outputVec,
               int numOfThreads);

    size_t numOfThreads;
    const MapReduceClient &client;
    const InputVec &inputVec;
    OutputVec &outputVec;
    pthread_t *threads;

    // 2 MSB bits are saved for the stage, 31 middle bits for total number of intermediate elements
    // 31 LSB are saved for the number of already processed keys.
    std::atomic<uint64_t> atomicCounter;
    int numOfPairs;
    sem_t postShuffle;
    std::atomic<int> jobIsWaiting;
    std::atomic<int> jobFinished;
    std::atomic<int> elementsToShuffle;
    Barrier *barrier;
    ThreadContext *contexts;

    pthread_mutex_t outputVecLock;
    pthread_mutex_t shuffledVecLock;
    ShuffleVector shuffleVector;

};


#endif //OS_EX3_CONTEXTCLASSES_H
