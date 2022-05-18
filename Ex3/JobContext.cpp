#include <cstdlib>
#include "JobContext.h"
#include "Exception.h"

JobContext::JobContext(const MapReduceClient &client, const InputVec &input_vec, OutputVec
&output_vec, int multi_thread_level) : client(client), input_vec(input_vec), output_vec(output_vec),
                                multi_thread_level(multi_thread_level), atomic_var(0), barrier{},
                                atomic_already_processed_counter(0), is_finished(false), is_joined(false) {
    threads = new(std::nothrow) pthread_t[multi_thread_level];
    if (threads == nullptr) {
        print_error_and_exit(MEM_ERROR);
    }

    contexts = new(std::nothrow) ThreadContext[multi_thread_level];
    if (contexts == nullptr) {
        delete[] threads;
        print_error_and_exit(MEM_ERROR);
    }

    barrier = new(std::nothrow) Barrier(multi_thread_level);
    if (barrier == nullptr) {
        delete[] threads;
        delete[] contexts;
        print_error_and_exit(MEM_ERROR);
    }

    if (pthread_mutex_init(&output_vec_lock, nullptr)) {
        delete[] threads;
        delete[] contexts;
        delete barrier;
        print_error_and_exit(MUTEX_INIT_ERROR);
    }

    if (sem_init(&shuffle_sem, 0, 0)) {
        delete[] threads;
        delete[] contexts;
        delete barrier;
        pthread_mutex_destroy(&output_vec_lock);
        print_error_and_exit(SEM_INIT_ERROR);
    }
}

JobContext::~JobContext() {
    delete[] threads;
    delete[] contexts;
    delete barrier;
    pthread_mutex_destroy(&output_vec_lock);
    sem_destroy(&shuffle_sem);
}

