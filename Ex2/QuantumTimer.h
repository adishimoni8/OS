#ifndef EX2__QUANTUMTIMER_H_
#define EX2__QUANTUMTIMER_H_

#include "Exception.h"
#include <sys/time.h>

#define SECOND 1000000

/**
 * This class represents a timer to measure the passing virtual time.
 */
class QuantumTimer {
 public:
  /**
   * Default constructor for QuantumTimer, recieves amount of quantum
   * microseconds to calculate the time which timer starts counting at.
   */

  QuantumTimer();

  explicit QuantumTimer(int);

  /**
   * Starts the timer.
   * @return 0 upon success, -1 otherwise.
   */
  int start_timer();

  /**
   *
   * @return timer.
   */
  itimerval& get_timer();

  /**
   *
   * @return amount of quantums of specified QuantumTimer.
   */
  int get_quantums() const;

  /**
   * Increments the amount of quantums of specified QuantumTimer.
   */
  void inc_quantums();

 private:
  struct itimerval _timer; //internal timer.
  int _quantums; //amount of quantums.
};

#endif //EX2__QUANTUMTIMER_H_
