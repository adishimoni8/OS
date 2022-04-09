#include <cstdlib>
#include "uthreads.h"
#include <csignal>
#include <deque>
#include <unordered_set>
#include <map>
#include "Exception.h"
#include "UThread.h"
#include "QuantumTimer.h"
#include "EnvHelpers.h"
using namespace std;

// Macros:
#define BACKED 1
#define MAIN_TID 0

//Global Variables:
std::map<int, UThread *> threads;
deque<int> ready_threads;
unordered_set<int> blocked_threads;
struct sigaction sa;
QuantumTimer timer;
int cur_tid;
int available_tid;
int num_of_threads;
sigset_t masked_signals;

//Utils (inner functions):

static void update_available_tid(){
  int i = available_tid + 1;
  while (threads[i] != nullptr){
	++i;
  }
  available_tid = i;
}

static void erase_element_from_queue(deque<int> *queue, int tid){
    for (auto it = queue->begin(); it != queue->end(); it++){
        if (*it == tid){
            queue->erase(it);
            break;
        }
    }
}

static int free_all_memory(){
    for (unsigned int i = 0; i < threads.size(); ++i){
        if (threads[i] != nullptr){
            delete threads[i];
        }
    }
    threads.clear();
    blocked_threads.clear();
    ready_threads.clear();
    return SUCCESS;
}

static void handle_sleepers(){
    for (auto it = blocked_threads.begin(); it != blocked_threads.end(); it++){
        int tid = *it;
        UThread *thread = threads[tid];
        ThreadState state = thread->get_state();
        if (state == BLOCKED_AND_SLEEPING || state == SLEEPING){
            thread->dec_sleep_time();
            if (thread->get_sleep_time() == 0){
                if (state == BLOCKED_AND_SLEEPING){
                    thread->set_state(BLOCKED);
                } else {
                    thread->set_state(READY);
                    blocked_threads.erase(tid);
                    ready_threads.push_back(tid);
                }
            }
        }
    }
}

static int handle_err(ERR_KIND kind){
    Exception exception = Exception(kind);
    exception.print_error();
    if (exception.get_type() == SYS_ERR){
	  free_all_memory();
        exit(EXIT_FAILURE);
    }
    return ERROR;
}

static void init_masked_signals(){
  if (sigemptyset(&masked_signals) < 0) {
	handle_err(SIGSET_ERR);
  }
  if (sigaddset(&masked_signals,SIGVTALRM) < 0) {
	handle_err(SIGSET_ERR);
  }
}

static int to_mask_signals(bool mask){
    if (mask){
        if (sigprocmask(SIG_BLOCK, &masked_signals, nullptr) != SUCCESS){
            return handle_err(SIGSET_ERR);
        }
    } else{
        if (sigprocmask(SIG_UNBLOCK, &masked_signals, nullptr) != SUCCESS){
            return handle_err(SIGSET_ERR);
        }
    }
    return SUCCESS;
}

static void sigvtalrm_handler(int sig_num){
  	timer.inc_quantums();
  	threads[cur_tid]->inc_quantums(); //inc the quantum of the thread finished.
    handle_sleepers();
	if (ready_threads.empty()){ //if current thread is only the main one.
      timer.start_timer();
	  return;
	}
	int where = threads[cur_tid]->env_snapshot();
	if (where != BACKED){ //if cur thread was jumped from prev.
	  threads[cur_tid]->set_state(READY); //sets thread RUNNING->READY.
	  ready_threads.push_back(threads[cur_tid]->get_tid());
	  cur_tid = ready_threads.front(); //sets current thread to first in line.
	  ready_threads.pop_front(); // pop the thread from queue.
	  threads[cur_tid]->set_state(RUNNING); // set thread READY->RUNNING.
	  timer.start_timer(); //starts a new timer for current thread.
	  threads[cur_tid]->load_env();
	}
}

//API
int uthread_init(int quantum_usecs){
  // Sanity check:
  if (quantum_usecs <= 0){
    return handle_err(QUANTUM_ERR);
  }

  // Set timer:
  timer = QuantumTimer(quantum_usecs);

  // Init masked signals:
  init_masked_signals();

  // Mask sigvtalrm:
  sa.sa_handler = &sigvtalrm_handler;
  if (sigaction(SIGVTALRM, &sa, nullptr) != SUCCESS){
    return handle_err(SIGACT_ERR);
  }

  // Initiate Main Thread:
  auto* main_thread = new (nothrow) UThread(MAIN_TID);
  if (main_thread == nullptr){
    return handle_err(MEM_ERR);
  }
  threads[MAIN_TID] = main_thread;
  main_thread->set_state(RUNNING);
  timer.start_timer();

  // Update global vars:
  available_tid = 1;
  num_of_threads++;
  return SUCCESS;
}

int uthread_spawn(thread_entry_point entry_point){
  to_mask_signals(true);

  // Sanity checks:
  if ((*entry_point) == nullptr){
    return handle_err(INVALID_FUNC_ERR);
  }
  if (num_of_threads >= MAX_THREAD_NUM){ //overflow check.
      return handle_err(MAX_THREAD_ERR);
  }

  // Create the new thread:
  int tid = available_tid;
  auto thread = new (nothrow) UThread(tid);
  if (thread == nullptr){
      return handle_err(MEM_ERR);
  }
  auto sp = (address_t) thread->get_stack() + STACK_SIZE - sizeof(address_t);
  auto pc = (address_t) *entry_point;
  thread->env_snapshot();
  (thread->get_env()->__jmpbuf)[JB_SP] = translate_address(sp);
  (thread->get_env()->__jmpbuf)[JB_PC] = translate_address(pc);
  sigemptyset(&(thread->get_env()->__saved_mask));
  threads[tid] = thread; //sets thread to be last in line in queue.
  thread->set_state(READY);
  ready_threads.push_back(tid);

  //Update global vars:
  num_of_threads++;
  update_available_tid();

  to_mask_signals(false);
  return tid;
}

