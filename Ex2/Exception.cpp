#include "Exception.h"

Exception::Exception(ERR_KIND kind) {
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
    case INVALID_TID_ERR:
      _message = INVALID_TID_ERR_MSG;
      _type = LIB_ERR;
      break;
    case MAIN_THREAD_BLOCK_ERR:
        _message = MAIN_THREAD_BLOCK_ERR_MSG;
        _type = LIB_ERR;
        break;
    case QUANTUM_ERR:
        _message = QUANTUM_ERR_MSG;
        _type = LIB_ERR;
        break;
    case MAIN_THREAD_SLEEP_ERR:
        _message = MAIN_THREAD_SLEEP_ERR_MSG;
        _type = LIB_ERR;
        break;
	case TIMER_ERR:
        _message = TIMER_ERR_MSG;
        _type = SYS_ERR;
        break;
	case SIGSET_ERR:
	    _message = SIGSET_ERR_MSG;
	    _type = SYS_ERR;
	    break;
	case SIGACT_ERR:
	    _message = SIGACT_ERR_MSG;
	    _type = SYS_ERR;
	    break;
	case MEM_ERR:
	    _message = MEM_ERR_MSG;
	    _type = SYS_ERR;
	}

}
void Exception::print_error() {
	flush(cerr << "Error: " << _message << endl);
}

ERR_TYPE Exception::get_type() {
    return _type;
}

