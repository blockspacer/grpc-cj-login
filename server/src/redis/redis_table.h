#ifndef CJ_LOGIN_REDIS_TABLE_H
#define CJ_LOGIN_REDIS_TABLE_H

#include <string>
#include "redis.h"

using std::string;

class RedisTable {
 public:
  RedisTable(Redis *redis, string table);
  ~RedisTable();

  bool existsKey(string key);
  bool setData(string key, const string &data);
  bool getData(string key, string &resp);
  bool increaseKey(string key, unsigned &out); // increaseKey and return current value

 private:
  string table;
  Redis *redis;
};

#endif
