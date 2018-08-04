#include "redis_table.h"

RedisTable::RedisTable(Redis *redis, string table) {
  this->redis = redis;
  this->table = table + "-";
}

RedisTable::~RedisTable() {

}

bool RedisTable::existsKey(string key) {
  return this->redis->existsKey(this->table + key);
}

bool RedisTable::setData(string key, const string &data) {
  return this->redis->setData(this->table + key, data);
}

bool RedisTable::getData(string key, string &resp) {
  return this->redis->getData(this->table + key, resp);
}

bool RedisTable::increaseKey(string key, unsigned &out) {
  return this->redis->increaseKey(this->table + key, out);
}
