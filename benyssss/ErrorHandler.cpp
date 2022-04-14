//
// Created by Bengo on 6/5/2021.
//
#include <iostream>
#include "ErrorHandler.h"

void printError(ERROR_TYPE errorType)
{
    std::cerr << "system error: ";
    switch (errorType){
        case MEMORY_ERROR:
            std::cerr << "memory allocation failed.";
            break;
        case SEM_INIT_ERROR:
            std::cerr << "sem_init failed.";
            break;
        case SEM_LOCK_UNLOCK_ERROR:
            std::cerr << "semaphore lock/unlock failed.";
            break;
        case MUTEX_INIT_ERROR:
            std::cerr << "mutex_init failed.";
            break;
        case MUTEX_LOCK_UNLOCK_ERROR:
            std::cerr << "mutex lock/unlock failed.";
            break;
        case THREAD_CREATION_ERROR:
            std::cerr << "thread creation failed.";
            break;
        case THREAD_JOIN_ERROR:
            std::cerr << "thread join failed.";
            break;
    }
}
