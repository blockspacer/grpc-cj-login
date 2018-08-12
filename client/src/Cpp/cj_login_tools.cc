#include "src/cj_login_imp.hpp"
#include "src/callback_impl.hpp"

using cjlogin::CjLoginImpl;
using cjlogin::RegisterUserCallbackImpl;
using cjlogin::LoginCallbackImpl;
using cjlogin::CheckLoginCallbackImpl;
using cjlogin::LogoutCallbackImpl;

int main(int argc, char** argv) {

  CjLoginImpl cjLogin;

  std::cout << "Usage: <function> arg1 arg2" << std::endl;
  std::cout << "e.g. register jeason password1" << std::endl;

  string func;
  while (std::cin >> func) {
    if (func == "register") {
      string userName, password;
      std::cin >> userName >> password;
      auto cb = std::make_shared<RegisterUserCallbackImpl>
        ([](int32_t errcode, const string errmsg) {
          std::cout << errcode << ", " << errmsg << std::endl;
        });
      cjLogin.registerUser(userName, password, cb);
    } else if (func == "login") {
      string userName, password;
      std::cin >> userName >> password;
      auto cb = std::make_shared<LoginCallbackImpl>
        ([](int32_t errcode, const string errmsg, const string loginTicket) {
          std::cout << errcode << ", " << errmsg << std::endl;
          std::cout << "loginTicket: " << loginTicket << std::endl;
        });
      cjLogin.login(userName, password, cb);
    } else if (func == "checkLogin") {
      string userName, loginTicket;
      std::cin >> userName >> loginTicket;
      auto cb = std::make_shared<CheckLoginCallbackImpl>
        ([](int32_t errcode, const string errmsg, const string sessionKey) {
          std::cout << errcode << ", " << errmsg << std::endl;
          std::cout << "sessionKey: " << sessionKey << std::endl;
        });
      cjLogin.checkLogin(userName, loginTicket, cb);
    } else if (func == "logout") {
      string userName, sessionKey;
      std::cin >> userName >> sessionKey;
      auto cb = std::make_shared<LogoutCallbackImpl>
        ([](int32_t errcode, const string errmsg) {
          std::cout << errcode << ", " << errmsg << std::endl;
        });
      cjLogin.logout(userName, sessionKey, cb);
    } else if (func == "exit") {
      break;
    }
  }

  return 0;
}
