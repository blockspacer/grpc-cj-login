#include "src/cj_login_imp.hpp"

using std::shared_ptr;
using std::string;
using std::string;

const char *serverCA = R"(-----BEGIN CERTIFICATE-----
MIIDfzCCAmegAwIBAgIJAP8uIh5MMKvMMA0GCSqGSIb3DQEBCwUAMFYxCzAJBgNV
BAYTAkFVMRMwEQYDVQQIDApTb21lLVN0YXRlMSEwHwYDVQQKDBhJbnRlcm5ldCBX
aWRnaXRzIFB0eSBMdGQxDzANBgNVBAMMBkplYXNvbjAeFw0xODA4MTQwMTAzMDNa
Fw0xOTA4MTQwMTAzMDNaMFYxCzAJBgNVBAYTAkFVMRMwEQYDVQQIDApTb21lLVN0
YXRlMSEwHwYDVQQKDBhJbnRlcm5ldCBXaWRnaXRzIFB0eSBMdGQxDzANBgNVBAMM
BkplYXNvbjCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAL5SIo+riqh6
rwwv0YlKnX9UfTXScHU0FpsdEinKMtP3rs+Ir6zih2tDRCwux2nOd2tTXwxwNQJv
jJ935pGGQlGIHn10Pol9GPzlcLsgumt8z/JH0Gq28PYPtOEpOPNljmTwiEYBd9u7
MDhnux6JreKUdkZMsts5wzvT9eHw9zeVYwM+NfZmGEJejd3sAaoz012EYAoBxakn
OXS5ej9URJFA5l1iShFSiAqNnrUW5Z2WqFM8JDU6vU03wH3PpZnoscLHgUh66t9U
LrV0W1ZZibKme7teXjadF3RqDpUasPLj39PM+C6TG4J+GMZNdMOmHzZyi+UBBvnX
P4oD90wUoVcCAwEAAaNQME4wHQYDVR0OBBYEFFAzcnFzcnYAIT9fOqqtdGm5iL3e
MB8GA1UdIwQYMBaAFFAzcnFzcnYAIT9fOqqtdGm5iL3eMAwGA1UdEwQFMAMBAf8w
DQYJKoZIhvcNAQELBQADggEBAFfPYimCN/NDQ0B3cKT8MPPcteGmgw2ep1eninZL
JTgilIxNn8an/FwikrhpCsZ5CjX7Y2zWypYr5psM9eRsL0Ok/8CB1X0KLq3rMP2f
juk7aM/irtS3V+bFSStKLFbpnWPT7DP/IjWujQwqDd8Jl17VO0NEZU3RWuxKtezB
r5Bx/Y3pnfhQtGa05mkoY/5rBcuLELwuCKkbJdxLFt085OLHHVRgTEui4VG+Lmx5
fNDL6+3tZIvYVnZP4DDGwBXlmwb52DvB7p40KDqItE2FtWxNdYpHblqscd1zH1xq
iD5nd7sIkEdQvaZfkzZL3lh4qzRotatnyuO8YYQVinXebho=
-----END CERTIFICATE-----)";

