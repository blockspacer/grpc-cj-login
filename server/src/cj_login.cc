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

using cjLogin::CjLoginCGI;

using grpc::InsecureServerCredentials;
using grpc::InsecureChannelCredentials;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

class CjLoginCGIImp final : public CjLoginCGI::Service {
 public:

  CjLoginCGIImp() {
    this->client = new CjLoginClient(CreateChannel("localhost:50051",
                                                   InsecureChannelCredentials()));

  }

  ~CjLoginCGIImp() {
    delete this->client;
  }

  Status registerUser(ServerContext *context,
                      const RegisterUserRequest *request,
                      RegisterUserResponse *response) override {
    return client->registerUser(*request, response);
  }

  Status login(ServerContext *context,
               const UserLoginRequest *request,
               UserLoginResponse *response) override {
    return client->login(*request, response);
  }

  Status checkLogin(ServerContext *context,
                    const UserCheckLoginRequest *request,
                    UserCheckLoginResponse *response) override {
    return client->checkLogin(*request, response);
  }

  Status logout(ServerContext *context,
                const UserLogoutRequest *request,
                UserLogoutResponse *response) override {
    return client->logout(*request, response);
  }

 private:
  CjLoginClient *client;
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
