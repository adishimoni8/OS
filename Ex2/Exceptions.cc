#include "Exceptions.h"

Exceptions::Exceptions(ERR_KIND kind) {
  	_kind = kind;
	switch(kind){
	case INVALID_FUNC_ERR:
	  _message = INVALID_FUNC_ERR_MSG;
	  _type = LIB_ERR;
	  break;
	case MAX_THREAD_ERR:
	  _message = MAX_THREAD_ERR_MSG;
	  _type = LIB_ERR;
	  break;
	case TIMER_ERR:
	  _message = TIMER_ERR_MSG;
	  _type = LIB_ERR;
	  break;
	case QUANTUM_ERR:
	  _message = QUANTUM_ERR_MSG;
	  _type = SYS_ERR;
	  break;
	case SIGSET_ERR:
	  _message = SIGSET_ERR_MSG;
	  _type = SYS_ERR;
	  break;
	case SIGACT_ERR:
	  _message = SIGACT_ERR_MSG;
	  _type = SYS_ERR;
	}

}
void Exceptions::print_error() {
	cerr << _message << endl;
}

