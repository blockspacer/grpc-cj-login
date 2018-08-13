package(default_visibility = ["//visibility:public"])

cc_library(
    name = "cj_login_helper",
    srcs = [
        "helper.cc"
    ],
    hdrs = [
        "helper.h",
        "semaphore.hpp"
    ],
    deps = [
        "@com_github_md5//:md5-lib",
        "@com_github_cppjwt//:jwt-lib",
        "@com_google_glog//:glog"
    ]
)

cc_library(
    name = "cj_login_redis",
    srcs = [
        "redis/redis.cc",
        "redis/redis_table.cc"
    ],
    hdrs = [
        "redis/redis.h",
        "redis/redis_table.h"
    ],
    deps = [
        "@com_redis_hiredis//:hiredis-lib"
    ]
)

cc_binary(
    name = "cj_login_server",
    srcs = ["cj_login_server.cc"],
    deps = [
        "//proto:cj_login_proto",
        ":cj_login_helper",
        ":cj_login_redis",
        "@com_google_glog//:glog"
    ]
)

cc_library(
    name = "cj_login_client",
    srcs = ["cj_login_client.cc"],
    hdrs = ["cj_login_client.h"],
    deps = [
        "//proto:cj_login_proto"
    ]
)

cc_binary(
    name = "cj_login_tools",
    srcs = ["cj_login_tools.cc"],
    deps = [
        ":cj_login_client"
    ]
)

cc_binary(
    name = "cj_login_cgi",
    srcs = ["cj_login.cc"],
    deps = [
        ":cj_login_client",
        "@com_google_glog//:glog",
        ":cj_login_helper"
    ]
)
