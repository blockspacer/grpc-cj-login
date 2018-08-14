#include "src/cj_login_imp.hpp"

using std::shared_ptr;
using std::string;
using std::string;

const char *serverCA = R"(-----BEGIN CERTIFICATE-----
MIIFhTCCA22gAwIBAgIJALEJWlSZYZPqMA0GCSqGSIb3DQEBCwUAMFkxCzAJBgNV
BAYTAkFVMRMwEQYDVQQIDApTb21lLVN0YXRlMSEwHwYDVQQKDBhJbnRlcm5ldCBX
aWRnaXRzIFB0eSBMdGQxEjAQBgNVBAMMCWxvY2FsaG9zdDAeFw0xNzA4MTAxNTQy
NTZaFw0xODA4MTAxNTQyNTZaMFkxCzAJBgNVBAYTAkFVMRMwEQYDVQQIDApTb21l
LVN0YXRlMSEwHwYDVQQKDBhJbnRlcm5ldCBXaWRnaXRzIFB0eSBMdGQxEjAQBgNV
BAMMCWxvY2FsaG9zdDCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAMql
UOUNLRcq4RqAVUkhk0Dhip7dIEGqkDHYKnj7evGJ+Xnx9Lbevtp9v7coQKdvmgf5
93omTeFivj537zeiqJAQeczTsNvRSZl4bt1WJ/sSYJzPHeg4GaShS78EAa0j3XX8
UffjYYVKbciVZjqaWCUgOk0g8gGwgvPnnV//FNsGclSQm71ybydB7Mruq/cq9vDz
gEip80g8K7nCczR695hE934Tb2TGmLjY02VkrNQA3BrSavM50oUe53IEZg663QxZ
WuoXAlml9y7n9l9tYB2hbfq/rm/CFstt45MIhNKVJgn0ky35czSE/N75LFYeopbG
75MH1JAtJCkSjhOWOTg1ktTgJ+D4BGEVITtgfUof51RjAfCixP38/uSrGCXgTAt9
5sQ0mk7IqQDeJwsoYMxk+JHePmR5pZc0jYanaAaJw4CEMd6h2aA9kYjExo7Mqg3q
pRosfY/vA+XRQk5sf/P9YOcgvtZh4lVZZuM5j2gXWNck9wFQONpyqHfIswppWWCb
RLAj3sk/wmjq5nvzREvJ5632zmGMvUIZsdzDUXRxwqSTg+eU1NjLXkOjt4qPUf57
06AnrYmtluFatEqTq+VjXHIaLDGlSVLl3IavZkbg4a0K+TX7sMg8vrDA02J505tX
EM/0KkBG8xKiuv0zGjbcf6PXJeH7FpQEpWkOH0GnAgMBAAGjUDBOMB0GA1UdDgQW
BBTQF+5xZmQT8ZFeFXBz7bbxwkwUwDAfBgNVHSMEGDAWgBTQF+5xZmQT8ZFeFXBz
7bbxwkwUwDAMBgNVHRMEBTADAQH/MA0GCSqGSIb3DQEBCwUAA4ICAQBWiRL6j566
M+gCV+H8LAXIl6CMvhSOV2+yYnrqN7yGXMpEzy/vP2lQLz5/5fSebtjjTOjGtg7h
2Qc6fyb3KLPX7S4Umx3qy692KaWJxJE8BXP8KSfGLvyvh1KSerGUmPNEKHHzYAfQ
VCF13vcw5zvfd6/bEvA5/lYvMX4FXBq/qAeCZww7jt2lIwDjaeiG+NiGTlkwZkUs
1KrGiAkJ3nSaDEkMWg81+wZYlH3K41Uydy8esBTq+yK+guhlLHVEsh/nkRr8azT6
SovIqyNSVuzIBQyo/o6P/1V+nyQ5s824LOXhS0uA2rDQq4srDv8ZKZijZzQvNKdC
JO4pud/uqmSNA6P5RlB6lSCqX/3cDD6w5b8TQHY5S9yL7jf7PS3iWepA35uwVCUn
kpypMivdhMjSN463FKvxurK4IfGqjq8m8PLoInuFBAkvFhH+CoErtfljxTrJuyNK
TSuPX2WVJHe9syNorUUPxkV+N6pjqfP+aToHAs4a0SKho1EAIk8VlzhnmqeWUpbv
peq2Pscb5+fefa9j3z7dzX4Hu7n8Qo6OX7qybcHNW2D8X+JlWerIoC8LwxiDP8jH
bE3bZdPz6XEd6i5snX8p4haIo16Chg4ebWXREZlUrHfvD+EyNcIisIETHETH/nk1
9HXKO5/6Jw5jFrtdO7tHmLyDIpv0SswFLg==
-----END CERTIFICATE-----)";

const char *clientKey = R"(-----BEGIN PRIVATE KEY-----
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
CUL0uhD/IuDDTO0u81K49NhjTL0VTw==
-----END PRIVATE KEY-----)";

