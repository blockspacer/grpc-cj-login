#include "proto/cj_login.grpc.pb.h"
#include <grpc++/channel.h>
#include <grpc++/client_context.h>
#include <grpc++/create_channel.h>
#include <grpc++/security/credentials.h>

#include <memory>

using cjLogin::CjLoginPushService;
using cjLogin::LogoutUserRequest;
using cjLogin::LogoutUserResponse;
using cjLogin::ConnectRequest;
using cjLogin::ServerMessage;
using cjLogin::MessageType;

using grpc::Channel;
using grpc::Status;
using grpc::Channel;
using grpc::ClientContext;
using grpc::CreateChannel;
using grpc::InsecureChannelCredentials;

class CjLoginPushClient {

public:
  CjLoginPushClient(std::shared_ptr<Channel> channel)
    : stub_(CjLoginPushService::NewStub(channel)) {}

  Status connect(const ConnectRequest &req, ConnectResponse *resp) {
    ClientContext context;
    return this->stub_->connect(&context, req, resp);
  }

  Status logoutUser(const LogoutUserRequest &req, LogoutUserResponse *resp) {
    ClientContext context;
    return this->stub_->logoutUser(&context, req, resp);
  }

  Status userLogout(const LogoutUserRequest &req, LogoutUserResponse *resp) {
    ClientContext context;
    return this->stub_->userLogout(&context, req, resp);
  }

private:
  std::unique_ptr<CjLoginPushService::Stub> stub_;
};
