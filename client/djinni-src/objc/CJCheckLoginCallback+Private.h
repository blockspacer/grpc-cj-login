// AUTOGENERATED FILE - DO NOT MODIFY!
// This file generated by Djinni from cj-login.djinni

#include "checkLoginCallback.hpp"
#include <memory>

static_assert(__has_feature(objc_arc), "Djinni requires ARC to be enabled for this file");

@class CJCheckLoginCallback;

namespace djinni_generated {

class CheckLoginCallback
{
public:
    using CppType = std::shared_ptr<::cjlogin::CheckLoginCallback>;
    using CppOptType = std::shared_ptr<::cjlogin::CheckLoginCallback>;
    using ObjcType = CJCheckLoginCallback*;

    using Boxed = CheckLoginCallback;

    static CppType toCpp(ObjcType objc);
    static ObjcType fromCppOpt(const CppOptType& cpp);
    static ObjcType fromCpp(const CppType& cpp) { return fromCppOpt(cpp); }

private:
    class ObjcProxy;
};

}  // namespace djinni_generated

