#include "redis.h"
#include <iostream>
#include <memory>

Redis::Redis(string ip, int port) {
  this->ctx = redisConnect(ip.c_str(), port);
  if(this->ctx == NULL) {
    std::cout << "redisConnect return NULL ctx" << std::endl;
    //throw runtime_error("redisConnect return NULL ctx");
    return;
  }

  if (this->ctx->err) {
    std::cout << "redisConnect error: " << this->ctx->err << std::endl;
    //throw runtime_error("redisConnect error");
    return;
  }
}

Redis::~Redis() {
  redisFree(this->ctx);
}

bool Redis::setData(string key, const string &data) {
  if (this->ctx == NULL) {
    return false;
  }

  if (key.size() == 0) {
    return false;
  }

  auto reply = (redisReply *)redisCommand(this->ctx,
                                          "SET %s %s",
                                          key.c_str(), data.c_str());
  if (reply == NULL) {
    std::cout << "setByte: " << key << ", redisReply error" << std::endl;
    return false;
  }

  if (reply->type == REDIS_REPLY_ERROR) {
    std::cout << "setByte: " << key
             << ", redisReply error: " << reply->str
             << std::endl;
    freeReplyObject(reply);
    return false;
  }

  freeReplyObject(reply);
  return true;
}

bool Redis::getData(string key, string &resp) {
  if (this->ctx == NULL) {
    return false;
  }

  if (key.size() == 0) {
    return false;
  }

  auto reply = (redisReply *)redisCommand(this->ctx, "GET %s", key.c_str());
  if (reply == NULL) {
    std::cout << "getByte: " << key << ", redisReply error" << std::endl;
    return false;
  }

  if (reply->type == REDIS_REPLY_ERROR) {
    std::cout << "getByte: " << key
             << ", redisReply error: " << reply->str
             << std::endl;
    freeReplyObject(reply);
    return false;
  }

  resp.assign(reply->str, reply->len);
  freeReplyObject(reply);
  return true;
}

bool Redis::existsKey(string key) {
  if (this->ctx == NULL) {
    return false;
  }

  if (key.size() == 0) {
    return false;
  }

  auto reply = (redisReply *)redisCommand(this->ctx, "exists %s", key.c_str());
  if (reply == NULL) {
    return false;
  }

  if (reply->type == REDIS_REPLY_INTEGER) {
    if (reply->integer == 1) {
      return true;
    }
  }

  return false;
}

bool Redis::increaseKey(string key, unsigned &out) {
  if (this->ctx == NULL) {
    return false;
  }

  if (key.size() == 0) {
    return false;
  }

  auto reply = (redisReply *)redisCommand(this->ctx, "INCR %s", key.c_str());
  if (reply == NULL) {
    return false;
  }

  if (reply->type == REDIS_REPLY_INTEGER) {
    out = reply->integer;
    return true;
  }

  return false;
}