const char *clientKey = R"(-----BEGIN PRIVATE KEY-----
MIIEvgIBADANBgkqhkiG9w0BAQEFAASCBKgwggSkAgEAAoIBAQCrKciNrKpYPpN2
fmWukoEHCiL4alTLDIFupR8C8dGrtoJut9pTYZrO4nQiji6374hAeMY/4jwqGevx
9QmQOj99SIZAzM5cGA5clq9TO6lSG5kl93bCxCo03Q8O+o4fjrvJfPpffWnPEwp1
UrNLXt6KyozSVDfLOlYHxxYLALN4Ij5sQb2Fjt8QJMmDov8BGZqh2xm1C0BVCI+l
bPCLPjI9w612fEENOh3iRpQyxyKq2/b9lmbkoRXgvSsj6azvqL7IOuhElFuO9U4N
ZF+LBh8VwU37RBOlFTb2Li/GMW8y6aWrPUY+qAw6QGdz9l1dhyUBPQbk99gBYpKy
6QJL2HKzAgMBAAECggEAEuX2Is5hsISYO4kRKQk7/c78tXjoC9NBpznMBsM0Z0Ur
eZYLIRGxyg3uzLHuLpjf7l5nZT5RbkSWbIyllasYv3h2ekkir8BoNYjdqs73jv1F
F5APKMoH7vZJZ1n6vzk/Oqv16P0r5ZUwnmuXKEc9Qw3CJqbt0h6KySkOIgDdz/Gp
lVgOdDL+LVrrhu+NRbBIhV0zrrUgPurrCadUe0ENXtXMsDHCRbr0Z04Gdvigh20s
kYwU6QIY9UdbGfFhr/aRqQA5NygkNQsoNnvQdZ4Ndw2Y0YTEfBJ+E+u8ZVvLMhmb
MMakX0Xym7wvRNrRjzKX2NhJKZYoqglEt5p8T4C1QQKBgQDkJqbI4Q6/1sbYTBvu
TQE6GUSRlbJ6a2sRrF1pnvenowjFRJrAQg1R6PxFzGp4FBCl/3+1eHPTMk7K+Rrb
Wr4MkA/6TN+7jm/4SWKsppEj+Una3AEw71kkgW1ecLm2CCAskihliFxpeweo5J1Z
f40wEdj87XpLg15U4rB9hfZ8yQKBgQDADltH/FRzjfQzGNn8SBCD/tVTw+ZTJIPP
CmIYl3Q6gtfvMzWd9rfUhrgPG4hpfYtaGD7wEzL+Ndl9IbveaXZ0iMOv3VX5Zbnv
HAcdHofVtlOCb95W3RymiVmkb1uvDXcQ2+HkUt6+kNj5ZK2OMnSK0asCHgGk02xM
gAKax2c9mwKBgDO+0HUfFklMXS0RlgQmUvDqajLa0PATDOWlQkqCU4kh4Qtc0/xm
/r5lKVJrWnqBkoUTvdNazmpILra6aOz3V8NhqT4L0XKoi5y0iDzASoruL2IEJAEF
SCKPeE7DIlu23BU/0Qm2M794t/QGZem2lD8izSQQeHxH/00hXQYbLByBAoGBAJbC
bksqu0xeMURvDKaqFzTWNLVlgQbhEgrTNr/hG97WWRUu2LbgNdvByPgq/8D3SQ13
HOit2YiN0uUUNvpXRSIjPaFumI+GmBcnpDnqfmx3jERG9nyvY08jYFfhBlJ6um1k
fYUSvEFRSNE0g7ysxKk3/wpRba7qPAgtMF45i0RjAoGBAJ96gtWHb2sScN3665GD
E3AdRIlZdLlguNxtzNjJlThHnMlCMf4iONqF481rWjz99VPcqvR5J1DVBB104Y+5
LNpexLrTHf5U/o9csTkT1DQ1B8dJlEG3zPrbLnOtlupUsYT4PxF+6fVnmD6PG4yV
uzX5XRiZngD8tUYYub5sZ542
-----END PRIVATE KEY-----)";

const char *clientCert = R"(-----BEGIN CERTIFICATE-----
MIICnjCCAYYCCQCIGY5GVt0O8TANBgkqhkiG9w0BAQsFADARMQ8wDQYDVQQDDAZj
bGllbnQwHhcNMTgwODE0MDExNzE5WhcNMTkwODE0MDExNzE5WjARMQ8wDQYDVQQD
DAZjbGllbnQwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQCrKciNrKpY
PpN2fmWukoEHCiL4alTLDIFupR8C8dGrtoJut9pTYZrO4nQiji6374hAeMY/4jwq
Gevx9QmQOj99SIZAzM5cGA5clq9TO6lSG5kl93bCxCo03Q8O+o4fjrvJfPpffWnP
Ewp1UrNLXt6KyozSVDfLOlYHxxYLALN4Ij5sQb2Fjt8QJMmDov8BGZqh2xm1C0BV
CI+lbPCLPjI9w612fEENOh3iRpQyxyKq2/b9lmbkoRXgvSsj6azvqL7IOuhElFuO
9U4NZF+LBh8VwU37RBOlFTb2Li/GMW8y6aWrPUY+qAw6QGdz9l1dhyUBPQbk99gB
YpKy6QJL2HKzAgMBAAEwDQYJKoZIhvcNAQELBQADggEBAAuS24kqYiereprx7UnV
E7DYOUTj0N/wVff/DZND3jAd/cPjjltfdcwKOgs2J59TZ/Z55tvOibGqQeF+10y6
VQx6xmlVxOFNpPfWu3QeUnzL4Xe5EnVi2ydvPOyhC+iBP0dyqBpAfx/OY6WiZ1DK
gOUan6GIPvUca3BzUh1CpakhAvBHx29lp5lrlT3+A1Xh1CBTkptp2uGoX/Wf4e5A
gdfrC7bOU35C7ejkn/LZd98ComJ0TdfJzvkGvGzGJYeJbLK7IJjEYugtMGDy1Q5B
iWh+ns9xi4RjOoIh++JiPp18o1t78wpI2e0LRch/dJu1XrUBODPq8O0jilTHoGpy
Fho=
-----END CERTIFICATE-----)";

