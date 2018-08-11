// AUTOGENERATED FILE - DO NOT MODIFY!
// This file generated by Djinni from cj-login.djinni

#pragma once

#include "cj_login.hpp"
#include "djinni_support.hpp"

namespace djinni_generated {

class NativeCjLogin final : ::djinni::JniInterface<::cjlogin::CjLogin, NativeCjLogin> {
public:
    using CppType = std::shared_ptr<::cjlogin::CjLogin>;
    using CppOptType = std::shared_ptr<::cjlogin::CjLogin>;
    using JniType = jobject;

    using Boxed = NativeCjLogin;

    ~NativeCjLogin();

    static CppType toCpp(JNIEnv* jniEnv, JniType j) { return ::djinni::JniClass<NativeCjLogin>::get()._fromJava(jniEnv, j); }
    static ::djinni::LocalRef<JniType> fromCppOpt(JNIEnv* jniEnv, const CppOptType& c) { return {jniEnv, ::djinni::JniClass<NativeCjLogin>::get()._toJava(jniEnv, c)}; }
    static ::djinni::LocalRef<JniType> fromCpp(JNIEnv* jniEnv, const CppType& c) { return fromCppOpt(jniEnv, c); }

private:
    NativeCjLogin();
    friend ::djinni::JniClass<NativeCjLogin>;
    friend ::djinni::JniInterface<::cjlogin::CjLogin, NativeCjLogin>;

};

}  // namespace djinni_generated
