#include <pthread.h>
#include <algorithm>
#include "MapReduceFramework.h"
#include "MapReduceClient.h"
#include "JobContext.h"
#include "ThreadContext.h"
#include "Exception.h"
#include <unistd.h>

// ======== Comments ======================================

// - In this file we refer to job context as jc, and thread context as tc

// ======== Macros to deal with the atomic counter ========

#define GET_KEYS_TO_PROCESS(ATOMIC) ((ATOMIC >> 31) & 0x7fffffff)
#define GET_ALREADY_PROCESSED(ATOMIC) (ATOMIC & 0x7fffffff)
#define GET_STAGE(ATOMIC) (ATOMIC >> 62)
#define SET_STAGE_AND_KEYS_PROCESS(STAGE, KEYS_TO_PROCESS) ((uint64_t) STAGE << 62 | \
(uint64_t) KEYS_TO_PROCESS << 31)
#define PERCENTAGES 100.0f

// =================== Declarations of helpers ===========

static void* map_phase(void*);
static void* shuffle_phase(void*);
static void* reduce_phase(void*);
static void initiate_threads(JobContext*);
static void* thread_entry(void*);
static K2* find_maximum_key(JobContext*);
static IntermediateVec make_specific_key_vec(JobContext*, K2*);
bool compareIntermediatePair(IntermediatePair, IntermediatePair);
static float get_percentage(uint64_t);
static int get_shuffle_keys_to_process(JobContext*);

// =================== API ================================

JobHandle startMapReduceJob(const MapReduceClient& client,
                            const InputVec& inputVec, OutputVec& outputVec,
                            int multiThreadLevel){
    // Create the job:
    auto *jc = new(std::nothrow) JobContext(client, inputVec, outputVec, multiThreadLevel);
    if (!jc) {
        print_error_and_exit(MEM_ERROR);
    }
    jc->atomic_var = SET_STAGE_AND_KEYS_PROCESS(MAP_STAGE, inputVec.size());
    initiate_threads(jc);
    return static_cast<JobHandle> (jc);
}

/**
 * Get a given pair and adds them to the framework databases.
 * @param key Key of given pair.
 * @param value Value of given pair.
 * @param context context
 */
void emit2(K2 *key, V2 *value, void *context) {
    auto *tc =  static_cast<ThreadContext *> (context);
    IntermediatePair pair = IntermediatePair(key, value);
    tc->intermediate_vec.push_back(pair);
}

void emit3 (K3* key, V3* value, void* context) {
    auto *tc = static_cast<ThreadContext *> (context);
    auto *jc = tc->job_context;
    OutputPair pair = OutputPair(key, value);
    if (pthread_mutex_lock(&jc->output_vec_lock) != 0) {
        print_error_and_exit(MUTEX_LOCK_ERROR);
    }
    jc->output_vec.push_back(pair);
    if (pthread_mutex_unlock(&jc->output_vec_lock) != 0) {
        print_error_and_exit(MUTEX_UNLOCK_ERROR);
    }
}

void getJobState(JobHandle job, JobState* state){
    auto* jc = static_cast<JobContext*> (job);
    uint64_t atomic_var = jc->atomic_var;
    state->stage = (stage_t) GET_STAGE(atomic_var);
    state->percentage = get_percentage(atomic_var);
}

void closeJobHandle(JobHandle job){
    auto* jc = static_cast<JobContext*> (job);
    waitForJob(job);
    while (jc->is_finished == 0){
        usleep(100);
    }
    delete jc;
}

void waitForJob(JobHandle job){
    auto* jc = static_cast<JobContext*> (job);
    int is_joined = jc->is_joined.exchange(true); // guaranteed to be atomic
    if (is_joined) {return;}
    for (size_t i = 0; i < jc->multi_thread_level; ++i) {
        if (pthread_join(jc->threads[i], nullptr)) {
            print_error_and_exit(PTHREAD_JOIN_ERROR);
        }
    }
    jc->is_finished = true;
}

// =================== Utils ============================

/**
 * The process each thread performs during the runtime of the program.
 * @param context Context of job.
 * @return
 */
