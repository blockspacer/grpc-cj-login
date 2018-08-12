#include "cj_login_imp.hpp"

namespace cjlogin {
    
    std::shared_ptr<CjLogin> CjLogin::create() {
        return std::make_shared<CjLoginImpl>();
    }
    
    CjLoginImpl::CjLoginImpl() {
    }
    
    void CjLoginImpl::registerUser(const std::string & userName, const std::string & password, const std::shared_ptr<RegisterUserCallback> & cb) {
    }
    
}
