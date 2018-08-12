#pragma once

#include "djinni-src/cpp/registerUserCallback.hpp"
#include <functional>
#include <string>

using cjlogin::RegisterUserCallback;
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
}
