#ifndef EX2__EXCEPTIONS_H_
#define EX2__EXCEPTIONS_H_

#include <iostream>
#include <map>
#include <utility>

using namespace std;

////macros, enums & variables:
//
//
//
///**
// * A struct organizing each error with it's matching error message.
// */
//struct err_data{
//    ERRORS _error;
//    ERR_TYPE _err_type;
//    string _err_msg;
//};
//
//
//
///**
// * A dictionary containing all errors with their error-types and error messages.
// */
//map<ERRORS,err_data> error_dict = {
//	{QUANTUM_ERR, err_data("A none-positive quantum was given.", LIB_ERR)},
//	{TIMER_ERR,err_data("Failed setting the timer.",SYS_ERR)},
//	{SIGSET_ERR,err_data("Failed setting a signal.",SYS_ERR)},
//	{MAX_THREAD_ERR,err_data("Exceeded max amount of threads.",LIB_ERR)},
//	{SIGACT_ERR,err_data("Failed inspecting or handling signal.",SYS_ERR)},
//	{INVALID_FUNC_ERR,err_data("Invalid function given as input.",LIB_ERR)},
//};
//
///**
// * Printing errors to stderr according to their type.
// * @param error error to be printed.
// */
//void error_to_stderr(ERRORS error){
//  cerr << (error_dict[error]._err_type==SYS_ERR ? "system error: "
//												: "thread library error: ");
//	cerr << error_dict[error]._err_msg << endl;
//}

//====================================================
// guy, please implement below the enums:
// The class of error which has 3 private fields: ERROR_TYPE,ERROR_KIND,message.
// A constructor which gets the ERROR_KIND and make a switch to find the ERR_TYPE
// and the message.
// Make a print_error method which prints the relevant message.
// After that, when we want to create an error on uthreds.cc just create an
// instance of an error and use the print_error method. It's much more of a good practice.
// you can use the errors commented out above.
// After this - go ahead and create the main thread in the init function just like
// we did at the beginning (I'll explain to you tomorrow why), and dont create it
// at the spawn method.
// When everything is ready try to run main - it will not compile due to the lines:
// (thread->get_env()->__jmpbuf)[JB_SP] = translate_address(sp).
// but please make sure there are the only errors we get, we'll fix it soon
// good luck bro!
//====================================================

/**
 * Enum describing the different error type origins.
 */
enum ERR_TYPE{
    LIB_ERR,
    SYS_ERR
};
/**
 * Enum containing all possible errors.
 */
enum ERROR_KIND{
    QUANTUM_ERR,
    MAX_THREAD_ERR,
    TIMER_ERR,
    SIGSET_ERR,
    SIGACT_ERR,
    INVALID_FUNC_ERR
};

class ERROR{

};


#endif //EX2__EXCEPTIONS_H_



