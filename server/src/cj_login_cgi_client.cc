#include "proto/cj_login.grpc.pb.h"
#include <grpc++/channel.h>
#include <grpc++/client_context.h>
#include <grpc++/create_channel.h>
#include <grpc++/security/credentials.h>
#include <memory>
#include <thread>
#include <iostream>
#include <utility>

using cjLogin::BaseRequest;
using cjLogin::CjLoginCGI;
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
using grpc::ClientReader;

using std::string;

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

private:
  std::unique_ptr<CjLoginCGI::Stub> stub_;
};

int main(int argc, char** argv) {
  CjLoginCGIClient client(grpc::CreateChannel("localhost:50052",
                                              grpc::InsecureChannelCredentials()));

    if (argc != 5) {
      std::cout << "params error: (" << argc << ")" << std::endl;
      return 0;
    }
  
    int i = 1;
    string v(argv[i]);
    if (v == "-f") {
      std::cout << "hello" << std::endl;
      string func(argv[i + 1]);

      Status status;
      string extra;

      if (func == "connect") {
        ConnectRequest request;
        string userName(argv[i + 2]);
        string sessionKey(argv[i + 3]);
        auto baseReq = request.mutable_basereq();
        baseReq->set_username(userName);
        baseReq->set_sessionkey(sessionKey);
        std::cout << "hello" << std::endl;
        status = client.connect(request);
        while(true) {
          
        }
        std::cout << "hello" << std::endl;

      }
    }

    return 0;
}
