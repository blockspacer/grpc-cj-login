#include "callback_impl.hpp"

using cjlogin::RegisterUserCallbackImpl;
using cjlogin::LoginCallbackImpl;
using cjlogin::CheckLoginCallbackImpl;
using cjlogin::LogoutCallbackImpl;

namespace cjlogin {

  // RegisterUser //
  RegisterUserCallbackImpl::RegisterUserCallbackImpl(function<void(int32_t,string)> cb) {
    this->_cb = cb;
  }

  void RegisterUserCallbackImpl::complete(int32_t errcode,
                                          const string & errmsg) {
    this->_cb(errcode, errmsg);
  }

  // Login //
  LoginCallbackImpl::LoginCallbackImpl(function<void(int32_t,string,string)> cb) {
    this->_cb = cb;
  }

  void LoginCallbackImpl::complete(int32_t errcode,
                                   const string & errmsg,
                                   const string & loginTicket) {
    this->_cb(errcode, errmsg, loginTicket);
  }

  // CheckLogin //
  CheckLoginCallbackImpl::CheckLoginCallbackImpl(function<void(int32_t,string,string)> cb) {
    this->_cb = cb;
  }

  void CheckLoginCallbackImpl::complete(int32_t errcode,
                                        const string & errmsg,
                                        const string & sessionKey) {
    this->_cb(errcode, errmsg, sessionKey);
  }

  // Logout //
  LogoutCallbackImpl::LogoutCallbackImpl(function<void(int32_t,string)> cb) {
    this->_cb = cb;
  }

  void LogoutCallbackImpl::complete(int32_t errcode,
                                    const string & errmsg) {
    this->_cb(errcode, errmsg);
  }
}
