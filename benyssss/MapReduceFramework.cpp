//
// Created by Bengo on 6/5/2021.
//

#include <cstdlib>
#include <algorithm>
#include <stdio.h> //TODO:REMOVE

#include <pthread.h>
#include <unistd.h>

#include "MapReduceFramework.h"
#include "ContextClasses.h"
#include "ErrorHandler.h"

#define SET_STAGE(STAGE) ((uint64_t) STAGE << 62)
// 31 bit mask
#define GET_31_LSB(COUNTER) (0x7fffffff & COUNTER) // 0b1111111111111111111111111111111
// get the middle 31 bits
#define GET_31_MIDDLE(COUNTER) ((COUNTER >> 31) & (0x7fffffff))

#define INC_31_MIDDLE (0x80000000)

#define KEY_EQUALS(A, B) (!((A < B) || (B < A)))

#define GET_STAGE(COUNTER) ((stage_t) (COUNTER >> 62))

#define QUOTIENT_TO_PERCENTAGE (100.0)

static void *doMap(void *context);

static void *doSort(void *context);

static void doShuffle(void * context);

static void *doReduce(void *context);

static void shuffleCounterSetup(void *context);

static void reduceCounterSetup(void *context);

static bool vecsNotEmpty(JobContext *jobContext);

static float calcPercentage(int a, int b);

bool compareFunc(IntermediatePair x, IntermediatePair y);

static float min(float a, float b);

static void *doWork(void *context) {
    auto *threadContext = static_cast<ThreadContext *> (context);
    JobContext *jobContext = threadContext->jobContext;
    doMap(context);
    doSort(context);
    // BARRIER
    jobContext->barrier->barrier();
    // all threads intermediate vectors are sorted.
    // start the SHUFFLE stage
    if (threadContext->threadID == 0) {
        // the total number of intermediate values is the count of values from the previous stage
        shuffleCounterSetup(context);
        doShuffle(context);
        reduceCounterSetup(context);
        for (size_t i = 0; i < jobContext->numOfThreads; ++i) {
            if (sem_post(&jobContext->postShuffle)) {
                printError(SEM_LOCK_UNLOCK_ERROR);
                exit(1);
            }
        }
    }
    else{
        if (sem_wait(&jobContext->postShuffle)) {
            printError(SEM_LOCK_UNLOCK_ERROR);
            exit(1);
        }
    }
    doReduce(context);
    return nullptr;
}

void emit2(K2 *key, V2 *value, void *context) {
    auto *threadContext = static_cast<ThreadContext *> (context);
    IntermediatePair pair = IntermediatePair(key, value);
    threadContext->intermediateVec.push_back(pair);
    threadContext->jobContext->elementsToShuffle++;
}

void emit3(K3 *key, V3 *value, void *context) {
    auto *threadContext = static_cast<ThreadContext *> (context);
    auto *jobContext = threadContext->jobContext;
    OutputPair pair = OutputPair(key, value);
    if (pthread_mutex_lock(&jobContext->outputVecLock) != 0) {
        printError(MUTEX_LOCK_UNLOCK_ERROR);
        exit(1);
    }
    jobContext->outputVec.push_back(pair);
    if (pthread_mutex_unlock(&jobContext->outputVecLock) != 0) {
        printError(MUTEX_LOCK_UNLOCK_ERROR);
        exit(1);
    }
    jobContext->atomicCounter+=(INC_31_MIDDLE);
}

static void *doMap(void *context) {
    auto *threadContext = static_cast<ThreadContext *> (context);
    JobContext *jobContext = threadContext->jobContext;
    unsigned long n = jobContext->inputVec.size();
    unsigned long oldValue = 0;
    while ((oldValue = GET_31_LSB(jobContext->atomicCounter++)) < n) {
        jobContext->client.map(jobContext->inputVec[oldValue].first,
                               jobContext->inputVec[oldValue].second,
                               threadContext);
        threadContext->jobContext->atomicCounter += INC_31_MIDDLE; // for shuffle stage
    }
    return nullptr;
}

static void *doSort(void *context) {
    auto *threadContext = static_cast<ThreadContext *> (context);
    std::sort(threadContext->intermediateVec.begin(), threadContext->intermediateVec.end(), compareFunc);
    return nullptr;
}

bool compareFunc(IntermediatePair x, IntermediatePair y) {
    return ((*x.first) < (*y.first));
}

static void shuffleCounterSetup(void *context){
    auto *threadContext = static_cast<ThreadContext *> (context);
    JobContext *jobContext = threadContext->jobContext;
    jobContext->numOfPairs = jobContext->elementsToShuffle; // emit2 counted this value
    jobContext->atomicCounter = SET_STAGE(SHUFFLE_STAGE);
}


static void reduceCounterSetup(void *context){
    auto *threadContext = static_cast<ThreadContext *> (context);
    JobContext *jobContext = threadContext->jobContext;
    jobContext->atomicCounter = SET_STAGE(REDUCE_STAGE);

}


