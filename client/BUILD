package(default_visibility = ["//visibility:public"])
load("@build_bazel_rules_apple//apple:ios.bzl", "ios_application")

cc_library(
    name = "cj_login_sdk",
    hdrs = glob([
        "djinni-src/*.hpp",
        "src/cj_login_imp.hpp",
        "src/callback_impl.hpp",
        "src/cj_login_cgi_client.hpp"
    ]),
    srcs = [
        "src/callback_impl.cc",
        "src/cj_login_imp.cc"
    ],
    deps = [
        "//proto:cj_login_proto"
    ]
)

cc_binary(
    name = "cj_login_test",
    srcs = [
        "src/Cpp/cj_login_tools.cc"
    ],
    deps = [
        ":cj_login_sdk"
    ]
)

objc_library(
    name = "Sources",
    srcs = glob([
        "djinni-src/*.mm",
        "src/iOS/*.mm"
    ]),
    hdrs = glob([
        "djinni-src/*.h",
        "src/iOS/*.h",
        "*.h",
        "*.hpp"
    ]),
    resources = glob([
        "src/iOS/Base.lproj/*.storyboard",
        "cert/*"
    ]),
    deps = [
        ":cj_login_sdk"
    ]
)

ios_application(
    name = "CJLoginDemo",
    bundle_id = "com.jeason.cjlogin",
    families = ["iphone"],
    infoplists = ["src/iOS/Info.plist"],
    deps = [":Sources"],
)

