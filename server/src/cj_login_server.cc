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

#include "redis/redis.h"
#include "redis/redis_table.h"
#include "helper.h"
#include "cj_login_push_client.hpp"

using cjLogin::User;
using cjLogin::ErrCode;

using cjLogin::BaseRequest;
using cjLogin::BaseResponse;
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

using cjLogin::CjLoginService;

using grpc::InsecureServerCredentials;
using grpc::InsecureChannelCredentials;
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
  CjLoginServiceImpl(string redisIp, int redisPort) {
    this->redis = new Redis(redisIp.c_str(), redisPort);
    this->commTable = new RedisTable(this->redis, commTableName);
    this->userName2UinTable = new RedisTable(this->redis, userName2UinTableName);
    this->userTable = new RedisTable(this->redis, userTableName);
    this->pushClient = new CjLoginPushClient(CreateChannel("localhost:50053",
                                                           InsecureChannelCredentials()));
  }

  ~CjLoginServiceImpl() {
    delete this->commTable;
    delete this->userName2UinTable;
    delete this->userTable;
    delete this->redis;
  }

  Status registerUser(ServerContext *context,
                      const RegisterUserRequest *request,
                      RegisterUserResponse *response) override {
    auto userName = request->username();
    auto password = request->password();
    auto baseResponse = response->mutable_baseresp();

    LOG(INFO) << "register: {" << userName << ", " << password << "}";

    if (!cjLogin::validateUsername(userName)) {
      return this->_finishRequest(baseResponse,
                                  ErrCode::REGISTER_ERROR_USERNAME_INVALID,
                                  "register invalid username");
    }

    if (!cjLogin::validatePassword(password)) {
      return this->_finishRequest(baseResponse,
                                  ErrCode::REGISTER_ERROR_PWD_INVALID,
                                  "register invalid password");
    }

    bool userExist = this->userName2UinTable->existsKey(userName);
    if (userExist) {
      return this->_finishRequest(baseResponse,
                                  ErrCode::REGISTER_ERROR_USERNAME_EXISTS,
                                  "register exists username");
    }

    User newUser;
    auto pwdServer = cjLogin::md5(password + salt);
    unsigned uin = 0;
    if (!this->commTable->increaseKey(curSystemGenUinKey, uin)) {
      LOG(ERROR) << "increase uin fail" ;
      return this->_replySystemError(baseResponse);
    }

    newUser.set_uin(uin);
    newUser.set_username(userName);
    newUser.set_password(pwdServer);

    string serialized;

    if (newUser.SerializeToString(&serialized)
        && this->userTable->setData(std::to_string(uin), serialized)
        && this->userName2UinTable->setData(userName, std::to_string(uin))) {
      return this->_replyOk(baseResponse);
    } else {
      return this->_replySystemError(baseResponse);
    }

  }

  Status userLogin(ServerContext *context,
                   const UserLoginRequest *request,
                   UserLoginResponse *response) override {
    auto userName = request->username();
    auto password = request->password();
    auto baseResponse = response->mutable_baseresp();

    LOG(INFO) << "login: {" << userName << ", " << password << "}";

    string uin;
    if (!this->userName2UinTable->getData(userName, uin)) {
      LOG(ERROR) << "login user not exists: " << userName ;
      return this->_finishRequest(baseResponse,
                                  ErrCode::LOGIN_ERROR_USER_NOT_EXISTS,
                                  "login user not exists");
    }

    string serialized;
    if (!this->userTable->getData(uin, serialized)) {
      LOG(ERROR) << "login user can not find userInfo: " << uin ;
      return this->_replySystemError(baseResponse);
    }

    User user;
    if (!user.ParseFromString(serialized)) {
      LOG(ERROR) << "parse User error: " << uin ;
      return this->_replySystemError(baseResponse);
    }

    auto pwdServer = cjLogin::md5(password + salt);
    if (user.password() != pwdServer) {
      return this->_finishRequest(baseResponse,
                                  ErrCode::LOGIN_ERROR_PWD_ERROR,
                                  "password error");
    }

    auto loginTicket = cjLogin::genLoginTicket(userName, uin);
    user.set_loginticket(loginTicket);
    user.set_logintickettimeout(60 * 60 * 24 * 30);
    if (user.SerializeToString(&serialized)
        && this->userTable->setData(uin, serialized)) {

      LogoutUserRequest req;
      req.set_uin(atoll(uin.c_str()));
      req.set_logoutbyothers(true);
      LogoutUserResponse resp;
      this->pushClient->logoutUser(req, &resp);

      response->set_loginticket(loginTicket);
      return this->_replyOk(baseResponse);
    } else {
      return this->_replySystemError(baseResponse);
    }
  }

  Status userCheckLogin(ServerContext *context,
                        const UserCheckLoginRequest *request,
                        UserCheckLoginResponse *response) override {
    auto userName = request->username();
    auto loginTicket = request->loginticket();
    auto baseResponse = response->mutable_baseresp();

    LOG(INFO) << "checkLogin: {" << userName << ", " << loginTicket << "}";

    PayloadInfo payload;
    if (!cjLogin::extraLoginTicket(loginTicket, payload)
        || payload.userName != userName) {
      return this->_finishRequest(baseResponse,
                                  ErrCode::SYSTEM_INVALID_PARAM,
                                  "invalid params");
    }

    string serialized;
    User user;
    if (!this->userTable->getData(payload.uin, serialized)
        || !user.ParseFromString(serialized)) {
      return this->_replySystemError(baseResponse);
    }

    if (user.loginticket().size() == 0) {
      return this->_finishRequest(baseResponse,
                                  ErrCode::CHECK_LOGIN_USER_LOGIN_OTHERS,
                                  "user logout, ticket timeout");
    }

    if (user.loginticket() != loginTicket) {
      return this->_finishRequest(baseResponse,
                                  ErrCode::CHECK_LOGIN_USER_LOGIN_OTHERS,
                                  "user has logined in other devices");
    }

    long timeout = (long)std::time(0) - payload.ts;
    if (user.logintickettimeout() < timeout) {
      return this->_finishRequest(baseResponse,
                                  ErrCode::CHECK_LOGIN_TICKET_TIMEOUT,
                                  "loginTicket timeout");
    }

    auto sessionKey = cjLogin::genLoginTicket(payload.userName, payload.uin);
    response->set_sessionkey(sessionKey);
    return this->_replyOk(baseResponse);
  }

  Status userLogout(ServerContext *context,
                    const UserLogoutRequest *request,
                    UserLogoutResponse *response) override {
    auto userName = request->basereq().username();
    auto sessionKey = request->basereq().sessionkey();
    auto baseResponse = response->mutable_baseresp();

    LOG(INFO) << "userLogout: {" << userName << ", " << sessionKey << "}";

    PayloadInfo payload;
    if (!cjLogin::extraLoginTicket(sessionKey, payload)
        || payload.userName != userName) {
      return this->_finishRequest(baseResponse,
                                  ErrCode::SYSTEM_INVALID_PARAM,
                                  "invalid params");
    }
    string serialized;
    User user;
    if (!this->userTable->getData(payload.uin, serialized)
        || !user.ParseFromString(serialized)) {
      return this->_replySystemError(baseResponse);
    }

    long timeout = (long)std::time(0) - payload.ts;
    if (10 * 60 < timeout) {
      return this->_finishRequest(baseResponse,
                                  ErrCode::SYSTEM_SESSION_TIMEOUT,
                                  "session timeout");
    }

    user.set_loginticket("");
    user.set_logintickettimeout(0);
    if (user.SerializeToString(&serialized)
        && this->userTable->setData(payload.uin, serialized)) {

      // clear stream
      LogoutUserRequest req;
      req.set_uin(atoll(payload.uin.c_str()));
      LogoutUserResponse resp;
      this->pushClient->userLogout(req, &resp);

      return this->_replyOk(baseResponse);
    } else {
      return this->_replySystemError(baseResponse);
    }
  }