static void doShuffle(void * context){
    auto *threadContext = static_cast<ThreadContext *> (context);
    JobContext *jobContext = threadContext->jobContext;
    while (vecsNotEmpty(jobContext)){
        K2 *maxKey = nullptr;
        for (size_t i = 0; i < jobContext->numOfThreads; ++i) {
            if (!jobContext->contexts[i].intermediateVec.empty()){
                if ((maxKey == nullptr) || (*maxKey) < *(jobContext->contexts[i].intermediateVec.back().first)) {
                    maxKey = jobContext->contexts[i].intermediateVec.back().first;
                }
            }
        }
        IntermediateVec uniqueKeyVec;
        for (size_t i = 0; i < jobContext->numOfThreads; ++i) {
            while ((!jobContext->contexts[i].intermediateVec.empty()) &&
                    (KEY_EQUALS((*maxKey), (*(jobContext->contexts[i].intermediateVec.back().first)) )))
            {
                uniqueKeyVec.push_back(jobContext->contexts[i].intermediateVec.back());
                jobContext->contexts[i].intermediateVec.pop_back();
            }
        }
        jobContext->shuffleVector.push_back(uniqueKeyVec);
        jobContext->atomicCounter += (uniqueKeyVec.size() << 31);
    }


}

static bool vecsNotEmpty(JobContext *jobContext) {
    for (size_t i = 0; i < jobContext->numOfThreads; ++i){
        if (!(jobContext->contexts[i].intermediateVec.empty()))
        {
            return true;
        }
    }
    return false;
}

void *doReduce(void *context) {

    auto *threadContext = static_cast<ThreadContext *> (context);
    JobContext *jobContext = threadContext->jobContext;
    IntermediateVec *vecToReducePointer = nullptr;
    unsigned long n = jobContext->shuffleVector.size();
    unsigned long oldValue = 0;
    while ((oldValue = GET_31_LSB(jobContext->atomicCounter++)) < n) {
        vecToReducePointer = &(jobContext->shuffleVector.at(oldValue));
        if (GET_31_MIDDLE(jobContext->atomicCounter) >= n)
        {
            break;
        }
        threadContext->numOfReducedPairs = vecToReducePointer->size();
        jobContext->client.reduce(vecToReducePointer, threadContext);
    }
    return nullptr;
}


JobHandle
startMapReduceJob(const MapReduceClient &client, const InputVec &inputVec, OutputVec &outputVec,
                  int multiThreadLevel) {
    JobContext *job = new(std::nothrow)
            JobContext(client, inputVec, outputVec, multiThreadLevel);
    if (job == nullptr) {
        printError(MEMORY_ERROR);
        exit(1);
    }
    for (int i = 0; i < multiThreadLevel; ++i) {
        job->contexts[i] = ThreadContext(i, job);
    }
    job->atomicCounter += SET_STAGE(MAP_STAGE);
    for (int i = 0; i < multiThreadLevel; ++i) {
        if (pthread_create(&(job->threads[i]), nullptr, doWork, &(job->contexts[i]))) {
            printError(THREAD_CREATION_ERROR);
            exit(1);
        }
    }
    return static_cast<JobHandle> (job);
}

void getJobState(JobHandle job, JobState *state) {
    auto *jobContext = static_cast<JobContext *> (job);
    uint64_t freezeCounter = jobContext->atomicCounter;
    state->stage = GET_STAGE(freezeCounter);
    switch (state->stage) {
        case UNDEFINED_STAGE:
            state->percentage = 0.0;
            return;
        case MAP_STAGE:
            state->percentage = min(calcPercentage(GET_31_MIDDLE(freezeCounter), jobContext->inputVec.size()), 100.0);
            return;
        case SHUFFLE_STAGE:
            state->percentage = min(calcPercentage(GET_31_MIDDLE(freezeCounter), jobContext->numOfPairs), 100.0);
            return;
        case REDUCE_STAGE:
            state->percentage = min(calcPercentage(GET_31_MIDDLE(freezeCounter), jobContext->shuffleVector.size()), 100.0);
            return;

    }

}

void waitForJob(JobHandle job) {
    auto *jobContext = static_cast<JobContext *> (job);
    int oldValue = jobContext->jobIsWaiting++; // guaranteed to be atomic
    if (oldValue) {
        return;
    }
    for (size_t i = 0; i < jobContext->numOfThreads; ++i) {
        if (pthread_join(jobContext->threads[i], nullptr)) {
            printError(THREAD_JOIN_ERROR);
            exit(1);
        }
    }
    jobContext->jobFinished++;
}

void closeJobHandle(JobHandle job) {
    auto *jobContext = static_cast<JobContext *> (job);
    waitForJob(job);
    while (!jobContext->jobFinished) {
        usleep(60);
    }
    delete jobContext;
}

static float min(float a, float b){
    if (a < b){return a;}
    return b;
}

static float calcPercentage(int a, int b){
    return (((float) a / (float) b) * QUOTIENT_TO_PERCENTAGE);
}