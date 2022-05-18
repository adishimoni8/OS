#ifndef EX2__EXCEPTIONS_H_
#define EX2__EXCEPTIONS_H_

#include <iostream>
using namespace std;

// All possible errors.
#define MEM_ERROR "Error locating memory."
#define PTHREAD_INIT_ERROR "Failed creating thread."
#define SEM_INIT_ERROR "Failed creating semaphore."
#define SEM_LOCK_ERROR "Failed locking semaphore."
#define SEM_UNLOCK_ERROR "Failed unlocking semaphore."
#define PTHREAD_JOIN_ERROR "Failed using pthread_join."
#define MUTEX_LOCK_ERROR "Failed locking mutex."
#define MUTEX_UNLOCK_ERROR "Failed unlocking mutex."
#define MUTEX_INIT_ERROR "Failed creating mutex."

static void print_error_and_exit(const string& error){
    std::cerr << "system error: " << error << std::endl;
    exit(1);
}


#endif //EX2__EXCEPTIONS_H_



