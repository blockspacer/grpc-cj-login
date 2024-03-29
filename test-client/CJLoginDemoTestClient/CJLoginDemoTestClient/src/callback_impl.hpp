#pragma once

#include "djinni-src/registerUserCallback.hpp"
#include "djinni-src/loginCallback.hpp"
#include "djinni-src/checkLoginCallback.hpp"
#include "djinni-src/logoutCallback.hpp"
#include "djinni-src/connectCallback.hpp"

#include <functional>
#include <string>

using cjlogin::RegisterUserCallback;
using cjlogin::LoginCallback;
using cjlogin::CheckLoginCallback;
using cjlogin::LogoutCallback;
using std::string;
using std::function;

namespace cjlogin {

  class RegisterUserCallbackImpl: public RegisterUserCallback {
  public:
    RegisterUserCallbackImpl(function<void(int32_t, string)> cb);

    void complete(int32_t errcode, const string & errmsg);

  private:
    function<void(int32_t, string)> _cb;
  };

  class LoginCallbackImpl: public LoginCallback {
  public:
    LoginCallbackImpl(function<void(int32_t, string, string)> cb);

    void complete(int32_t errcode, const string & errmsg, const string &loginTicket);
  private:
    function<void(int32_t, string, string)> _cb;
  };

  class CheckLoginCallbackImpl: public CheckLoginCallback {
  public:
    CheckLoginCallbackImpl(function<void(int32_t, string, string)> cb);

    void complete(int32_t errcode, const string & errmsg, const string &sessionKey);
  private:
    function<void(int32_t, string, string)> _cb;
  };

  class LogoutCallbackImpl: public LogoutCallback {
  public:
    LogoutCallbackImpl(function<void(int32_t, string)> cb);

    void complete(int32_t errcode, const string & errmsg);
  private:
    function<void(int32_t, string)> _cb;
  };

  class ConnectCallbackImpl: public ConnectCallback {
  public:
    ConnectCallbackImpl(function<void(int32_t, string)> cb);

    void complete(int32_t messageType, const string & content);
  private:
    function<void(int32_t, string)> _cb;
  };
}
