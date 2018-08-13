#include "src/cj_login_imp.hpp"

using std::shared_ptr;
using std::string;
using std::string;

namespace cjlogin {

  shared_ptr<CjLogin> CjLogin::create() {
    return std::make_shared<CjLoginImpl>();
  }

  CjLoginImpl::CjLoginImpl() {
    this->client = new CjLoginCGIClient(grpc::CreateChannel("35.194.225.201:50052",
                                                            grpc::InsecureChannelCredentials()));
    auto t = std::thread(&CjLoginCGIClient::AsyncCompleteRpc, this->client);
    t.detach();
  }

  CjLoginImpl::~CjLoginImpl() {
    delete this->client;
  }

  void CjLoginImpl::registerUser(const string & userName,
                                 const string & password,
                                 const shared_ptr<RegisterUserCallback> & cb) {
    RegisterUserRequest request;
    request.set_username(userName);
    request.set_password(password);

    this->client->registerUser(request, [cb](void *goTag) {
        auto call = static_cast<AsyncClientCall<RegisterUserResponse>*>(goTag);
        auto errcode = call->reply.baseresp().errcode();
        auto errmsg = call->reply.baseresp().errmsg();

        cb->complete((int32_t)errcode, errmsg);
      });
  }

  void CjLoginImpl::login(const std::string & userName,
                          const std::string & password,
                          const std::shared_ptr<LoginCallback> & cb) {
    UserLoginRequest request;
    request.set_username(userName);
    request.set_password(password);
    std::cout << "doing login: " << userName << ", " << password << std::endl;
    this->client->login(request, [cb](void *goTag) {
        auto call = static_cast<AsyncClientCall<UserLoginResponse>*>(goTag);
        auto errcode = call->reply.baseresp().errcode();
        auto errmsg = call->reply.baseresp().errmsg();
        auto loginTicket = call->reply.loginticket();
        cb->complete((int32_t)errcode, errmsg, loginTicket);
      });
  }

  void CjLoginImpl::checkLogin(const std::string & userName,
                               const std::string & loginTicket,
                               const std::shared_ptr<CheckLoginCallback> & cb) {
    UserCheckLoginRequest request;
    request.set_username(userName);
    request.set_loginticket(loginTicket);

    this->client->checkLogin(request, [cb](void *goTag) {
        auto call = static_cast<AsyncClientCall<UserCheckLoginResponse>*>(goTag);
        auto errcode = call->reply.baseresp().errcode();
        auto errmsg = call->reply.baseresp().errmsg();
        auto sessionKey = call->reply.sessionkey();

        cb->complete((int32_t)errcode, errmsg, sessionKey);
      });
  }

  void CjLoginImpl::logout(const std::string & userName,
                           const std::string & sessionKey,
                           const std::shared_ptr<LogoutCallback> & cb) {
    UserLogoutRequest request;
    auto baseRequest = request.mutable_basereq();
    baseRequest->set_username(userName);
    baseRequest->set_sessionkey(sessionKey);

    this->client->logout(request, [cb](void *goTag) {
        auto call = static_cast<AsyncClientCall<UserLogoutResponse>*>(goTag);
        auto errcode = call->reply.baseresp().errcode();
        auto errmsg = call->reply.baseresp().errmsg();

        cb->complete((int32_t)errcode, errmsg);
      });
  }

  void CjLoginImpl::connect(const string & userName,
                            const string & sessionKey,
                            const std::shared_ptr<ConnectCallback> & cb) {
    ConnectRequest request;
    auto baseRequest = request.mutable_basereq();
    baseRequest->set_username(userName);
    baseRequest->set_sessionkey(sessionKey);

    this->client->connect(request, [cb](ServerMessage &message) {
        cb->complete((int32_t)message.type(), message.content());
      });
  }
}

