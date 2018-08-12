package(default_visibility = ["//visibility:public"])
load("@build_bazel_rules_apple//apple:ios.bzl", "ios_application")

cc_library(
    name = "cj_login_sdk",
    hdrs = [
        "djinni-src/cpp/registerUserCallback.hpp",
        "djinni-src/cpp/cj_login.hpp",
        "src/cj_login_imp.hpp",
        "src/callback_impl.hpp",
        "src/cj_login_cgi_client.hpp"
    ],
    srcs = [
        "src/callback_impl.cc",
        "src/cj_login_imp.cc"
    ],
    deps = [
        "//proto:cj_login_proto"
    ]
)

objc_library(
    name = "Sources",
    srcs = glob([
        "src/iOS/*.mm",
        "deps/djinni/support-lib/*.mm"
    ]),
    hdrs = glob([
        "src/iOS/*.h",
        "deps/djinni/support-lib/*.h"
    ]),
    resources = glob([
        "src/iOS/Base.lproj/*.storyboard"
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

