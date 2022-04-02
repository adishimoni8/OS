#include "UThread.h"

UThread::~UThread() = default;

UThread::UThread(int tid){
  _tid = tid;
  _state = READY;
  _quantums = 0;
  _quantums_to_sleep = 0;
}


int UThread::get_tid() const{
	return _tid;
}

ThreadState UThread::get_state() const{
	return _state;
}

void UThread::set_state(ThreadState state){
	_state = state;
}

int UThread::get_quantums() const{
	return _quantums;
}

void UThread::inc_quantums(){
	_quantums++;
}

sigjmp_buf& UThread::get_env(){
  return _env;
}

int UThread::env_snapshot(){
	return sigsetjmp(_env,1);
}

void UThread::load_env(){
  siglongjmp(_env,1);
}

char *UThread::get_stack() {
  return _stack;
}

void UThread::set_sleep_time(int quantums) {
    _quantums_to_sleep = quantums;
}

int UThread::get_sleep_time() {
    return _quantums_to_sleep;
}

void UThread::dec_sleep_time() {
    _quantums_to_sleep--;
}


