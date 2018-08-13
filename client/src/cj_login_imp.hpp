#pragma once

#include "djinni-src/cj_login.hpp"
#include "src/callback_impl.hpp"
#include "src/cj_login_cgi_client.hpp"

namespace cjlogin {
  class CjLoginImpl : public cjlogin::CjLogin {
  public:
    CjLoginImpl();
    ~CjLoginImpl();

    void registerUser(const std::string & userName,
                      const std::string & password,
                      const std::shared_ptr<RegisterUserCallback> & cb);

    void login(const std::string & userName,
               const std::string & password,
               const std::shared_ptr<LoginCallback> & cb);

    void checkLogin(const std::string & userName,
                    const std::string & loginTicket,
                    const std::shared_ptr<CheckLoginCallback> & cb);

    void logout(const std::string & userName,
                const std::string & sessionKey,
                const std::shared_ptr<LogoutCallback> & cb);

    void connect(const std::string & userName,
                 const std::string & sessionKey,
                 const std::shared_ptr<ConnectCallback> &cb);

  private:
    CjLoginCGIClient *client;
  };
}
