// AUTOGENERATED FILE - DO NOT MODIFY!
// This file generated by Djinni from cj-login.djinni

#import "CJCjLogin+Private.h"
#import "CJCjLogin.h"
#import "DJICppWrapperCache+Private.h"
#import "DJIError.h"
#include <exception>
#include <stdexcept>
#include <utility>

static_assert(__has_feature(objc_arc), "Djinni requires ARC to be enabled for this file");

@interface CJCjLogin ()

- (id)initWithCpp:(const std::shared_ptr<::cjlogin::CjLogin>&)cppRef;

@end

@implementation CJCjLogin {
    ::djinni::CppProxyCache::Handle<std::shared_ptr<::cjlogin::CjLogin>> _cppRefHandle;
}

- (id)initWithCpp:(const std::shared_ptr<::cjlogin::CjLogin>&)cppRef
{
    if (self = [super init]) {
        _cppRefHandle.assign(cppRef);
    }
    return self;
}

+ (nullable CJCjLogin *)create {
    try {
        auto objcpp_result_ = ::cjlogin::CjLogin::create();
        return ::djinni_generated::CjLogin::fromCpp(objcpp_result_);
    } DJINNI_TRANSLATE_EXCEPTIONS()
}

namespace djinni_generated {

auto CjLogin::toCpp(ObjcType objc) -> CppType
{
    if (!objc) {
        return nullptr;
    }
    return objc->_cppRefHandle.get();
}

auto CjLogin::fromCppOpt(const CppOptType& cpp) -> ObjcType
{
    if (!cpp) {
        return nil;
    }
    return ::djinni::get_cpp_proxy<CJCjLogin>(cpp);
}

}  // namespace djinni_generated

@end
