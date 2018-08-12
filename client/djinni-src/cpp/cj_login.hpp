// AUTOGENERATED FILE - DO NOT MODIFY!
// This file generated by Djinni from cj-login.djinni

#pragma once

#include <memory>
#include <string>

namespace cjlogin {

class CheckLoginCallback;
class LoginCallback;
class LogoutCallback;
class RegisterUserCallback;

class CjLogin {
public:
    virtual ~CjLogin() {}

    static std::shared_ptr<CjLogin> create();

    virtual void registerUser(const std::string & userName, const std::string & password, const std::shared_ptr<RegisterUserCallback> & cb) = 0;

    virtual void login(const std::string & userName, const std::string & password, const std::shared_ptr<LoginCallback> & cb) = 0;

    virtual void checkLogin(const std::string & userName, const std::string & loginTicket, const std::shared_ptr<CheckLoginCallback> & cb) = 0;

    virtual void logout(const std::string & userName, const std::string & sessionKey, const std::shared_ptr<LogoutCallback> & cb) = 0;
};

}  // namespace cjlogin
