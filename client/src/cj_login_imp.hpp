#pragma once

#include "djinni-src/cpp/cj_login.hpp"
#include "src/cj_login_cgi_client.hpp"

namespace cjlogin {
    class CjLoginImpl : public cjlogin::CjLogin {
    public:
        CjLoginImpl();
        
        void registerUser(const std::string & userName, const std::string & password, const std::shared_ptr<RegisterUserCallback> & cb);
    };
}
