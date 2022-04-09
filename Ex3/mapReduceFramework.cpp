#include <pthread.h>
#include <atomic>
#include <algorithm>
#include "Barrier.h"
#include "mapReduceFramework.h"
#include "MapReduceClient.h"

// ================ Declarations of Data Structures ======

class JobContext;
class ThreadContext;

// ================ Data Structures ======================

class ThreadContext{
public:
    int threadID{};
    JobContext* job_context{};
    std::atomic<int>* split_counter{};
    std::vector<IntermediatePair> IntermediateVec;
};

class JobContext{
public:
    MapReduceClient& client;
    InputVec& inputVec;
    OutputVec& outputVec;
    int multiThreadLevel;
    JobState state;
    pthread_t* threads;
    ThreadContext* thread_contexts;
    Barrier* barrier;
    JobContext(MapReduceClient& client, InputVec& inputVec, OutputVec& outputVec, int multiThreadLevel, JobState state,
               pthread_t* threads, ThreadContext* thread_contexts, Barrier* barrier):client(client), inputVec(inputVec),
               outputVec(outputVec), multiThreadLevel(multiThreadLevel), state(state), threads(threads),
               thread_contexts(thread_contexts), barrier(barrier){};
};

// =================== Declarations of helpers ============
static void map_phase(ThreadContext* tc);
static void initiate_threads(JobContext*);
static void* thread_entry(void* arg);
// =================== API ================================

JobHandle startMapReduceJob(const MapReduceClient& client,
                            const InputVec& inputVec, OutputVec& outputVec,
                            int multiThreadLevel){
    pthread_t threads[multiThreadLevel];
    ThreadContext contexts[multiThreadLevel];
    Barrier barrier(multiThreadLevel);
    JobContext job = new JobContext(client, inputVec, outputVec, multiThreadLevel,
                      UNDEFINED_STAGE, &threads,
                      &contexts, &barrier);
    initiate_threads(&job);
    std::atomic<int> atomic_counter(0);
    for (int i = 0; i < multiThreadLevel; ++i) {
        pthread_join(threads[i], nullptr);
    }
    return static_cast<JobHandle> (job);
}

// =================== Utils ============================

static void initiate_threads(JobContext* job){
    for (int i = 0; i < job->multiThreadLevel; ++i) {
        job->thread_contexts[i] = {i, job};
    }
    for (int i = 0; i < job->multiThreadLevel; ++i) {
        pthread_create(&(job->threads[i]), nullptr, thread_entry, &(job->thread_contexts[i]));
    }
}

static void* thread_entry(void* arg){
    auto* tc = (ThreadContext*) arg;
    map_phase(tc);
    tc->job_context->barrier->barrier();
};

static void map_phase(ThreadContext* tc){
    JobContext* job_context = tc->job_context;
    int old_value = (*(tc->split_counter))++;
    while(old_value < (job_context->inputVec).size()){
        job_context->client.map(job_context->inputVec[old_value].first, job_context->inputVec[old_value].second, &(tc->IntermediateVec));
        old_value = (*(tc->split_counter))++;
    }
    std::sort(job_context->inputVec.begin(), job_context->inputVec.end());
}

int main() {
    std::cin << "hi";
}