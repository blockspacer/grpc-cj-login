package(default_visibility = ["//visibility:public"])

cc_binary(
    name = "cj_login_server",
    srcs = ["cj_login_server.cc"],
    deps = [
        "//proto:cj_login_proto",
        "@com_redis_hiredis//:hiredis-lib"
    ]
)

cc_binary(
    name = "cj_login_client",
    srcs = ["cj_login_client.cc"],
    deps = [
        "//proto:cj_login_proto"
    ]
)
