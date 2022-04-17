#include <pthread.h>
#include <algorithm>
#include "mapReduceFramework.h"
#include "MapReduceClient.h"
#include "JobContext.h"
#include "ThreadContext.h"
#include "Exception.h"
#include <unistd.h>


// In this file we refer to job context as jc, and thread context as tc

// ========Macros to deal with the atomic counter ========

#define GET_KEYS_TO_PROCESS(ATOMIC) (ATOMIC & 0x7fffffff)
#define SET_KEYS_TO_PROCESS(ATOMIC, NUM) (ATOMIC & (~(0x7fffffff))) + NUM
#define GET_ALREADY_PROCESSED(ATOMIC) (ATOMIC & (0x7fffffff))
#define GET_STAGE(ATOMIC) ((uint64_t) ATOMIC >> 62))
#define SET_STAGE(ATOMIC) ((uint64_t) ATOMIC << 62)

// =================== Declarations of helpers ===========
static void* map_phase(void*);
static void* shuffle_phase(void*);
static void* reduce_phase(void*);
static void initiate_threads(JobContext*);
static void* thread_entry(void*);
static K2* find_maximum_key(JobContext*);
static IntermediateVec make_specific_key_vec(JobContext*, K2*);
bool compareIntermediatePair(IntermediatePair, IntermediatePair);
// =================== API ================================

JobHandle startMapReduceJob(const MapReduceClient& client,
                            const InputVec& inputVec, OutputVec& outputVec,
                            int multiThreadLevel){
    // Create the job:
    auto *jc = new(std::nothrow) JobContext(client, inputVec, outputVec, multiThreadLevel);
    if (!jc) {
        print_error_and_exit(MEM_ERROR);
    }
    jc->atomic_var = ((uint64_t)MAP_STAGE << 62);
    initiate_threads(jc);
    return static_cast<JobHandle> (jc);
}

/**
 * Recieves a given pair and adds them to the framework databases.
 * @param key Key of given pair.
 * @param value Value of given pair.
 * @param context context
 */
void emit2(K2 *key, V2 *value, void *context) {
    auto *tc =  static_cast<ThreadContext *> (context);
    IntermediatePair pair = IntermediatePair(key, value);
    tc->intermediate_vec.push_back(pair);
}

// =================== Utils ============================

/**
 * The process each thread performs during the runtime of the program.
 * @param context Context of job.
 * @return //todo change.
 */
static void* thread_entry(void* context){
    auto *tc = static_cast<ThreadContext *> (context);
    auto *jc = tc->job_context;
    map_phase(tc);
    tc->job_context->barrier->barrier();
    if (tc->thread_id == 0) {
        jc->atomic_var = ((uint64_t)SHUFFLE_STAGE << 62);
        shuffle_phase(tc);
        for (int i = 0; i < jc->multi_thread_level; ++i) {
            if (sem_post(&jc->shuffle_sem)) {
                exit(1);
            }
        }
    } else {
        if (sem_wait(&jc->shuffle_sem)) {
            exit(1);
        }
    }
    //reduce_phase(context);
    return nullptr;
}

/**
 * Creates new threads, according to orders found in the job context.
 * @param jc job context to receive orders from and update the new threads into.
 */
static void initiate_threads(JobContext* jc){
    int num_of_threads = jc->multi_thread_level;
    for (int i = 0; i < num_of_threads; ++i) { //saves the context of each thread.
        jc->contexts[i] = ThreadContext(i, jc);
    }
    for (int i = 0; i < num_of_threads; ++i) {
        if (pthread_create(&(jc->threads[i]), nullptr, thread_entry, &(jc->contexts[i]))){
            print_error_and_exit(PTHREAD_LIB_ERROR);
        }
    }
}

/**
 * Performs the map phase of the program, ran by the threads individually.
 * @param context Context of a thread.
 * @return //todo change.
 */
static void *map_phase(void* context) {
    auto *tc = static_cast<ThreadContext *> (context);
    JobContext *jc = tc->job_context;
    int num_of_elements = (int) jc->input_vec.size();
    int old_value = GET_ALREADY_PROCESSED(jc->atomic_var++);
    while (old_value < num_of_elements) {
        jc->client.map(jc->input_vec[old_value].first, jc->input_vec[old_value].second, tc);
        old_value = GET_ALREADY_PROCESSED(jc->atomic_var++);
    }
    std::sort(tc->intermediate_vec.begin(), tc->intermediate_vec.end(), compareIntermediatePair);
    return nullptr;
}

/**
 * Compares between 2 pairs, used to sort the vectors.
 * @param first_pair first pair.
 * @param second_pair second pair.
 * @return
 */
bool compareIntermediatePair(IntermediatePair first_pair, IntermediatePair second_pair) {
    return ((*first_pair.first) < (*second_pair.first));
}

/**
 * Performs the shuffle phase.
 * @param context Context of a thread.
 * @return //todo change.
 */
static void* shuffle_phase(void *context){
    auto *tc = static_cast<ThreadContext *> (context);
    JobContext *jc = tc->job_context;
    K2* maximum_key = find_maximum_key(jc);
    while (maximum_key != nullptr){
        IntermediateVec specific_key_vec = make_specific_key_vec(jc, maximum_key);
        jc->shuffle_vec.push_back(specific_key_vec);
        jc->atomic_var += (specific_key_vec.size() << 31);
        maximum_key = find_maximum_key(jc);
    }
    for (auto & i : jc->shuffle_vec){
        for (auto & j : i){
            std::cout << j.first << " " << j.second << "!" << std::endl;
        }
    }
    return nullptr;
}

/**
 * Finds maximum key.
 * @param jc job context.
 * @return
 */
static K2* find_maximum_key(JobContext* jc){
    K2* maximum_key = nullptr;
    for (int i = 0; i < jc->multi_thread_level; ++i) {
        if (jc->contexts[i].intermediate_vec.empty()) {
            continue;
        }
        if ((maximum_key == nullptr) || (*maximum_key) < (*(jc->contexts[i].intermediate_vec.front().first))) {
            maximum_key = jc->contexts[i].intermediate_vec.front().first;
        }
    }
    return maximum_key;
}

/**
 * Creates a vector for every intermediate type key.
 * @param jc job context.
 * @param maximum_key the maximum key
 * @return A vector containing all pairs relevant to specific key.
 */
static IntermediateVec make_specific_key_vec(JobContext* jc, K2* maximum_key){
    IntermediateVec specific_key_vec;
    for (int i = 0; i < jc->multi_thread_level; ++i) {
        while ((!jc->contexts[i].intermediate_vec.empty())) {
            if (!(((*maximum_key) < (*(jc->contexts[i].intermediate_vec.front().first))) & // If keys are equal
                    (*(jc->contexts[i].intermediate_vec.front().first)) < (*maximum_key))) {
                specific_key_vec.push_back(jc->contexts[i].intermediate_vec.front());
                jc->contexts[i].intermediate_vec.erase(jc->contexts[i].intermediate_vec.begin());
            }
        }
    }
    return specific_key_vec;
}

//static void *reduce_phase(void* context) {
//    auto *tc = static_cast<ThreadContext *> (context);
//    JobContext *jc = tc->job_context;
//    return nullptr;
//}


