#include <cstdlib>
#include <pthread.h>
#include <atomic>
#include <semaphore.h>
#include "ThreadContext.h"

JobContext::~JobContext() {
    delete[] threads;
    delete[] contexts;
    delete barrier;
}

JobContext::JobContext(const MapReduceClient &client, const InputVec &input_vec, OutputVec
&output_vec, int multi_thread_level) : client(client), input_vec(input_vec), output_vec(output_vec),
                                multi_thread_level(multi_thread_level), atomic_var(0), barrier{} {
    threads = new(std::nothrow) pthread_t[multi_thread_level];
    if (threads == nullptr) {
        exit(1);
    }
    contexts = new(std::nothrow) ThreadContext[multi_thread_level];
    if (contexts == nullptr) {
        delete[] threads;
        exit(1);
    }

    barrier = new(std::nothrow) Barrier(multi_thread_level);
    if (barrier == nullptr) {
        delete[] threads;
        delete[] contexts;
    }

    if (sem_init(&shuffle_sem, 0, 0)) {
        delete[] threads;
        delete[] contexts;
        delete barrier;
        exit(1);
    }
}

