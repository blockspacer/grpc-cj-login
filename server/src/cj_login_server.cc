#include "proto/cj_login.grpc.pb.h"
#include <grpc++/security/server_credentials.h>
#include <grpc++/server.h>
#include <grpc++/server_builder.h>
#include <grpc++/server_context.h>
#include <grpc/grpc.h>

#include <iostream>
#include <memory>
#include <string>

#include "hiredis.h"

using cjLogin::RegisterUserRequest;
using cjLogin::RegisterUserResponse;
using cjLogin::CjLoginService;

using grpc::InsecureServerCredentials;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

class CjLoginServiceImpl final : public CjLoginService::Service {
 public:
  Status registerUser(ServerContext *context, const RegisterUserRequest *request,
                      RegisterUserResponse *response) override {
    auto userName = request->username();
    auto password = request->password();
    std::cout << "Register User: " << userName << " password: " << password << std::endl;
    response->set_sessionkey("hello");
    return Status::OK;
  }
};

void RunServer() {
  std::string address("0.0.0.0:50051");
  CjLoginServiceImpl service;
  ServerBuilder builder;
  builder.AddListeningPort(address, InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << address << std::endl;
  server->Wait();
}

int main(int argc, char *argv[]) {
  RunServer();
  return 0;
}

