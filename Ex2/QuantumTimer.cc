//
// Created by gopgo on 27/03/2022.
//

#include "QuantumTimer.h"


#define SECONDS_TO_MICROS 1000000

QuantumTimer::QuantumTimer(int quantum_usec) {
  _timer.it_value.tv_sec = quantum_usec/SECONDS_TO_MICROS;
  _timer.it_value.tv_usec = quantum_usec % SECONDS_TO_MICROS;
  _quantums = 0;
}

int QuantumTimer::start_timer(){
  if (setitimer(ITIMER_VIRTUAL, &_timer, nullptr) != SUCCESS)
  {
	error_to_stderr(TIMER_ERR);
	exit(EXIT_FAILURE);
  }
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