static void* thread_entry(void* context){
    auto *tc = static_cast<ThreadContext *> (context);
    auto *jc = tc->job_context;
    map_phase(tc);
    tc->job_context->barrier->barrier();
    if (tc->thread_id == 0) {
        jc->atomic_already_processed_counter = 0;
	  	jc->atomic_var = SET_STAGE_AND_KEYS_PROCESS(SHUFFLE_STAGE, get_shuffle_keys_to_process(jc));
        shuffle_phase(tc);
	 	jc->atomic_var = SET_STAGE_AND_KEYS_PROCESS(REDUCE_STAGE, jc->shuffle_vec.size());
        for (int i = 0; i < jc->multi_thread_level; ++i) {
            if (sem_post(&jc->shuffle_sem)) {
                print_error_and_exit(SEM_LOCK_ERROR);
            }
        }
    } else {
        if (sem_wait(&jc->shuffle_sem)) {
            print_error_and_exit(SEM_UNLOCK_ERROR);
        }
    }
    reduce_phase(context);
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
            print_error_and_exit(PTHREAD_INIT_ERROR);
        }
    }
}

/**
 * Performs the map phase of the program, ran by the threads individually.
 * @param context Context of a thread.
 * @return
 */
static void *map_phase(void* context) {
    auto *tc = static_cast<ThreadContext *> (context);
    JobContext *jc = tc->job_context;
    int num_of_elements = (int) jc->input_vec.size();
    unsigned long old_value = 0;
    while ((old_value = jc->atomic_already_processed_counter++) < num_of_elements){
        jc->client.map(jc->input_vec[old_value].first, jc->input_vec[old_value].second, tc);
        jc->atomic_var++;
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
 * @return
 */
static void* shuffle_phase(void *context){
    auto *tc = static_cast<ThreadContext *> (context);
    JobContext *jc = tc->job_context;
    K2* maximum_key = find_maximum_key(jc);
    void* prev_key =  nullptr;
    IntermediateVec specific_key_vec;
    while (maximum_key != nullptr){
        if (prev_key != maximum_key){
          specific_key_vec = make_specific_key_vec(jc, maximum_key);
          jc->shuffle_vec.push_back(specific_key_vec);
          jc->atomic_var += specific_key_vec.size();
        }
        prev_key = maximum_key;
        maximum_key = find_maximum_key(jc);
    }
    return nullptr;
}

/**
 * Finds maximum key.
 * @param jc job context.
 * @return maximum key
 */
static K2* find_maximum_key(JobContext* jc){
    K2* maximum_key = nullptr;
    for (int i = 0; i < jc->multi_thread_level; ++i) {
        if (jc->contexts[i].intermediate_vec.empty()) {
            continue;
        }
        if ((maximum_key == nullptr) || (*maximum_key) < *(jc->contexts[i].intermediate_vec.back().first)) {
            maximum_key = jc->contexts[i].intermediate_vec.back().first;
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
    for (int i = 0; i < jc->multi_thread_level; ++i){
        IntermediateVec& inter_vec = jc->contexts[i].intermediate_vec;
        IntermediatePair& pair = jc->contexts[i].intermediate_vec.back();
        while ((!jc->contexts[i].intermediate_vec.empty()) &&
                !((*(pair.first) < *(maximum_key)) || (*(maximum_key) < *(pair.first)))){
            specific_key_vec.push_back(jc->contexts[i].intermediate_vec.back());
            jc->contexts[i].intermediate_vec.pop_back();
            if (jc->contexts[i].intermediate_vec.empty()){
                break;
            }
            pair = jc->contexts[i].intermediate_vec.back();
        }
    }
    return specific_key_vec;
}

static void *reduce_phase(void* context) {
    auto *tc = static_cast<ThreadContext *> (context);
    JobContext *jc = tc->job_context;
    int num_of_elements = (int) jc->shuffle_vec.size();
    unsigned long old_value = 0;
    while ((old_value = jc->atomic_already_processed_counter++) < num_of_elements){
        jc->client.reduce(&(jc->shuffle_vec.at(old_value)), tc);
        jc->atomic_var++;
    }
    return nullptr;
}

static int get_shuffle_keys_to_process(JobContext* jc){
    int counter = 0;
    for (int i = 0; i < jc->multi_thread_level; ++i){
        counter += (int) jc->contexts[i].intermediate_vec.size();
    }
    return counter;
}

static float get_percentage(uint64_t atomic_var){
    return  ((float) GET_ALREADY_PROCESSED(atomic_var) / (float) GET_KEYS_TO_PROCESS(atomic_var)) * PERCENTAGES;
}
