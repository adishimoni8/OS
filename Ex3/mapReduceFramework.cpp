#include <pthread.h>
#include <atomic>
#include <algorithm>
#include "Barrier.h"
#include "mapReduceFramework.h"

typedef struct {
    int threadID;
    Barrier* barrier;
    std::atomic<int>* split_counter;
    const MapReduceClient& client;
    const InputVec& inputVec;
    OutputVec& outputVec;
    std::vector<IntermediatePair> IntermediateVec;
} ThreadContext;

typedef struct{

    int multiThreadLevel;
    JobState state;
    pthread_t threads[];
} JobContext;

void map_phase(ThreadContext* tc){
    int old_value = (*(tc->split_counter))++;
    while(old_value < (tc->inputVec).size()){
        tc->client.map(tc->inputVec[old_value].first, tc->inputVec[old_value].second, &(tc->IntermediateVec));
        old_value = (*(tc->split_counter))++;
    }
    std::sort(tc->inputVec.begin(), tc->inputVec.end());
}

void* thread_entry(void* arg){
    auto* tc = (ThreadContext*) arg;
    map_phase(tc);
    tc->barrier->barrier();
    // Wake 1 of the threads.
};



void initiate_threads(pthread_t* threads, int multiThreadLevel, Barrier barrier, ThreadContext* contexts){
    for (int i = 0; i < multiThreadLevel; ++i) {
        contexts[i] = {i, &barrier};
    }
    for (int i = 0; i < multiThreadLevel; ++i) {
        pthread_create(threads + i, nullptr, foo, contexts + i);
    }
}

JobHandle startMapReduceJob(const MapReduceClient& client,
                            const InputVec& inputVec, OutputVec& outputVec,
                            int multiThreadLevel){

    Barrier barrier(multiThreadLevel);
    ThreadContext contexts[multiThreadLevel];
    pthread_t threads[multiThreadLevel];
    std::atomic<int> atomic_counter(0);
    initiate_threads(threads, multiThreadLevel, barrier, contexts);
    JobContext job_context = { client, inputVec, outputVec, multiThreadLevel, UNDEFINED_STAGE, &threads };
    for (int i = 0; i < MT_LEVEL; ++i) {
        pthread_join(threads[i], NULL);
    }

    // split the input vector

    return static_cast<JobHandle> (job_context);
}