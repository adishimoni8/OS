#include "QuantumTimer.h"

QuantumTimer::QuantumTimer(int quantum_usec) {
  _timer.it_value.tv_sec = quantum_usec/SECOND;
  _timer.it_value.tv_usec = quantum_usec % SECOND;
  _quantums = 0;
}

int QuantumTimer::start_timer(){
  if (setitimer(ITIMER_VIRTUAL, &_timer, nullptr) != SUCCESS)
  {
	Exception(TIMER_ERR).print_error();
	exit(EXIT_FAILURE);
  }
  return SUCCESS;
}

itimerval &QuantumTimer::get_timer(){
  return _timer;
}

int QuantumTimer::get_quantums() const{
  return _quantums;
}
void QuantumTimer::inc_quantums(){
  _quantums++;
}

QuantumTimer::QuantumTimer() = default;


