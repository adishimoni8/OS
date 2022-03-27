#include <cstdlib>
#include "uthreads.h"
#include <cstdio>
#include <csetjmp>
#include <csignal>
#include <unistd.h>
#include <ctime>
#include <iostream>
#include <deque>
#include <unordered_set>
#include "UThread.cc"
#include "Error.h"
using namespace std;

//Global variables:
std::map<int, UThread *> threads;
deque<int> ready_threads;
unordered_set<int> blocked_threads;
struct sigaction sa;
QuantumTimer timer;
int cur_tid;
int available_tid;

//Utils:

static void main_func(){};

static void sigvtalrm_handler(){
  	timer.inc_quantums();
  	threads[cur_tid]->inc_quantums(); //
	if (ready_threads.empty()){ //if current thread is only the main one.
	  return;
	}
	int where = threads[cur_tid]->env_snapshot();
	if (where != BACKED){ //if cur thread was jumped from prev.
	  threads[cur_tid]->set_state(READY); //sets thread RUNNING->READY.
	  ready_threads.push_back(threads[cur_tid]->get_tid());
	  cur_tid = ready_threads.front(); //sets current thread to first in line.
	  ready_threads.pop_front(); // pop the thread from queue.
	  threads[cur_tid]->set_state(RUNNING); // set thread READY->RUNNING
	  timer.start_timer(); //starts a new timer for current thread.
	  threads[cur_tid]->load_env();
	}
}

static void to_mask_sigvtalrm(bool mask){
  if (mask){
	if (sigprocmask(SIG_BLOCK, &sa.sa_mask, nullptr) != SUCCESS){
	  error_to_stderr(SIGSET_ERR);
	  exit(EXIT_FAILURE);
	}
  } else{
	if (sigprocmask(SIG_UNBLOCK, &sa.sa_mask, nullptr) != SUCCESS){
	  error_to_stderr(SIGSET_ERR);
	  exit(EXIT_FAILURE);
	}
  }
}

//API
int uthread_init(int quantum_usecs){
  if (quantum_usecs <= 0){ //invalid input check.
	error_to_stderr(QUANTUM_ERR);
    return ERROR;
  }
  if (MAX_THREAD_NUM <= 0){
	error_to_stderr(MAX_THREAD_ERR);
	return ERROR;
  }
  sa.sa_handler = reinterpret_cast<_sig_func_ptr>(&sigvtalrm_handler);
  sa.sa_flags = 0;
  if (sigaction(SIGVTALRM, &sa, nullptr) != SUCCESS){ //masks SIGVTALRM.
	error_to_stderr(SIGACT_ERR);
	exit(EXIT_FAILURE);
  }
  available_tid = 0;
  return uthread_spawn(main_func);
}

int uthread_spawn(thread_entry_point entry_point){
  to_mask_sigvtalrm(true);
  if ((*entry_point) == nullptr){ //invalid input check.
	error_to_stderr(INVALID_FUNC_ERR);
	return ERROR;
  }
  if (threads.size() >= MAX_THREAD_NUM){ //overflow check.
	error_to_stderr(MAX_THREAD_ERR);
	return ERROR;
  }
  int tid = available_tid;
  auto thread = new UThread(tid);
  if (thread == nullptr){
	exit(EXIT_FAILURE);
  }
  auto sp = (address_t) thread->get_stack() + STACK_SIZE - sizeof(address_t);
  auto pc = (address_t) *entry_point;
  thread->env_snapshot();
  (thread->get_env()->__jmpbuf)[JB_SP] = translate_address(sp);
  (thread->get_env()->__jmpbuf)[JB_PC] = translate_address(pc);
  sigemptyset(&(thread->get_env()->__saved_mask));
  threads[tid] = thread; //sets thread to be last in line in queue.
  ready_threads.push_back(tid);
  available_tid++;
  to_mask_sigvtalrm(false);
  return tid;
}

int uthread_terminate(int tid){
  return 0;
}

int uthread_block(int tid){
  return 0;
}

int uthread_resume(int tid){
  return 0;
}

int uthread_sleep(int num_quantums){
  return 0;
}

int uthread_get_tid(){
  return cur_tid;
}

int uthread_get_total_quantums(){
  return timer.get_quantums();
}

int uthread_get_quantums(int tid){
  return threads[cur_tid]->get_quantums();
}