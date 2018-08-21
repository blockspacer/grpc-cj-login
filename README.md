# grpc-cj-login
A login demo using grpc.


## Build Server

~~~
cd server
bazel build --cxxopt='-std=c++14' //src:all
~~~

## Run Server
Suppose a redis server is running on host <code>172.17.0.4</code> with port <code>6379</code>, you can run cj_login_server like this:

~~~
./bazel-bin/src/cj_login_server -h 172.17.0.4 -p 6379 &
./bazel-bin/src/cj_login_cgi cert/client-self-signed-cert.pem cert/server.key.pem cert/server-self-signed.cert.pem
~~~

glog files is located on /tmp/ as default.

## Build&Run iOS Client

open client/CJLoginDemo.tulsiproj using [tulsi](https://github.com/bazelbuild/tulsi), generate XCode Project, then build&run in XCode.

## Run Server Tools

~~~
# register:
./bazel-bin/src/cj_login_tools -f register jeason_test test1234

# login
./bazel-bin/src/cj_login_tools -f login jeason_test test1234 # console will output login ticket

# checkLogin
./bazel-bin/src/cj_login_tools -f checkLogin jeason_test <loginTicket> # console will output sessionkey

# logout
./bazel-bin/src/cj_login_tools -f logout jeason_test <sessionkey> 

# logoutUser
./bazel-bin/src/cj_login_tools -f logoutUser uin <uin>
~~~
