#pragma once

#include "proto/cj_login.grpc.pb.h"
#include <grpc++/channel.h>
#include <grpc++/client_context.h>
#include <grpc++/create_channel.h>
#include <grpc++/security/credentials.h>

#include <memory>
#include <thread>
#include <iostream>
#include <utility>
#include <functional>

using cjLogin::BaseRequest;
using cjLogin::CjLoginCGI;
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
using cjLogin::ConnectRequest;
using cjLogin::ServerMessage;
using cjLogin::MessageType;
using cjLogin::ActionCode;

using grpc::Channel;
using grpc::Status;
using grpc::Channel;
using grpc::ClientContext;
using grpc::CreateChannel;
using grpc::InsecureChannelCredentials;
using grpc::ClientReader;
using grpc::ClientAsyncResponseReader;
using grpc::ClientAsyncReader;
using grpc::CompletionQueue;

using std::string;
using google::protobuf::Message;

typedef std::function<void(void *)> Completion;
typedef std::function<void(ServerMessage &)> MessageHandler;

template <class T>
struct AsyncClientCall {
  ActionCode action;

  T reply;

  Completion handler; // send a pointer to this

  ClientContext context;

  Status status;

  std::unique_ptr<ClientAsyncResponseReader<T>> responseReader;
  std::unique_ptr<ClientAsyncReader<T>> streamReader;
};

class CjLoginCGIClient {

public:
  CjLoginCGIClient(std::shared_ptr<Channel> channel)
    : stub_(CjLoginCGI::NewStub(channel)) {}

  void readMessage(const ConnectRequest &req, MessageHandler handler) {
    ClientContext context;
    CompletionQueue cq;
    ServerMessage message;
    Status status;
    void *goTag;
    bool ok = false;

    std::unique_ptr<ClientAsyncReader<ServerMessage>>
      reader(this->stub_->Asyncconnect(&context, req, &cq, (void *)1));

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

  void connect(const ConnectRequest &req, MessageHandler handler) {
    auto t = std::thread(&CjLoginCGIClient::readMessage, this, req, handler);
    t.detach();
  }

  void registerUser(const RegisterUserRequest &request,
                    Completion handler) {
    auto call = new AsyncClientCall<RegisterUserResponse>;
    call->responseReader = this->stub_->PrepareAsyncregisterUser(&call->context,
                                                                 request,
                                                                 &this->cq_);
    this->doRequest(call, handler, ActionCode::Register);

  }

  void login(const UserLoginRequest &request,
             Completion handler) {
    auto call = new AsyncClientCall<UserLoginResponse>;
    call->responseReader = this->stub_->PrepareAsynclogin(&call->context,
                                                          request,
                                                          &this->cq_);
    this->doRequest(call, handler, ActionCode::Login);
  }

  void checkLogin(const UserCheckLoginRequest &request,
                  Completion handler) {
    auto call = new AsyncClientCall<UserCheckLoginResponse>;
    call->responseReader = this->stub_->PrepareAsynccheckLogin(&call->context,
                                                               request,
                                                               &this->cq_);
    this->doRequest(call, handler, ActionCode::CheckLogin);
  }

  void logout(const UserLogoutRequest &request,
              Completion handler) {
    auto call = new AsyncClientCall<UserLogoutResponse>;
    call->responseReader = this->stub_->PrepareAsynclogout(&call->context,
                                                           request,
                                                           &this->cq_);
    this->doRequest(call, handler, ActionCode::Logout);
  }

  void AsyncCompleteRpc() {
    void *goTag;
    bool ok = false;
    while (cq_.Next(&goTag, &ok)) {
      if (ok) {
        auto call = static_cast<AsyncClientCall<RegisterUserResponse>*>(goTag);
        call->handler(goTag);
      }
    }
  }

private:

  void doRequest(void *goTag,
                 Completion handler,
                 ActionCode action) {
    AsyncClientCall<RegisterUserResponse> *call = static_cast<AsyncClientCall<RegisterUserResponse>*>(goTag);
    call->action = action;
    call->handler = handler;
    call->responseReader->StartCall();
    call->responseReader->Finish(&call->reply, &call->status, (void *)call);
  }

  std::unique_ptr<CjLoginCGI::Stub> stub_;
  CompletionQueue cq_;
};
