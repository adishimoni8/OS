#ifndef EX2__EXCEPTIONS_H_
#define EX2__EXCEPTIONS_H_

#include <iostream>
#include <map>
#include <tuple>
#include <utility>

using namespace std;

//macros, enums & variables:
#define ERROR -1
#define SUCCESS 0
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
enum ERRORS{
  QUANTUM_ERR,
  MAX_THREAD_ERR,
  TIMER_ERR,
  SIGSET_ERR,
  SIGACT_ERR,
  INVALID_FUNC_ERR
};

/**
 * A struct organizing each error with it's matching error message.
 */
struct err_data{
  err_data(string msg, ERR_TYPE type) {
	_err_msg = std::move(msg);
	_err_type = type;
  }
  string _err_msg;
  ERR_TYPE _err_type;
};

/**
 * A dictionary containing all errors with their error-types and error messages.
 */
map<ERRORS,err_data> error_dict = { //todo check error types with adi.
	{QUANTUM_ERR, err_data("A none-positive quantum was given.", LIB_ERR)},
	{TIMER_ERR,err_data("Failed setting the timer.",SYS_ERR)},
	{SIGSET_ERR,err_data("Failed setting a signal.",SYS_ERR)},
	{MAX_THREAD_ERR,err_data("Exceeded max amount of threads.",LIB_ERR)},
	{SIGACT_ERR,err_data("Failed inspecting or handling signal.",SYS_ERR)},
	{INVALID_FUNC_ERR,err_data("Invalid function given as input.",LIB_ERR)},

};

/**
 * Printing errors to stderr according to their type.
 * @param error error to be printed.
 */
void error_to_stderr(ERRORS error){
  cerr << (error_dict[error]._err_type==SYS_ERR ? "system error: "
												: "thread library error: ");
	cerr << error_dict[error]._err_msg << endl;
}
#endif //EX2__EXCEPTIONS_H_
