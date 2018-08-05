#include "proto/cj_login.grpc.pb.h"
#include <grpc++/security/server_credentials.h>
#include <grpc++/server.h>
#include <grpc++/server_builder.h>
#include <grpc++/server_context.h>
#include <grpc/grpc.h>

#include <gflags/gflags.h>
#include <glog/logging.h>
#include <glog/stl_logging.h>

#include <string>
#include <map>
#include "helper.h"

using grpc::InsecureServerCredentials;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using grpc::ServerReader;
using grpc::ServerReaderWriter;
using grpc::ServerWriter;

using cjLogin::CjLoginPushService;
using cjLogin::LogoutUserRequest;
using cjLogin::LogoutUserResponse;
using cjLogin::ConnectRequest;
using cjLogin::ServerMessage;
using cjLogin::MessageType;

class CjLoginPushServiceImpl final : public CjLoginPushService::Service {
 public:
  Status connect(ServerContext *context,
                 const ConnectRequest *request,
                 ServerWriter<ServerMessage> *writer) override {
    auto userName = request->basereq().username();
    auto sessionKey = request->basereq().sessionkey();

    PayloadInfo payload;
    if (!cjLogin::extraLoginTicket(sessionKey, payload)
        || payload.userName != userName) {
      LOG(ERROR) << "connect request invalid" << userName;
      return Status::OK;
    }

    this->clientMap[payload.uin] = writer;
    return Status::OK;
  }

  Status logoutUser(ServerContext *context,
                    const LogoutUserRequest *request,
                    LogoutUserResponse *response) override {
    auto uin = std::to_string(request->uin());
    auto writer = this->clientMap[uin];
    if (writer) {
      ServerMessage message;
      if (request->logoutbyothers()) {
        message.set_type(MessageType::LOGOUT_BY_OTHERS);
        message.set_content("logout by others");
      } else {
        message.set_type(MessageType::LOGOUT_BY_SYSTEM);
        message.set_content("logout by system");
      }

      this->clientMap.erase(uin);
    }
    return Status::OK;
  }

  Status userLogout(ServerContext *context,
                    const LogoutUserRequest *request,
                    LogoutUserResponse *response) override {
    auto uin = std::to_string(request->uin());
    auto writer = this->clientMap[uin];
    if (writer) {
      this->clientMap.erase(uin);
    }
    return Status::OK;
  }

 private:
  std::map<std::string, ServerWriter<ServerMessage> *> clientMap;
};

void RunServer() {
  std::string address("0.0.0.0:50053");
  CjLoginPushServiceImpl service;
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
