#include "proto/cj_login.grpc.pb.h"
#include <grpc++/channel.h>
#include <grpc++/client_context.h>
#include <grpc++/create_channel.h>
#include <grpc++/security/credentials.h>

#include <memory>

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
using grpc::Status;

class CjLoginClient {

public:
  CjLoginClient(std::shared_ptr<Channel> channel);

  Status registerUser(RegisterUserRequest &req,
                      RegisterUserResponse *resp);

  Status login(const UserLoginRequest &req,
               UserLoginResponse *resp);

  Status checkLogin(const UserCheckLoginRequest &req,
                    UserCheckLoginResponse *resp);

  Status logout(const UserLogoutRequest &req,
                UserLogoutResponse *resp);

private:
  std::unique_ptr<CjLoginService::Stub> stub_;
};