int uthread_terminate(int tid){
  to_mask_signals(true);

  //Sanity checks:
  if ((threads.find(tid) == threads.end()) || (threads[tid] == nullptr)){
	to_mask_signals(false);
    return handle_err(INVALID_TID_ERR);
  }

  // Exit the program:
  if (tid == 0){
	free_all_memory();
	exit(EXIT_SUCCESS);
  }

  // Find and terminate thread:
  UThread *thread = threads[tid];
  ThreadState state = thread->get_state();
  delete thread;
  threads[tid] = nullptr;

  // Update global vars:
  if (available_tid > tid){
	  available_tid = tid;
  }
  num_of_threads--;

  // Decide what to do next:
  switch(state){
    case READY:
	  erase_element_from_queue(&ready_threads,tid);
	  break;
    case RUNNING:
	  cur_tid = ready_threads.front();
	  ready_threads.pop_front();
	  threads[cur_tid]->set_state(RUNNING);
	  handle_sleepers();
	  timer.inc_quantums();
	  timer.start_timer();
	  to_mask_signals(false);
	  threads[cur_tid]->load_env();
	  break;
	default: // All other states
	  blocked_threads.erase(tid);
	  break;
  }

  to_mask_signals(false);
  return SUCCESS;
}

int uthread_block(int tid){
  to_mask_signals(true);

  // Sanity checks:
  if (threads.find(tid) == threads.end() || threads[tid] == nullptr){
	to_mask_signals(false);
    return handle_err(INVALID_TID_ERR);
  }
  if (tid == 0){
	to_mask_signals(false);
	  return handle_err(MAIN_THREAD_BLOCK_ERR);
  }

  // Find and block thread:
  UThread *thread = threads[tid];
  ThreadState state = thread->get_state();

  switch(state){
	case BLOCKED:
	  break;
	case BLOCKED_AND_SLEEPING:
	  break;
    case READY:
	  erase_element_from_queue(&ready_threads,tid);
	  thread->set_state(BLOCKED);
	  blocked_threads.insert(tid);
	  break;
    case SLEEPING:
	  thread->set_state(BLOCKED_AND_SLEEPING);
	  break;
    case RUNNING:
	  int where = thread->env_snapshot();
	  if (where != BACKED) {
		cur_tid = ready_threads.front();
		ready_threads.pop_front();
		threads[cur_tid]->set_state(RUNNING);
		handle_sleepers();
		timer.inc_quantums();
		timer.start_timer();
		thread->inc_quantums();
		thread->set_state(BLOCKED);
		blocked_threads.insert(tid);
		to_mask_signals(false);
		threads[cur_tid]->load_env();
	  }
	  break;
  }
  to_mask_signals(false);
  return SUCCESS;
}

int uthread_resume(int tid){
  to_mask_signals(true);

  // Sanity checks:
    if (threads.find(tid) == threads.end() || threads[tid] == nullptr){
	to_mask_signals(false);
	  return handle_err(INVALID_TID_ERR);
  }

  // Find and resume thread:
  UThread *thread = threads[tid];
  ThreadState state = thread->get_state();

  switch(state){
    case RUNNING:
        break;
    case READY:
        break;
    case SLEEPING:
        break;
    case BLOCKED:
	  thread->set_state(READY);
	  blocked_threads.erase(tid);
	  ready_threads.push_back(tid);
	  break;
    case BLOCKED_AND_SLEEPING:
	  thread->set_state(SLEEPING);
	  break;
  }
//  threads[tid]->inc_quantums();
  to_mask_signals(false);
  return SUCCESS;
}

int uthread_sleep(int num_quantums){
  to_mask_signals(true);

  // Sanity checks:
  if (num_quantums <= 0){ //invalid input check.
	to_mask_signals(false);
	  return handle_err(QUANTUM_ERR);
  }
  if (cur_tid == 0){
	to_mask_signals(false);
	  return handle_err(MAIN_THREAD_SLEEP_ERR);
  }

  // Find and make thread sleep:
  int sleeper_tid = cur_tid;
  UThread *thread = threads[sleeper_tid];
  int where = thread->env_snapshot();
  if (where != BACKED) {
	cur_tid = ready_threads.front();
	ready_threads.pop_front();
	threads[cur_tid]->set_state(RUNNING);
	handle_sleepers();
	timer.inc_quantums();
	timer.start_timer();
	thread->inc_quantums();
	thread->set_state(SLEEPING);
	thread->set_sleep_time(num_quantums);
	blocked_threads.insert(sleeper_tid);
	to_mask_signals(false);
	threads[cur_tid]->load_env();
  }
  to_mask_signals(false);
  return SUCCESS;
}

int uthread_get_tid(){
  return cur_tid;
}

int uthread_get_total_quantums(){
  return timer.get_quantums();
}

int uthread_get_quantums(int tid){
  if (threads.find(tid) == threads.end() || threads[tid] == nullptr) {
      return handle_err(INVALID_TID_ERR);
  }
  return threads[tid]->get_quantums();
}