#ifndef EX2__UTHREAD_H_
#define EX2__UTHREAD_H_

#include "uthreads.h"
#include <setjmp.h>


enum ThreadState{
	RUNNING,
	READY,
	BLOCKED
};
class UThread {
 public:
  /**
 * Constructor for a new Thread type object.
 * @param tid - new thread's ID.
 */
  UThread(int tid);

  /**
   * Id getter.
   * @return ID of specified thread.
   */
  int get_tid() const;

  /**
   *
   * @return the state of specified thread.
   */
  ThreadState get_state() const;

  /**
   * sets the state of specified thread.
   * @param state state wished to be changed to.
   */
  void set_state(ThreadState state);

  /**
   *
   * @return quantums of specified thread.
   */
  int get_quantums() const;

  /**
   * increments the amount of quantums the specified thread has.
   */
  void inc_quantums();

  /**
   *
   * @return the environment of specified thread.
   */
  sigjmp_buf& get_env();

  /**
   * captures the current environment and status of specified thread.
   * @return 0 if just captured, 1 if returned from another thread.
   */
  int env_snapshot();

  /**
   * returns from another thread to current thread, loads the environment and
   * status of current thread to continue it's progress.
   */
  void load_env();

  /**
   *
   * @return stack of specified thread.
   */
  char* get_stack();

  /**
   * default destructor of specified thread.
   */
  virtual ~UThread();

 private:
  int _tid; //id of thread.
  ThreadState _state; //state of thread (READY\RUNNING\BLOCKED)
  int _quantums; //amount of quantums thread has taken so far.
  sigjmp_buf _env; //environment and status of thread.
  char _stack[STACK_SIZE]{}; //stack of thread.
};

#endif //EX2__UTHREAD_H_
