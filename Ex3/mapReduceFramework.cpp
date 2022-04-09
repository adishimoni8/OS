#include <pthread.h>
#include <atomic>
#include <algorithm>
#include "Barrier.h"
#include "mapReduceFramework.h"

// ================ Declarations of Data Structures ======

struct ThreadContext;
struct JobContext;

// ================ Data Structures ======================

typedef struct {
    int threadID;
    std::atomic<int>* split_counter;
    std::vector<IntermediatePair> IntermediateVec;
    JobContext* job_context;
} ThreadContext;

typedef struct{
    const MapReduceClient& client;
    const InputVec& inputVec;
    OutputVec& outputVec;
    int multiThreadLevel;
    JobState state;
    pthread_t threads[];
    ThreadContext thread_contexts[];
    Barrier* barrier;
} JobContext;

// =================== Declarations of helpers ============
static void map_phase(ThreadContext* tc);
static void initiate_threads(pthread_t*, int, Barrier, ThreadContext);
static void* thread_entry(void* arg);
// =================== API ================================

JobHandle startMapReduceJob(const MapReduceClient& client,
                            const InputVec& inputVec, OutputVec& outputVec,
                            int multiThreadLevel){

    JobContext job = { client, inputVec, outputVec, multiThreadLevel,
                               UNDEFINED_STAGE, threads[multiThreadLevel],
                               contexts[multiThreadLevel], barrier(multiThreadLevel)};
    initiate_threads(&job);
    std::atomic<int> atomic_counter(0);
    for (int i = 0; i < MT_LEVEL; ++i) {
        pthread_join(threads[i], NULL);
    }
    return static_cast<JobHandle> (job);
}

// =================== Utils ============================

static void initiate_threads(JobContext* job){
    for (int i = 0; i < job->multiThreadLevel; ++i) {
        job->contexts[i] = {i, job};
    }
    for (int i = 0; i < job->multiThreadLevel; ++i) {
        pthread_create(&(job->threads[i]), nullptr, thread_entry, &(job->contexts[i]))
    }
}

static void* thread_entry(void* arg){
    auto* tc = (ThreadContext*) arg;
    map_phase(tc);
    tc->barrier->barrier();
};

static void map_phase(ThreadContext* tc){
//    int old_value = (*(tc->split_counter))++;
//    while(old_value < (tc->inputVec).size()){
//        tc->client.map(tc->inputVec[old_value].first, tc->inputVec[old_value].second, &(tc->IntermediateVec));
//        old_value = (*(tc->split_counter))++;
//    }
//    std::sort(tc->inputVec.begin(), tc->inputVec.end());
}