private:
  // redis-cli -h 172.17.0.4 -p 6379
  Redis *redis;
  RedisTable *commTable;
  RedisTable *userName2UinTable;
  RedisTable *userTable;
  CjLoginPushClient *pushClient;

  Status _finishRequest(BaseResponse *baseResp,
                        ErrCode errcode,
                        const string &errmsg) {
    baseResp->set_errcode(errcode);
    baseResp->set_errmsg(errmsg);
    LOG(INFO) << "returns: {" << errcode << ", " + errmsg + "}";
    return Status::OK;
  }

  Status _replySystemError(BaseResponse *baseResp) {
    return this->_finishRequest(baseResp, ErrCode::SYSTEM_ERROR, "system error");
  }

  Status _replyOk(BaseResponse *baseResp) {
    return this->_finishRequest(baseResp, ErrCode::OK, "ok");
  }
};

void RunServer(string redisIp, int redisPort) {
  std::string address("0.0.0.0:50051");
  CjLoginServiceImpl service(redisIp, redisPort);
  ServerBuilder builder;
  builder.AddListeningPort(address, InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  LOG(INFO) << "Server listening on " << address;
  server->Wait();
}

int main(int argc, char *argv[]) {

  google::InitGoogleLogging(argv[0]);

  if (argc != 5) {
    std::cout << "Usage: -h <redisIp> -p <redisPort>" << std::endl;
    return -1;
  }

  string redisIp;
  int redisPort;
  for (int i = 1; i < argc; i += 2) {
    string action = string(argv[i]);
    if (action == "-h") {
      redisIp = string(argv[i + 1]);
    } else if (action == "-p") {
      redisPort  = atoi(argv[i + 1]);
    }
  }

  RunServer(redisIp, redisPort);
  return 0;
}
