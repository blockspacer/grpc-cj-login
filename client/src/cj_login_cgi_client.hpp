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
using grpc::CompletionQueue;

using std::string;
using google::protobuf::Message;

typedef std::function<void(void *)> Completion;

template <class T>
struct AsyncClientCall {
  ActionCode action;

  T reply;

  Completion handler; // send a pointer to this

  ClientContext context;

  Status status;

  std::unique_ptr<ClientAsyncResponseReader<T>> responseReader;
};

class CjLoginCGIClient {

public:
  CjLoginCGIClient(std::shared_ptr<Channel> channel)
    : stub_(CjLoginCGI::NewStub(channel)) {}

  void readMessage(const ConnectRequest &req) {
    ClientContext context;
    std::unique_ptr<ClientReader<ServerMessage>>
      reader(this->stub_->connect(&context, req));
    ServerMessage message;
    while (true) {
      std::this_thread::sleep_for(std::chrono::seconds(1));
      while (reader->Read(&message)) {
        std::cout << "read message" << std::endl;
        std::cout << message.type() << ", " << message.content() << std::endl;
      }
    }

    Status status = reader->Finish();
    if (status.ok()) {
      std::cout << "ok" << std::endl;
    } else {
      std::cout << "fail" << std::endl;
    }
  }

  Status connect(const ConnectRequest &req) {
    std::cout << "hello" << std::endl;
    std::thread t(&CjLoginCGIClient::readMessage, this, std::ref(req));
    t.join();
    return Status::OK;
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
      AsyncClientCall<RegisterUserResponse> *call = static_cast<AsyncClientCall<RegisterUserResponse>*>(goTag);

      call->handler(goTag);
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

/*
int main(int argc, char** argv) {
  CjLoginCGIClient client(grpc::CreateChannel("35.194.225.201:50052",
                                              grpc::InsecureChannelCredentials()));

    if (argc != 5) {
      std::cout << "params error: (" << argc << ")" << std::endl;
      return 0;
    }

    int i = 1;
    string v(argv[i]);
    if (v == "-f") {
      string func(argv[i + 1]);

      Status status;
      string extra;
      std::thread thread_ = std::thread(&CjLoginCGIClient::AsyncCompleteRpc, &client);

      if (func == "connect") {
        ConnectRequest request;
        string userName(argv[i + 2]);
        string sessionKey(argv[i + 3]);
        auto baseReq = request.mutable_basereq();
        baseReq->set_username(userName);
        baseReq->set_sessionkey(sessionKey);
        status = client.connect(request);
        while(true) {
          
        }
      } else if (func == "register") {
        string userName(argv[i + 2]);
        string password(argv[i + 3]);
        RegisterUserRequest request;
        request.set_username(userName);
        request.set_password(password);
        client.registerUser(request, [](void *goTag) {
            AsyncClientCall<RegisterUserResponse> *call = static_cast<AsyncClientCall<RegisterUserResponse>*>(goTag);
            std::cout << "response: " << call->reply.baseresp().errcode()
                      << ", " << call->reply.baseresp().errmsg() << std::endl;
          });
        std::cout << "continue after call registerUser" << std::endl;
      }

      thread_.join();
    }


    return 0;
}
*/
