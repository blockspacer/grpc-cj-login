#include "callback_impl.hpp"

using cjlogin::RegisterUserCallbackImpl;

namespace cjlogin {

  RegisterUserCallbackImpl::RegisterUserCallbackImpl(function<void(int32_t,string)> cb) {
    this->_cb = cb;
  }

  void RegisterUserCallbackImpl::complete(int32_t errcode,
                                          const string & errmsg) {
    this->_cb(errcode, errmsg);
  }
}
