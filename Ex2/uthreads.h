/*
 * User-Level Threads Library (uthreads)
 * Hebrew University OS course.
 * Author: OS, os@cs.huji.ac.il
 */

#ifndef _UTHREADS_H
#define _UTHREADS_H

#import "QuantumTimer.h"
#import "Exceptions.h"

//typedefs:
typedef unsigned long address_t;

#define MAX_THREAD_NUM 100 /* maximal number of threads */
#define STACK_SIZE 4096 /* stack size per UThread (in bytes) */
#define BACKED 1


typedef void (*thread_entry_point)(void);

/* External interface */


/**
 * @brief initializes the UThread library.
 *
 * You may assume that this function is called before any other UThread library function, and that it is called
 * exactly once.
 * The input to the function is the length of a quantum in micro-seconds.
 * It is an error to call this function with non-positive quantum_usecs.
 *
 * @return On success, return 0. On failure, return -1.
*/
int uthread_init(int quantum_usecs);

/**
 * @brief Creates a new UThread, whose entry point is the function entry_point with the signature
 * void entry_point(void).
 *
 * The UThread is added to the end of the READY threads list.
 * The uthread_spawn function should fail if it would cause the number of concurrent threads to exceed the
 * limit (MAX_THREAD_NUM).
 * Each UThread should be allocated with a stack of size STACK_SIZE bytes.
 *
 * @return On success, return the ID of the created UThread. On failure, return -1.
*/
int uthread_spawn(thread_entry_point entry_point);


/**
 * @brief Terminates the UThread with ID tid and deletes it from all relevant control structures.
 *
 * All the resources allocated by the library for this UThread should be released. If no UThread with ID tid exists it
 * is considered an error. Terminating the main UThread (tid == 0) will result in the termination of the entire
 * process using exit(0) (after releasing the assigned library memory).
 *
 * @return The function returns 0 if the UThread was successfully terminated and -1 otherwise. If a UThread terminates
 * itself or the main UThread is terminated, the function does not return.
*/
int uthread_terminate(int tid);


/**
 * @brief Blocks the UThread with ID tid. The UThread may be resumed later using uthread_resume.
 *
 * If no UThread with ID tid exists it is considered as an error. In addition, it is an error to try blocking the
 * main UThread (tid == 0). If a UThread blocks itself, a scheduling decision should be made. Blocking a UThread in
 * BLOCKED _state has no effect and is not considered an error.
 *
 * @return On success, return 0. On failure, return -1.
*/
int uthread_block(int tid);


/**
 * @brief Resumes a blocked UThread with ID tid and moves it to the READY _state.
 *
 * Resuming a UThread in a RUNNING or READY _state has no effect and is not considered as an error. If no UThread with
 * ID tid exists it is considered an error.
 *
 * @return On success, return 0. On failure, return -1.
*/
int uthread_resume(int tid);


/**
 * @brief Blocks the RUNNING UThread for num_quantums quantums.
 *
 * Immediately after the RUNNING UThread transitions to the BLOCKED _state a scheduling decision should be made.
 * After the sleeping time is over, the UThread should go back to the end of the READY threads list.
 * The number of quantums refers to the number of times a new quantum starts, regardless of the reason. Specifically,
 * the quantum of the UThread which has made the call to uthread_sleep isn’t counted.
 * It is considered an error if the main UThread (tid==0) calls this function.
 *
 * @return On success, return 0. On failure, return -1.
*/
int uthread_sleep(int num_quantums);


/**
 * @brief Returns the UThread ID of the calling UThread.
 *
 * @return The ID of the calling UThread.
*/
int uthread_get_tid();


/**
 * @brief Returns the total number of quantums since the library was initialized, including the current quantum.
 *
 * Right after the call to uthread_init, the value should be 1.
 * Each time a new quantum starts, regardless of the reason, this number should be increased by 1.
 *
 * @return The total number of quantums.
*/
int uthread_get_total_quantums();


/**
 * @brief Returns the number of quantums the UThread with ID tid was in RUNNING _state.
 *
 * On the first time a UThread runs, the function should return 1. Every additional quantum that the UThread starts should
 * increase this value by 1 (so if the UThread with ID tid is in RUNNING _state when this function is called, include
 * also the current quantum). If no UThread with ID tid exists it is considered an error.
 *
 * @return On success, return the number of quantums of the UThread with ID tid. On failure, return -1.
*/
int uthread_get_quantums(int tid);


#endif
