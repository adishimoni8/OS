#include "osm.h"
#include <sys/time.h>

#define SECOND_TO_NANO 1000000000
#define MICRO_TO_NANO 1000
#define RETURN_ERROR if (iterations == 0) return -1;
#define UNROLLING_FACTOR 5
#define REMAINDER unsigned int remainder = iterations % UNROLLING_FACTOR;

void just_function(){
}

double osm_operation_time(unsigned int iterations){
  RETURN_ERROR
  REMAINDER
  struct timeval start, end;
  gettimeofday(&start, nullptr);
  int counter = 0;
  for (int i = 0; i < iterations + remainder; i+=UNROLLING_FACTOR) {
    counter += 1;
    counter += 1;
    counter += 1;
    counter += 1;
    counter += 1;
  }
  gettimeofday(&end, nullptr);
  return ((double)(end.tv_sec * SECOND_TO_NANO + end.tv_usec * MICRO_TO_NANO)
	  - (double)(start.tv_sec * SECOND_TO_NANO + start.tv_usec * MICRO_TO_NANO))
	  / iterations;
}

double osm_function_time(unsigned int iterations){
  RETURN_ERROR
  REMAINDER
  struct timeval start, end;
  gettimeofday(&start, nullptr);
  for (int i = 0; i < iterations + remainder; i+=UNROLLING_FACTOR) {
	just_function();
	just_function();
	just_function();
	just_function();
	just_function();
  }
  gettimeofday(&end, nullptr);
  return ((double)(end.tv_sec * SECOND_TO_NANO + end.tv_usec * MICRO_TO_NANO)
	  - (double)(start.tv_sec * SECOND_TO_NANO + start.tv_usec * MICRO_TO_NANO))
	  / iterations;
}

double osm_syscall_time(unsigned int iterations){
  RETURN_ERROR
  REMAINDER
  struct timeval start, end;
  gettimeofday(&start, nullptr);
  for (int i = 0; i < iterations + remainder; i+=UNROLLING_FACTOR) {
    OSM_NULLSYSCALL;
	OSM_NULLSYSCALL;
	OSM_NULLSYSCALL;
	OSM_NULLSYSCALL;
	OSM_NULLSYSCALL;
  }
  gettimeofday(&end, nullptr);
  return ((double)(end.tv_sec * SECOND_TO_NANO + end.tv_usec * MICRO_TO_NANO)
	  - (double)(start.tv_sec * SECOND_TO_NANO + start.tv_usec * MICRO_TO_NANO))
	  / iterations;
}
