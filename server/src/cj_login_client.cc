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

  Status registerUser(string username,
                      string password) {
    RegisterUserRequest req;
    req.set_username(username);
    req.set_password(password);

    RegisterUserResponse res;
    ClientContext context;
    auto status = stub_->registerUser(&context, req, &res);
    std::cout << res.baseresp().errcode() << res.baseresp().errmsg() << std::endl;
    return status;
  }

  Status userLogin(string username, string password) {
    UserLoginRequest req;
    req.set_username(username);
    req.set_password(password);

    UserLoginResponse resp;
    ClientContext context;
    auto status = stub_->userLogin(&context, req, &resp);
    std::cout << resp.baseresp().errcode()
              << resp.baseresp().errmsg()
              << resp.loginticket()
              << std::endl;
    return status;
  }

private:
  std::unique_ptr<CjLoginService::Stub> stub_;
};

int main(int argc, char *argv[]) {
  CjLoginClient client(CreateChannel("localhost:50051",
                                     InsecureChannelCredentials()));

  int i = 1;
  string v = string(argv[i]);
  if (v == "-f") {
    string func(argv[i + 1]);

    Status status;
    if (func == "register") {
      string userName(argv[i + 2]);
      string password(argv[i + 3]);
      status = client.registerUser(userName, password);
    } else if (func == "login") {
      string userName(argv[i + 2]);
      string password(argv[i + 3]);
      status = client.userLogin(userName, password);
    }
  }


  return 0;
}
