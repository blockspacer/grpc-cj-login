#ifndef GRPC_CJ_LOGIN_HELPER
#define GRPC_CJ_LOGIN_HELPER

#include <string>
#include "src/md5.h"

using std::string;

typedef struct {
  string userName;
  string uin;
  long ts;
} PayloadInfo;

namespace cjLogin {
  bool validateUsername(string username);
  bool validatePassword(string password);

  string genLoginTicket(string username, string uin);
  bool extraLoginTicket(string loginTicket, PayloadInfo &payload);

  inline string md5(string raw) {
    return MD5(raw).toStr();
  }
}

#endif
