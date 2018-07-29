#include "proto/cj_login.grpc.pb.h"
#include <grpc++/channel.h>
#include <grpc++/client_context.h>
#include <grpc++/create_channel.h>
#include <grpc++/security/credentials.h>
#include <iostream>
#include <memory>
#include <string>

using cjLogin::RegisterUserRequest;
using cjLogin::RegisterUserResponse;
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

  std::string registerUser(const std::string &username, const std::string &password) {
    RegisterUserRequest req;
    req.set_username(username);
    req.set_password(password);

    RegisterUserResponse res;
    ClientContext context;
    Status status = stub_->registerUser(&context, req, &res);
    if (status.ok()) {
      return res.sessionkey();
    } else {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return "RPC failed";
    }
  }

private:
  std::unique_ptr<CjLoginService::Stub> stub_;
};

int main(int argc, char *argv[]) {
  CjLoginClient client(CreateChannel("localhost:50051", InsecureChannelCredentials()));
  auto reply = client.registerUser("jeason", "zhu");
  std::cout << "Received: " << reply << std::endl;

  return 0;
}
