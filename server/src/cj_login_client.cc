#include "cj_login_client.h"
#include <thread>

using grpc::Channel;
using grpc::ClientContext;
using grpc::CreateChannel;
using grpc::InsecureChannelCredentials;
using grpc::Status;
using grpc::ClientAsyncReader;
using grpc::CompletionQueue;

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

void CjLoginClient::internalConnect(const InternalConnectRequest &req,
                                    InternalMessageHandler handler) {
  auto t = std::thread(&CjLoginClient::readMessage, this, req, handler);
  t.detach();
}

void CjLoginClient::readMessage(const InternalConnectRequest &req,
                                InternalMessageHandler handler) {
  ClientContext context;
  CompletionQueue cq;
  InternalMessage message;
  Status status;
  void *goTag;
  bool ok = false;
  std::unique_ptr<ClientAsyncReader<InternalMessage>>
    reader(this->stub_->AsyncinternalConnect(&context, req, &cq, (void *)1));

  while (cq.Next(&goTag, &ok)) {
    if (ok && goTag == (void *)1) {
      reader->Read(&message, (void *)1);
      if (message.type() != 0) {
        std::cout << "receive Message: " << message.type()
                  << ", " << message.content() << std::endl;
        handler(message);
      }
    }
  }

  reader->Finish(&status, (void *)1);
  if (status.ok()) {
    std::cout << "Finish Read Message" << std::endl;
  } else {
    std::cout << "Fail" << std::endl;
  }
}
