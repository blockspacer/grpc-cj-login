#pragma once

#include "registerUserCallback.hpp"
#include <functional>
#include <string>

using cjlogin::RegisterUserCallback;
using std::string;
using std::function;

namespace cjLogin {

  class RegisterUserCallbackImpl: public RegisterUserCallback {
  public:
    RegisterUserCallbackImpl(function<void(int32_t, string)> cb);

  private:
    function<void(int32_t, string)> _cb;
  };
}
