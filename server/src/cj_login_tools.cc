#include <grpc++/channel.h>
#include <grpc++/client_context.h>
#include <grpc++/create_channel.h>
#include <grpc++/security/credentials.h>
#include <iostream>
#include <string>

#include "cj_login_client.h"

using grpc::InsecureChannelCredentials;
using grpc::CreateChannel;

using std::string;

int main(int argc, char *argv[]) {
  CjLoginClient client(CreateChannel("localhost:50051",
                                     InsecureChannelCredentials()));

  if (argc != 5) {
    std::cout << "params error: (" << argc << ")" << std::endl;
    return 0;
  }

  int i = 1;
  string v(argv[i]);
  if (v == "-f") {
    string func(argv[i + 1]);

    Status status;
    if (func == "register") {
      string userName(argv[i + 2]);
      string password(argv[i + 3]);

      RegisterUserRequest req;
      RegisterUserResponse resp;
      req.set_username(userName);
      req.set_password(password);

      status = client.registerUser(req, &resp);
    } else if (func == "login") {
      string userName(argv[i + 2]);
      string password(argv[i + 3]);

      UserLoginRequest req;
      UserLoginResponse resp;
      req.set_username(userName);
      req.set_password(password);

      status = client.login(req, &resp);
      std::cout << resp.loginticket() << std::endl;
    } else if (func == "checkLogin") {
      string userName(argv[i + 2]);
      string loginTicket(argv[i + 3]);

      UserCheckLoginRequest req;
      UserCheckLoginResponse resp;
      req.set_username(userName);
      req.set_loginticket(loginTicket);

      status = client.checkLogin(req, &resp);
      std::cout << resp.sessionkey() << std::endl;
    } else if (func == "logout") {
      string userName(argv[i + 2]);
      string sessionKey(argv[i + 3]);

      UserLogoutRequest req;
      auto baseReq = req.mutable_basereq();
      UserLogoutResponse resp;
      baseReq->set_username(userName);
      baseReq->set_sessionkey(sessionKey);

      status = client.logout(req, &resp);
    }
  }


  return 0;
}
