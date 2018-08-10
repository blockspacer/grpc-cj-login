#include "proto/cj_login.grpc.pb.h"
#include <grpc++/security/server_credentials.h>
#include <grpc++/server.h>
#include <grpc++/server_builder.h>
#include <grpc++/server_context.h>
#include <grpc/grpc.h>

#include <gflags/gflags.h>
#include <glog/logging.h>
#include <glog/stl_logging.h>

#include <iostream>
#include <memory>
#include <string>

#include "cj_login_client.h"
#include "helper.h"

using cjLogin::ErrCode;
using cjLogin::CjLoginCGI;

using grpc::InsecureServerCredentials;
using grpc::InsecureChannelCredentials;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using cjLogin::ConnectRequest;
using cjLogin::ServerMessage;
using cjLogin::MessageType;

class CjLoginCGIImp final : public CjLoginCGI::Service {
 public:

  CjLoginCGIImp() {
    this->server = new CjLoginClient(CreateChannel("localhost:50051",
                                                   InsecureChannelCredentials()));

  }

  ~CjLoginCGIImp() {
    delete this->server;
  }

  Status connect(ServerContext *context,
                 const ConnectRequest *request,
                 ServerWriter<ServerMessage> *writer) override {
    auto userName = request->basereq().username();
    auto sessionKey = request->basereq().sessionkey();

    PayloadInfo payload;
    if (!cjLogin::extraLoginTicket(sessionKey, payload)
        || payload.userName != userName) {
      LOG(ERROR) << "connect request invalid" << userName;
      return Status::CANCELLED;
    }

    this->clientMap[payload.uin] = writer;
    return Status::OK;
  }

  Status registerUser(ServerContext *context,
                      const RegisterUserRequest *request,
                      RegisterUserResponse *response) override {
    return this->server->registerUser(*request, response);
  }

  Status login(ServerContext *context,
               const UserLoginRequest *request,
               UserLoginResponse *response) override {
    return this->server->login(*request, response);
  }

  Status checkLogin(ServerContext *context,
                    const UserCheckLoginRequest *request,
                    UserCheckLoginResponse *response) override {
    return this->server->checkLogin(*request, response);
  }

  Status logout(ServerContext *context,
                const UserLogoutRequest *request,
                UserLogoutResponse *response) override {
    auto userName = request->basereq().username();
    auto sessionKey = request->basereq().sessionkey();
    auto baseResponse = response->mutable_baseresp();

    PayloadInfo payload;
    if (!cjLogin::extraLoginTicket(sessionKey, payload)
        || payload.userName != userName) {
      LOG(ERROR) << "logout User Invalid request invalid" << userName;
      baseResponse->set_errcode(ErrCode::SYSTEM_INVALID_PARAM);
      baseResponse->set_errmsg("invalid params");
      return Status::OK;
    }

    auto writer = this->clientMap[payload.uin];
    if (writer) {
      ServerMessage message;
      this->clientMap.erase(uin);
    }

    return this->server->logout(*request, response);
  }

 private:
  CjLoginClient *server;
  std::map<std::string, ServerWriter<ServerMessage> *> clientMap;
};


void RunServer() {
  std::string address("0.0.0.0:50052");
  CjLoginCGIImp service;
  ServerBuilder builder;
  builder.AddListeningPort(address, InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  LOG(INFO) << "Server listening on " << address;
  server->Wait();
}

int main(int argc, char *argv[]) {
  google::InitGoogleLogging(argv[0]);
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  RunServer();
  return 0;
}
