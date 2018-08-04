#include "proto/cj_login.grpc.pb.h"
#include <grpc++/security/server_credentials.h>
#include <grpc++/server.h>
#include <grpc++/server_builder.h>
#include <grpc++/server_context.h>
#include <grpc/grpc.h>

#include <iostream>
#include <memory>
#include <string>

#include "redis/redis.h"
#include "redis/redis_table.h"
#include "helper.h"

using cjLogin::User;
using cjLogin::BaseResponse;
using cjLogin::RegisterUserRequest;
using cjLogin::RegisterUserResponse;
using cjLogin::CjLoginService;
using cjLogin::ErrCode;

using grpc::InsecureServerCredentials;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

const char *salt = "cj_login_server_grpc_salt";

const char *curSystemGenUinKey = "--cj_login_server_grpc_uin--";
const char *userTableName = "UserTable";
const char *userName2UinTableName = "UserName2UinTableName";
const char *commTableName = "CommonTable";

class CjLoginServiceImpl final : public CjLoginService::Service {
 public:
  CjLoginServiceImpl() {
    this->redis = new Redis("172.17.0.4", 6379);
    this->commTable = new RedisTable(this->redis, commTableName);
    this->userName2UinTable = new RedisTable(this->redis, userName2UinTableName);
    this->userTable = new RedisTable(this->redis, userTableName);
  }

  ~CjLoginServiceImpl() {
    delete this->commTable;
    delete this->userName2UinTable;
    delete this->userTable;
    delete this->redis;
  }

  Status registerUser(ServerContext *context, const RegisterUserRequest *request,
                      RegisterUserResponse *response) override {
    auto userName = request->username();
    auto password = request->password();
    auto baseResponse = response->mutable_baseresp();

    if (!cjlogin::validateUsername(userName)) {
      std::cout << "register invalid username: " << userName << std::endl;
      baseResponse->set_errcode(ErrCode::REGISTER_ERROR_USERNAME_INVALID);
      baseResponse->set_errmsg("register invalid username");
      return Status::OK;
    }

    bool userExist = this->userName2UinTable->existsKey(userName);
    if (userExist) {
      std::cout << "register exists username" << std::endl;
      baseResponse->set_errcode(ErrCode::REGISTER_ERROR_USERNAME_EXISTS);
      baseResponse->set_errmsg("register exists username");
      return Status::OK;
    }

    User newUser;
    auto pwdServer = cjlogin::md5(password + salt);
    unsigned uin = 0;
    if (!this->commTable->increaseKey(curSystemGenUinKey, uin)) {
      std::cout << "increase uin fail" << std::endl;
      baseResponse->set_errcode(ErrCode::SYSTEM_ERROR);
      baseResponse->set_errmsg("system error");
      return Status::OK;
    }

    newUser.set_uin(uin);
    newUser.set_username(userName);
    newUser.set_password(pwdServer);

    string serialized;
    newUser.SerializeToString(&serialized);

    this->userTable->setData(std::to_string(uin), serialized);
    this->userName2UinTable->setData(userName, std::to_string(uin));

    baseResponse->set_errcode(ErrCode::OK);
    baseResponse->set_errmsg("ok");
    
    return Status::OK;
  }

private:
  // redis-cli -h 172.17.0.4 -p 6379
  Redis *redis;
  RedisTable *commTable;
  RedisTable *userName2UinTable;
  RedisTable *userTable;
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

