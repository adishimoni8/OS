#ifndef EX2__EXCEPTIONS_H_
#define EX2__EXCEPTIONS_H_

#include <iostream>
using namespace std;

#define ERROR -1
#define SUCCESS 0

/**
 * Enum containing all possible errors.
 */

#define MEM_ERROR "Error locating memory"
#define PTHREAD_LIB_ERROR "Error in pthread library"

static void print_error_and_exit(const string& error){
    std::cerr << "system error: " << error << std::endl;
}


#endif //EX2__EXCEPTIONS_H_



