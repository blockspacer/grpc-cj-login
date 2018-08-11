package(default_visibility = ["//visibility:public"])

cc_binary(
    name = "cj_login_cgi_tools",
    srcs = ["src/cj_login_cgi_client.cc"],
    deps = [
        "//proto:cj_login_proto"
    ]
)

cc_library(
    name = "cj_login_sdk",
    hdrs = [
        "src/cj_login_imp.hpp",
        "generated-src/cpp/cj_login.hpp"
    ],
    srcs = [
        "src/cj_login_imp.cc"
    ]
)

cc_binary(
    name = "cj_login_tools",
    srcs = [
        "src/cj_login_tools.cc"
    ],
    deps = [
        ":cj_login_sdk"
    ]
)
