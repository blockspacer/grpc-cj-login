#pragma once

#include "generated-src/cpp/cj_login.hpp"

using cjlogin::CjLogin;

namespace cjlogin {
  class CjLoginImpl : public CjLogin {
  public:
    CjLoginImpl();
  };
}