namespace cjlogin {

  shared_ptr<CjLogin> CjLogin::create(const std::string & server, const std::string & serverCert, const std::string & cert, const std::string & key, const std::string & sslOverride) {
    return std::make_shared<CjLoginImpl>(server, serverCert, cert, key, sslOverride);
  }

  CjLoginImpl::CjLoginImpl(const std::string & server, const std::string & serverCert, const std::string & cert, const std::string & key, const std::string & sslOverride) {
    grpc::SslCredentialsOptions ssl_opts;
    ssl_opts.pem_root_certs = serverCert.c_str();
    ssl_opts.pem_private_key = key.c_str();
    ssl_opts.pem_cert_chain = cert.c_str();
    std::shared_ptr<grpc::ChannelCredentials> creds = grpc::SslCredentials(ssl_opts);
    grpc::ChannelArguments args;
    args.SetSslTargetNameOverride(sslOverride.c_str());
    this->client = new CjLoginCGIClient(grpc::CreateCustomChannel(server.c_str(),creds, args));
    auto t = std::thread(&CjLoginCGIClient::AsyncCompleteRpc, this->client);
    t.detach();
  }

  CjLoginImpl::~CjLoginImpl() {
    delete this->client;
  }

  void CjLoginImpl::registerUser(const string & userName,
                                 const string & password,
                                 const shared_ptr<RegisterUserCallback> & cb) {
    RegisterUserRequest request;
    request.set_username(userName);
    request.set_password(password);

    this->client->registerUser(request, [cb](void *goTag) {
        auto call = static_cast<AsyncClientCall<RegisterUserResponse>*>(goTag);
        auto errcode = call->reply.baseresp().errcode();
        auto errmsg = call->reply.baseresp().errmsg();

        cb->complete((int32_t)errcode, errmsg);
      });
  }

  void CjLoginImpl::login(const std::string & userName,
                          const std::string & password,
                          const std::shared_ptr<LoginCallback> & cb) {
    UserLoginRequest request;
    request.set_username(userName);
    request.set_password(password);
    std::cout << "doing login: " << userName << ", " << password << std::endl;
    this->client->login(request, [cb](void *goTag) {
        auto call = static_cast<AsyncClientCall<UserLoginResponse>*>(goTag);
        auto errcode = call->reply.baseresp().errcode();
        auto errmsg = call->reply.baseresp().errmsg();
        auto loginTicket = call->reply.loginticket();
        cb->complete((int32_t)errcode, errmsg, loginTicket);
      });
  }

  void CjLoginImpl::checkLogin(const std::string & userName,
                               const std::string & loginTicket,
                               const std::shared_ptr<CheckLoginCallback> & cb) {
    UserCheckLoginRequest request;
    request.set_username(userName);
    request.set_loginticket(loginTicket);

    this->client->checkLogin(request, [cb](void *goTag) {
        auto call = static_cast<AsyncClientCall<UserCheckLoginResponse>*>(goTag);
        auto errcode = call->reply.baseresp().errcode();
        auto errmsg = call->reply.baseresp().errmsg();
        auto sessionKey = call->reply.sessionkey();

        cb->complete((int32_t)errcode, errmsg, sessionKey);
      });
  }

  void CjLoginImpl::logout(const std::string & userName,
                           const std::string & sessionKey,
                           const std::shared_ptr<LogoutCallback> & cb) {
    UserLogoutRequest request;
    auto baseRequest = request.mutable_basereq();
    baseRequest->set_username(userName);
    baseRequest->set_sessionkey(sessionKey);

    this->client->logout(request, [cb](void *goTag) {
        auto call = static_cast<AsyncClientCall<UserLogoutResponse>*>(goTag);
        auto errcode = call->reply.baseresp().errcode();
        auto errmsg = call->reply.baseresp().errmsg();

        cb->complete((int32_t)errcode, errmsg);
      });
  }

  void CjLoginImpl::connect(const string & userName,
                            const string & sessionKey,
                            const std::shared_ptr<ConnectCallback> & cb) {
    ConnectRequest request;
    auto baseRequest = request.mutable_basereq();
    baseRequest->set_username(userName);
    baseRequest->set_sessionkey(sessionKey);

    this->client->connect(request, [cb](ServerMessage &message) {
        cb->complete((int32_t)message.type(), message.content());
      });
  }
}

