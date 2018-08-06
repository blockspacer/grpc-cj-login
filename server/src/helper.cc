#include <ctime>
#include <iostream>
#include <sstream>
#include <vector>

#include "helper.h"
#include "src/md5.h"
#include "include/jwt/jwt.hpp"

using namespace jwt::params;

auto jwtLoginTicketKey = "__cj_login_jwt_key__login_ticket_key__";

namespace cjLogin {
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

  string genLoginTicket(string username, string uin) {
    // TODO: JWT here
    std::time_t ts = std::time(nullptr);
    jwt::jwt_object obj {
      secret(jwtLoginTicketKey),
      algorithm("hs256"),
      payload({
          {"X-uin", uin}, {"X-username", username}, {"ts", std::to_string(ts)}
        })
    };
    return obj.signature();
  }

  bool extraLoginTicket(string loginTicket, PayloadInfo &payload) {
    std::error_code ec;

    auto decObj = jwt::decode(loginTicket,
                              algorithms({"hs256"}),
                              ec,
                              secret(jwtLoginTicketKey));
    if (ec) {
      return false;
    }

    auto json = dec_obj.payload();
    payload.uin = json["X-uin"];
    payload.username = json["X-username"];
    payload.ts = json["ts"];
    return true;
  }
}
