#include <ctime>
#include <iostream>
#include <sstream>
#include <vector>

#include "helper.h"
#include "src/md5.h"
#include "include/jwt/jwt.hpp"

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

  string genLoginTicket(string username, string uin) {
    // TODO: JWT here
    std::time_t ts = std::time(nullptr);
    return username + " " + uin + " " + std::to_string(ts);
  }

  bool extraLoginTicket(string loginTicket, PayloadInfo &payload) {
    std::stringstream ss(loginTicket);
    std::vector<string> v;
    string t;
    while (ss >> t) {
      v.push_back(t);
    }
    if (v.size() != 3) {
      return false;
    }

    payload.userName = v[0];
    payload.uin = v[1];
    payload.ts = atol(v[2].c_str());
    return true;
  }
}
