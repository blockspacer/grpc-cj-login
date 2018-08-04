#include "proto/cj_login.grpc.pb.h"
#include <grpc++/channel.h>
#include <grpc++/client_context.h>
#include <grpc++/create_channel.h>
#include <grpc++/security/credentials.h>
#include <iostream>
#include <memory>
#include <string>

using std::string;

using cjLogin::RegisterUserRequest;
using cjLogin::RegisterUserResponse;
using cjLogin::UserLoginRequest;
using cjLogin::UserLoginResponse;
using cjLogin::UserCheckLoginRequest;
using cjLogin::UserCheckLoginResponse;
using cjLogin::UserLogoutRequest;
using cjLogin::UserLogoutResponse;
using cjLogin::CjLoginService;

using grpc::Channel;
using grpc::ClientContext;
using grpc::CreateChannel;
using grpc::InsecureChannelCredentials;
using grpc::Status;

class CjLoginClient {

public:
  CjLoginClient(std::shared_ptr<Channel> channel)
    : stub_(CjLoginService::NewStub(channel)) {}

  Status registerUser(RegisterUserRequest &req,
                      RegisterUserResponse *resp) {
    ClientContext context;
    return stub_->registerUser(&context, req, resp);
  }

  Status login(const UserLoginRequest &req,
                   UserLoginResponse *resp) {
    ClientContext context;
    return stub_->userLogin(&context, req, resp);
  }

  Status checkLogin(const UserCheckLoginRequest &req,
                    UserCheckLoginResponse *resp) {
    ClientContext context;
    return stub_->userCheckLogin(&context, req, resp);
  }

  Status logout(const UserLogoutRequest &req,
                UserLogoutResponse *resp) {
    ClientContext context;
    return stub_->userLogout(&context, req, resp);
  }

private:
  std::unique_ptr<CjLoginService::Stub> stub_;
};

int main(int argc, char *argv[]) {
  CjLoginClient client(CreateChannel("localhost:50051",
                                     InsecureChannelCredentials()));

  if (argc != 5) {
    std::out << "params error: (" << argc << ")" << std::endl;
    return 0;
  }

  int i = 1;
  string v(argv[i]);
  if (v == "-f") {
    string func(argv[i + 1]);

    Status status;
    if (func == "register") {
      string userName(argv[i + 2]);
      string password(argv[i + 3]);

      RegisterUserRequest req;
      RegisterUserResponse resp;
      req.set_username(userName);
      req.set_password(password);

      status = client.registerUser(req, &resp);
    } else if (func == "login") {
      string userName(argv[i + 2]);
      string password(argv[i + 3]);

      UserLoginRequest req;
      UserLoginResponse resp;
      req.set_username(userName);
      req.set_password(password);

      status = client.login(req, &resp);
      std::cout << resp.loginticket() << std::endl;
    } else if (func == "checkLogin") {
      string userName(argv[i + 2]);
      string loginTicket(argv[i + 3]);

      UserCheckLoginRequest req;
      UserCheckLoginResponse resp;
      req.set_username(userName);
      req.set_loginticket(loginTicket);

      status = client.checkLogin(req, &resp);
      std::cout << resp.sessionkey() << std::endl;
    } else if (func == "logout") {
      string userName(argv[i + 2]);
      string sessionKey(argv[i + 3]);

      UserLogoutRequest req;
      auto baseReq = req.mutable_basereq();
      UserLogoutResponse resp;
      baseReq->set_username(userName);
      baseReq->set_sessionkey(sessionKey);

      status = client.logout(req, &resp);
    }
  }


  return 0;
}
