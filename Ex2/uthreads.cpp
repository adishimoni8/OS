#include <cstdlib>
#include "uthreads.h"
#include <csignal>
#include <deque>
#include <unordered_set>
#include <map>
#include "Exception.h"
#include "UThread.h"
#include "EnvHelpers.h"
#include "QuantumTimer.h"
using namespace std;

// Macros:
#define BACKED 1

//Global Variables:
std::map<int, UThread *> threads;
deque<int> ready_threads;
unordered_set<int> blocked_threads;
struct sigaction sa;
QuantumTimer timer;
int cur_tid;
int available_tid;
int num_of_threads;

//Utils:
static void erase_element_from_queue(deque<int> *queue, int tid){
    for (auto it = queue->begin(); it != queue->end(); it++){
        if (*it == tid){
            queue->erase(it);
            break;
        }
    }
}

static int free_all(){
    for (int i = 0; i < threads.size(); ++i){
        delete threads[i];
    }
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
        free_all();
        exit(EXIT_FAILURE);
    }
    return ERROR;
}

static int to_mask_sigvtalrm(bool mask){
    if (mask){
        if (sigprocmask(SIG_BLOCK, &sa.sa_mask, nullptr) != SUCCESS){
            return handle_err(SIGSET_ERR);
        }
    } else{
        if (sigprocmask(SIG_UNBLOCK, &sa.sa_mask, nullptr) != SUCCESS){
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

//API
int uthread_init(int quantum_usecs){
  if (quantum_usecs <= 0){ //invalid input check.
    return handle_err(QUANTUM_ERR);
  }
  timer = QuantumTimer(quantum_usecs);
  sa.sa_handler = &sigvtalrm_handler;
  sa.sa_flags = 0;
  if (sigaction(SIGVTALRM, &sa, nullptr) != SUCCESS){ //masks SIGVTALRM.
    return handle_err(SIGACT_ERR);
  }
  available_tid = 0;
  auto* main_thread = new (nothrow) UThread(0); // define main thread with tid 0
  if (main_thread == nullptr){
    return handle_err(MEM_ERR);
  }
  threads[0] = main_thread;
  ready_threads.push_back(available_tid);
  available_tid = 1;
  timer.start_timer();
  return SUCCESS;
}

int uthread_spawn(thread_entry_point entry_point){
  to_mask_sigvtalrm(true);
  if ((*entry_point) == nullptr){ //invalid input check.
    return handle_err(INVALID_FUNC_ERR);
  }
  if (num_of_threads >= MAX_THREAD_NUM){ //overflow check.
      return handle_err(MAX_THREAD_ERR);
  }
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
  int i = available_tid + 1;
    while (threads[i] != nullptr){
        ++i;
    }
  available_tid = i;
  num_of_threads++;
  to_mask_sigvtalrm(false);
  return tid;
}

int uthread_terminate(int tid){
    to_mask_sigvtalrm(true);
    if (threads.find(tid) == threads.end()){
        to_mask_sigvtalrm(false);
        return handle_err(INVALID_TID_ERR);
    }
    if (tid == 0){
        free_all();
        exit(EXIT_SUCCESS);
    }
    UThread *thread = threads[tid];
    ThreadState state = thread->get_state();
    delete thread;
    threads[tid] = nullptr;
    if (available_tid > tid){
        available_tid = tid;
    }
    if (state == READY){
        erase_element_from_queue(&ready_threads,tid);
    } else if (state == BLOCKED || state == SLEEPING || state == BLOCKED_AND_SLEEPING){
        blocked_threads.erase(tid);
    } else if (state == RUNNING) {
        cur_tid = ready_threads.front();
        ready_threads.pop_front();
        threads[cur_tid]->set_state(RUNNING);
        handle_sleepers();
        timer.inc_quantums();
        timer.start_timer();
        to_mask_sigvtalrm(false);
        num_of_threads--;
        threads[cur_tid]->load_env();
    }
    num_of_threads--;
    to_mask_sigvtalrm(false);
    return SUCCESS;
}

int uthread_block(int tid){
    to_mask_sigvtalrm(true);
    if (threads.find(tid) == threads.end()){
        to_mask_sigvtalrm(false);
        return handle_err(INVALID_TID_ERR);
    }
    if (tid == 0){
        to_mask_sigvtalrm(false);
        return handle_err(MAIN_THREAD_BLOCK_ERR);
    }
    UThread *thread = threads[tid];
    ThreadState state = thread->get_state();
    if (state == READY){
        erase_element_from_queue(&ready_threads,tid);
        thread->set_state(BLOCKED);
        blocked_threads.insert(tid);
    } else if (state == BLOCKED || state == BLOCKED_AND_SLEEPING){
        return SUCCESS;
    } else if (state == SLEEPING){
        thread->set_state(BLOCKED_AND_SLEEPING);
    } else if (state == RUNNING) {
        cur_tid = ready_threads.front();
        ready_threads.pop_front();
        thread->set_state(BLOCKED);
        threads[cur_tid]->set_state(RUNNING);
        handle_sleepers();
        timer.inc_quantums();
        timer.start_timer();
        thread->inc_quantums();
        thread->env_snapshot();
        threads[cur_tid]->load_env();
    }
    to_mask_sigvtalrm(false);
    return SUCCESS;
}

int uthread_resume(int tid){
    to_mask_sigvtalrm(true);
    if (threads.find(tid) == threads.end()){
        to_mask_sigvtalrm(false);
        return handle_err(INVALID_TID_ERR);
    }
    UThread *thread = threads[tid];
    ThreadState state = thread->get_state();
    if (state == BLOCKED){
        thread->set_state(READY);
        blocked_threads.erase(tid);
        ready_threads.push_back(tid);
    } else if (state == BLOCKED_AND_SLEEPING){
        thread->set_state(SLEEPING);
    }
    to_mask_sigvtalrm(false);
    return SUCCESS;
}

int uthread_sleep(int num_quantums){
    to_mask_sigvtalrm(true);
    if (num_quantums <= 0){ //invalid input check.
        to_mask_sigvtalrm(false);
        return handle_err(QUANTUM_ERR);
    }
    if (cur_tid == 0){
        to_mask_sigvtalrm(false);
        return handle_err(MAIN_THREAD_SLEEP_ERR);
    }
    UThread *thread = threads[cur_tid];
    int tid = cur_tid;
    cur_tid = ready_threads.front();
    ready_threads.pop_front();
    threads[cur_tid]->set_state(RUNNING);
    handle_sleepers();
    timer.inc_quantums();
    timer.start_timer();
    thread->inc_quantums();
    thread->set_state(SLEEPING);
    thread->set_sleep_time(num_quantums);
    blocked_threads.insert(tid);
    thread->env_snapshot();
    threads[cur_tid]->load_env();
    to_mask_sigvtalrm(false);
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