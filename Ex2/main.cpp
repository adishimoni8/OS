#include <iostream>
#include "uthreads.h"

// Thread 1:

void func() {
  std::cout << "hi" << std::endl;
}

void func1() {
  std::cout << "buy" << std::endl;
  uthread_resume(2);
}

int main() {
  uthread_init(10000000);
  uthread_spawn(func);
  uthread_block(2);
  uthread_spawn(func1);
  uthread_terminate(3);
  uthread_terminate(2);
  uthread_terminate(1);
  return 0;
}