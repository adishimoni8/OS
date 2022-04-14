//
// Created by Bengo on 6/5/2021.
//

#include <cstdlib>
#include <pthread.h>
#include <atomic>
#include <semaphore.h>

#include "ContextClasses.h"
#include "ErrorHandler.h"

ThreadContext::ThreadContext(int threadId, JobContext *jobContext) : threadID(threadId), numOfReducedPairs(0),
                                                                     jobContext(jobContext)
{
}
JobContext::~JobContext() {
    delete[] threads;
    delete[] contexts;
    delete barrier;
    sem_destroy(&postShuffle);
    pthread_mutex_destroy(&shuffledVecLock);
    pthread_mutex_destroy(&outputVecLock);
}

ThreadContext::ThreadContext() : threadID(-1), numOfReducedPairs(0),
                                 jobContext(nullptr)
{
}

JobContext::JobContext(const MapReduceClient &client, const InputVec &inputVec, OutputVec
&outputVec, int numOfThreads) : numOfThreads(numOfThreads), client(client),
                                inputVec(inputVec), outputVec(outputVec),
                                atomicCounter(0), numOfPairs(0), postShuffle{},
                                jobIsWaiting(0), jobFinished(0), elementsToShuffle(0),
                                barrier{}, outputVecLock{}  {
    threads = new(std::nothrow) pthread_t[numOfThreads];
    if (threads == nullptr) {
        printError(MEMORY_ERROR);
        exit(1);
    }
    contexts = new(std::nothrow) ThreadContext[numOfThreads];
    if (contexts == nullptr) {
        printError(MEMORY_ERROR);
        delete[] threads;
        exit(1);
    }
    jobIsWaiting = false;

    if (sem_init(&postShuffle, 0, 0)) {
        printError(SEM_INIT_ERROR);
        delete[] threads;
        delete[] contexts;
        exit(1);
    }
    if (pthread_mutex_init(&outputVecLock, nullptr)) {
        printError(MUTEX_INIT_ERROR);
        delete[] threads;
        delete[] contexts;
        sem_destroy(&postShuffle);
        exit(1);
    }
    if (pthread_mutex_init(&shuffledVecLock, nullptr)) {
        printError(MUTEX_INIT_ERROR);
        delete[] threads;
        delete[] contexts;
        sem_destroy(&postShuffle);
        pthread_mutex_destroy(&outputVecLock);
        exit(1);
    }
    barrier = new(std::nothrow) Barrier(numOfThreads);
    if (barrier == nullptr) {
        printError(MEMORY_ERROR);
        delete[] threads;
        delete[] contexts;
        sem_destroy(&postShuffle);
        pthread_mutex_destroy(&outputVecLock);
        pthread_mutex_destroy(&shuffledVecLock);
    }
}

