#include "cj_login_client.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::CreateChannel;
using grpc::InsecureChannelCredentials;
using grpc::Status;

CjLoginClient::CjLoginClient(std::shared_ptr<Channel> channel)
  : stub_(CjLoginService::NewStub(channel)) {}

Status CjLoginClient::registerUser(const RegisterUserRequest &req,
                                   RegisterUserResponse *resp) {
  ClientContext context;
  return stub_->registerUser(&context, req, resp);
}

Status CjLoginClient::login(const UserLoginRequest &req,
                            UserLoginResponse *resp) {
  ClientContext context;
  return stub_->userLogin(&context, req, resp);
}

Status CjLoginClient::checkLogin(const UserCheckLoginRequest &req,
                                 UserCheckLoginResponse *resp) {
  ClientContext context;
  return stub_->userCheckLogin(&context, req, resp);
}

Status CjLoginClient::logout(const UserLogoutRequest &req,
                             UserLogoutResponse *resp) {
  ClientContext context;
  return stub_->userLogout(&context, req, resp);
}
