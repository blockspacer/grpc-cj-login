#ifndef CJ_LOGIN_REDIS_H
#define CJ_LOGIN_REDIS_H

#include <string>

#include "hiredis.h"

using std::string;

class Redis {
 public:
  Redis(string ip, int port);
  ~Redis();

  bool existsKey(string key);
  bool setData(string key, const string &data);
  bool getData(string key, string &resp);
  bool increaseKey(string key, unsigned &out); // increaseKey and return current value

 private:
  redisContext *ctx;
};

#endif
