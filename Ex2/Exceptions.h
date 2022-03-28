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
//XThe class of error which has 3 private fields: ERROR_TYPE,ERROR_KIND,message.
//XA constructor which gets the ERROR_KIND and make a switch to find the
//XERR_TYPE and the message.
//XMake a print_error method which prints the relevant message.
//XAfter that, when we want to create an error on uthreds.cc just create an
//Xinstance of an error and use the print_error method. It's much more of a
//Xgood practice.
//Xyou can use the errors commented out above.
//XAfter this - go ahead and create the main thread in the init function just
//xlike
//xwe did at the beginning (I'll explain to you tomorrow why), and dont
//xcreate it at the spawn method.
// When everything is ready try to run main - it will not compile due to the lines:
// (thread->get_env()->__jmpbuf)[JB_SP] = translate_address(sp).
// but please make sure there are the only errors we get, we'll fix it soon
// good luck bro!
//====================================================

#define QUANTUM_ERR_MSG "A none-positive quantum was given."
#define TIMER_ERR_MSG "Failed setting the timer."
#define SIGSET_ERR_MSG "Failed setting a signal."
#define MAX_THREAD_ERR_MSG "Exceeded max amount of threads."
#define SIGACT_ERR_MSG "Failed inspecting or handling signal."
#define INVALID_FUNC_ERR_MSG "Invalid function given as input."

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
enum ERR_KIND{ //todo define which enums match which type.
    QUANTUM_ERR,
    MAX_THREAD_ERR,
    TIMER_ERR,
    SIGSET_ERR,
    SIGACT_ERR,
    INVALID_FUNC_ERR
};

class Exceptions{
 public:
  /**
   * Constructor for type of error, recieves error kind and concludes errors
   * type and relevant error message.
   */
  explicit Exceptions(ERR_KIND);

  /**
   * Prints error message upon demand.
   */
  void print_error();

 private:
  ERR_TYPE _type; //type of error.
  ERR_KIND _kind; // kind of error.
  string _message; // message to be printed when error occurs.
};


#endif //EX2__EXCEPTIONS_H_



