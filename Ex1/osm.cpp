#include "osm.h"
#include <sys/time.h>
#include <iostream>
#include "osm.h"

#define SECOND_TO_NANO 1000000000
#define MICRO_TO_NANO 1000
#define ERROR -1
#define ITERATION_ERROR if (iterations == 0) return ERROR;
#define UNROLLING_FACTOR 10

/**
 * A dummy function.
 */
void just_function(){}

/**
 * Calculates rounded-up fixed amount of iterations.
 * @param iterations amount of given iterations to round-up.
 * @return rounded amount of iterations.
 */
unsigned int get_num_of_iterations(unsigned int iterations){
  return iterations + (UNROLLING_FACTOR - (iterations % UNROLLING_FACTOR));
}

/**
 * Calculates the average time taken for a task to accomplish
 * @param start starting time of task.
 * @param end ending time of task.
 * @param iterations amount of repetitions of task.
 * @return average performance time of task.
 */
double avg_time(timeval start, timeval end, unsigned int iterations){
  auto start_time = (double)(start.tv_sec * SECOND_TO_NANO + start.tv_usec *
  	MICRO_TO_NANO);
  auto end_time = (double)(end.tv_sec * SECOND_TO_NANO + end.tv_usec *
	  MICRO_TO_NANO);
  return (end_time - start_time) / (double) iterations;
}

/**
 * Performs the addition operation and measures the time it takes for it to
 * be done.
 * @param iterations amount of desired iterations (repetitions) of task to be
 * done.
 * @return average performance time of task.
 */
double osm_operation_time(unsigned int iterations){
  ITERATION_ERROR
  unsigned int rounded_iterations = get_num_of_iterations(iterations);
  timeval start{}, end{};
  if (gettimeofday(&start, nullptr) == ERROR) return ERROR;
  int counter1 = 0;
  int counter2 = 0;
  int counter3 = 0;
  int counter4 = 0;
  int counter5 = 0;
  int counter6 = 0;
  int counter7 = 0;
  int counter8 = 0;
  int counter9 = 0;
  int counter10 = 0;

  for (unsigned int i = 0; i < rounded_iterations; i+=UNROLLING_FACTOR) {
    counter1 ++;
    counter2 ++;
    counter3 ++;
    counter4 ++;
    counter5 ++;
	counter6 ++;
	counter7 ++;
	counter8 ++;
	counter9 ++;
	counter10 ++;
  }
  if (gettimeofday(&end, nullptr) == ERROR) return ERROR;
  return avg_time(start, end, rounded_iterations);
}

/**
 * Performs the function call operation and measures the time it takes for it
 * to be done.
 * @param iterations amount of desired iterations (repetitions) of task to be
 * done.
 * @return average performance time of task.
 */
double osm_function_time(unsigned int iterations){
  ITERATION_ERROR
  unsigned int rounded_iterations = get_num_of_iterations(iterations);
  timeval start{}, end{};
  if (gettimeofday(&start, nullptr) == ERROR) return ERROR;
  for (unsigned int i = 0; i < rounded_iterations; i+=UNROLLING_FACTOR) {
	just_function();
	just_function();
	just_function();
	just_function();
	just_function();
	just_function();
	just_function();
	just_function();
	just_function();
	just_function();
  }
  if (gettimeofday(&end, nullptr) == ERROR) return ERROR;
  return avg_time(start, end, rounded_iterations);
}

/**
 * Performs the system call operation (found in given osm.h file) and measures
 * the time it takes for it to be done.
 * @param iterations amount of desired iterations (repetitions) of task to be
 * done.
 * @return average performance time of task.
 */
double osm_syscall_time(unsigned int iterations){
  ITERATION_ERROR
  unsigned int rounded_iterations = get_num_of_iterations(iterations);
  timeval start{}, end{};
  if (gettimeofday(&start, nullptr) == ERROR) return ERROR;
  for (unsigned int i = 0; i < rounded_iterations; i+=UNROLLING_FACTOR) {
    OSM_NULLSYSCALL;
	OSM_NULLSYSCALL;
	OSM_NULLSYSCALL;
	OSM_NULLSYSCALL;
	OSM_NULLSYSCALL;
	OSM_NULLSYSCALL;
	OSM_NULLSYSCALL;
	OSM_NULLSYSCALL;
	OSM_NULLSYSCALL;
	OSM_NULLSYSCALL;
  }
  if (gettimeofday(&end, nullptr) == ERROR) return ERROR;
  return avg_time(start, end, rounded_iterations);
}