#include <iostream>
#include "uthreads.h"

// Thread 1:

void func() {
  std::cout << "hi" << std::endl;
}

void func1() {
  std::cout << "buy" << std::endl;
}

int main() {
  uthread_init(10000000);
  return 0;
}