const char *clientCert = R"(-----BEGIN CERTIFICATE-----
MIIFfzCCA2egAwIBAgIJAMaN3ph0TzawMA0GCSqGSIb3DQEBCwUAMFYxCzAJBgNV
BAYTAkFVMRMwEQYDVQQIDApTb21lLVN0YXRlMSEwHwYDVQQKDBhJbnRlcm5ldCBX
aWRnaXRzIFB0eSBMdGQxDzANBgNVBAMMBmNsaWVudDAeFw0xNzA4MTAxNjQ1MjFa
Fw0xODA4MTAxNjQ1MjFaMFYxCzAJBgNVBAYTAkFVMRMwEQYDVQQIDApTb21lLVN0
YXRlMSEwHwYDVQQKDBhJbnRlcm5ldCBXaWRnaXRzIFB0eSBMdGQxDzANBgNVBAMM
BmNsaWVudDCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAN0F+jyrzVMA
34ICzCf5XkCc9zy5998y9MwNgUyEbFuju9HeBdp3GhxpNHdMt/LMvssJLgfKnhdt
/s4ipPicytYoVJCxAvUW4Bdzz6OWua3efGk6tcDWBXBg+Cjt7xcnfomr/vG4fW6A
LWYY/yRkD/EyU+ANJLGQOfpdOdg86RrAXSTnOd+wx5uNk1zKNJWnRuI9lTak+KZs
uKtyzQCQ5cxgHcfxgizqEVg0+KPz0gk9jtcgUoDY3URaV5bUBrRUtpxhcalpsTji
oIMefWLr/ec8smhdHF7VJwVKXDzR5FdVRotFgjdBcHpzQI24GscUnqPzejmWWZgj
PNynj1TjGSyXloucQIsoCR3FSdmpuRLzLfzHoxP+zI8wCIa7bvD6OOCLCVe8KHwv
+24u8369ubKS4s5nSycedAP53fMaZkU/u42zwL09rrQwOIdXGibUvlWmdSInv9Md
hLJg6VUAKUV2/lzQsp2MYFs9wqtwZ8yd5ySS3uPrAXGfp1hw5eDIRokxoa93pEbk
FipDIkhCz5jwJ5QYzyJgIO42mdaAZjssEXHvjAISWG/LCZH/FL5kjZ4hRxJ57lfA
UV4dLpLluWiWdwD8ui+Th8ECwr3qBsrmHFkLWZ128+3Q99clcWrFIGNpmcKSSjaM
Rr0yWGP/MHR6uSEVw/nmaMsvmRp5MIz7AgMBAAGjUDBOMB0GA1UdDgQWBBRnBN7A
awejoO/zyKP/vxbMerKYuTAfBgNVHSMEGDAWgBRnBN7AawejoO/zyKP/vxbMerKY
uTAMBgNVHRMEBTADAQH/MA0GCSqGSIb3DQEBCwUAA4ICAQC6ue8rQeimVt68ToxI
K5aft/syK4F09bD8893lUvvRQGGf2IemD+zjQrlO2muKlCMka8SlTB6lu1QFoztH
55v2rzueEOQWHFvccu+bAaNuu1NKVeyk/CbJeotg3gY9mvuu80KGvqC5X8tSoq68
DGEa6VJMwwVakIt7hZAMEwQNIurYy8OCwRTj8iSxclUG+aV5mcHy/el1G5rR5upv
NfY+rxmAHh/u3zbGJXD9PGmD0h+QR1C5Jb6prFGhweolsy8Yj1NOAIGwETap5NW6
C9w3h/YBi322hQuAN4L2eQxWQw9vNlFKxMqVnYM/U5LyD9cfqEFLyi+o/uw1x3/U
3EHPb/M/M1iUv3nB+fXSLSTOzDRvA85Wkp/18LKhnG2yUskJgCXCIdWO69Z5FpqR
HfE0sd/se5M0C7MBoIweNie7A1Awko+Iz8tf7y7wDKk4Y7dmRdPZzVABfhM/Z9F5
dh670PizalOeKML3Eocu/3s1Z4awLu4XFzhPXjaVYUFwQafMb8anjKO53PuSS4f6
sJxD3/mW9noU1XMSNUpE7mgJ/fqeXEsUEsx8P1S6rXMQAiUtBHZq3phBRUKQlfyM
auUDNNg+l/z0Kg/YP6c5CuNXUfIdEgeptZA8Bb3evRuDU/8jYh+kGsW+d/v2fZzO
26pm4SuloHoX8exUUw2MFIu2uw==
-----END CERTIFICATE-----)";

namespace cjlogin {

  shared_ptr<CjLogin> CjLogin::create() {
    return std::make_shared<CjLoginImpl>();
  }

  CjLoginImpl::CjLoginImpl() {
    grpc::SslCredentialsOptions ssl_opts;
    ssl_opts.pem_root_certs = serverCA;
    ssl_opts.pem_private_key = clientKey;
    ssl_opts.pem_cert_chain = clientCert;
    std::shared_ptr<grpc::ChannelCredentials> creds = grpc::SslCredentials(ssl_opts);
    grpc::ChannelArguments args;
    args.SetSslTargetNameOverride("localhost");
    this->client = new CjLoginCGIClient(grpc::CreateCustomChannel("35.194.225.201:50052",creds, args));
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

