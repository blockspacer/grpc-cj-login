#include "proto/cj_login.grpc.pb.h"
#include <grpc++/channel.h>
#include <grpc++/client_context.h>
#include <grpc++/create_channel.h>
#include <grpc++/security/credentials.h>

#include <memory>
#include <functional>

using cjLogin::RegisterUserRequest;
using cjLogin::RegisterUserResponse;
using cjLogin::UserLoginRequest;
using cjLogin::UserLoginResponse;
using cjLogin::UserCheckLoginRequest;
using cjLogin::UserCheckLoginResponse;
using cjLogin::UserLogoutRequest;
using cjLogin::UserLogoutResponse;
using cjLogin::LogoutUserRequest;
using cjLogin::LogoutUserResponse;
using cjLogin::CjLoginService;
using cjLogin::InternalConnectRequest;
using cjLogin::InternalMessage;

using grpc::Channel;
using grpc::Status;

typedef std::function<void(InternalMessage &)> InternalMessageHandler;

class CjLoginClient {

public:
  CjLoginClient(std::shared_ptr<Channel> channel);

  Status registerUser(const RegisterUserRequest &req,
                      RegisterUserResponse *resp);

  Status login(const UserLoginRequest &req,
               UserLoginResponse *resp);

  Status checkLogin(const UserCheckLoginRequest &req,
                    UserCheckLoginResponse *resp);

  Status logout(const UserLogoutRequest &req,
                UserLogoutResponse *resp);

  Status logoutUser(const LogoutUserRequest &req,
                    LogoutUserResponse *resp);

  void internalConnect(const InternalConnectRequest &req,
                       InternalMessageHandler handler);

private:
  std::unique_ptr<CjLoginService::Stub> stub_;

  void readMessage(const InternalConnectRequest &req,
                   InternalMessageHandler handler);
};


