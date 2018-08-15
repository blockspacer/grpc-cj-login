#include <ctime>
#include <iostream>
#include <sstream>
#include <vector>

#include "helper.h"
#include "src/md5.h"
#include "include/bcrypt/BCrypt.hpp"
#include "source/duthomhas/csprng.hpp"
#include <glog/logging.h>

using namespace jwt::params;

auto jwtLoginTicketKey = "jwtLoginTicketKey";
auto jwtSessionKeyKey = "jwtSessionKeyKey";

namespace cjLogin {
  string getPasswordSalt() {
    duthomhas::csprng rng;
    return rng(string(20, 0));
  }

  string genPassword(string raw, string salt) {
    BCrypt bcrypt;
    string pwd = raw + salt;
    return bcrypt.generateHash(pwd);
  }

  bool validateUsername(string username) {
    for (const auto &ch : username) {
      if (ch != '_'
          && !(ch >= '0' && ch <= '9')
          && !(ch >= 'a' && ch <= 'z')
          && !(ch >= 'A' && ch <= 'Z')) {
        return false;
      }
    }

    return true;
  }

  bool validatePassword(string password) {
    if (password.size() < 6) {
      return false;
    }

    size_t numberCount = 0, charCount = 0;
    for (const auto &ch: password) {
      if (ch >= '0' && ch <= '9') {
        numberCount++;
      } else if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) {
        charCount++;
      }
    }

    return password.size() != numberCount && password.size() != charCount;
  }

  string genJWTToken(string username, string uin, const char *key) {
    std::time_t ts = std::time(nullptr);
    jwt::jwt_object obj {
      secret(key),
        algorithm("hs256"),
        payload({
            {"X-uin", uin}, {"X-username", username}, {"ts", std::to_string(ts)}
          })
        };
    return obj.signature();
  }

  bool extraJWTToken(string token, PayloadInfo &payload, const char *key) {
    std::error_code ec;
    auto decObj = jwt::decode(token,
                              algorithms({"hs256"}),
                              ec,
                              secret(key));
    if (ec) {
      LOG(ERROR) << "extraJWTToken fail, errcode: " << ec;
      return false;
    }

    auto json = decObj.payload().create_json_obj();
    payload.uin = json["X-uin"];
    payload.userName = json["X-username"];
    string ts = json["ts"];
    payload.ts = atol(ts.c_str());

    LOG(INFO) << "extraJWTToken Success, [uin]" << payload.uin;
    return true;
  }

  string genLoginTicket(string username, string uin) {
    return genJWTToken(username, uin, jwtLoginTicketKey);
  }

  bool extraLoginTicket(string loginTicket, PayloadInfo &payload) {
    return extraJWTToken(loginTicket, payload, jwtLoginTicketKey);
  }

  string genSessionKey(string username, string uin) {
    return genJWTToken(username, uin, jwtSessionKeyKey);
  }

  bool extraSessionKey(string sessionKey, PayloadInfo &payload) {
    return extraJWTToken(sessionKey, payload, jwtSessionKeyKey);
  }
}
