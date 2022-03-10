#include <iostream>
#include "osm.h"

int main() {
  std::cout << "add: " << osm_operation_time(10000000) << std::endl;
  std::cout << "func: " << osm_function_time(10000000) << std::endl;
  std::cout << "sys: " << osm_syscall_time(10000000) << std::endl;
  return 0;
}
