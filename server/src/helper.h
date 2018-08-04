#ifndef GRPC_CJ_LOGIN_HELPER
#define GRPC_CJ_LOGIN_HELPER

#include <string>
#include "src/md5.h"

using std::string;

namespace cjlogin {
  bool validateUsername(string username);
  inline string md5(string raw) {
    return MD5(raw).toStr();
  }
}

#endif
