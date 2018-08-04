#include "helper.h"
#include "src/md5.h"

namespace cjlogin {
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

}
