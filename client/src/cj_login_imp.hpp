#pragma once

#include "cj_login.hpp"

namespace cjlogin {
    class CjLoginImpl : public cjlogin::CjLogin {
    public:
        CjLoginImpl();
        
        void registerUser(const std::string & userName, const std::string & password, const std::shared_ptr<RegisterUserCallback> & cb);
    };
}
