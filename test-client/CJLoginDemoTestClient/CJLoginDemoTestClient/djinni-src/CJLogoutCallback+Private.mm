// AUTOGENERATED FILE - DO NOT MODIFY!
// This file generated by Djinni from cj-login.djinni

#import "CJLogoutCallback+Private.h"
#import "CJLogoutCallback.h"
#import "DJICppWrapperCache+Private.h"
#import "DJIError.h"
#import "DJIMarshal+Private.h"
#import "DJIObjcWrapperCache+Private.h"
#include <exception>
#include <stdexcept>
#include <utility>

static_assert(__has_feature(objc_arc), "Djinni requires ARC to be enabled for this file");

@interface CJLogoutCallbackCppProxy : NSObject<CJLogoutCallback>

- (id)initWithCpp:(const std::shared_ptr<::cjlogin::LogoutCallback>&)cppRef;

@end

@implementation CJLogoutCallbackCppProxy {
    ::djinni::CppProxyCache::Handle<std::shared_ptr<::cjlogin::LogoutCallback>> _cppRefHandle;
}

- (id)initWithCpp:(const std::shared_ptr<::cjlogin::LogoutCallback>&)cppRef
{
    if (self = [super init]) {
        _cppRefHandle.assign(cppRef);
    }
    return self;
}

- (void)complete:(int32_t)errcode
          errmsg:(nonnull NSString *)errmsg {
    try {
        _cppRefHandle.get()->complete(::djinni::I32::toCpp(errcode),
                                      ::djinni::String::toCpp(errmsg));
    } DJINNI_TRANSLATE_EXCEPTIONS()
}

namespace djinni_generated {

class LogoutCallback::ObjcProxy final
: public ::cjlogin::LogoutCallback
, private ::djinni::ObjcProxyBase<ObjcType>
{
    friend class ::djinni_generated::LogoutCallback;
public:
    using ObjcProxyBase::ObjcProxyBase;
    void complete(int32_t c_errcode, const std::string & c_errmsg) override
    {
        @autoreleasepool {
            [djinni_private_get_proxied_objc_object() complete:(::djinni::I32::fromCpp(c_errcode))
                                                        errmsg:(::djinni::String::fromCpp(c_errmsg))];
        }
    }
};

}  // namespace djinni_generated

namespace djinni_generated {

auto LogoutCallback::toCpp(ObjcType objc) -> CppType
{
    if (!objc) {
        return nullptr;
    }
    if ([(id)objc isKindOfClass:[CJLogoutCallbackCppProxy class]]) {
        return ((CJLogoutCallbackCppProxy*)objc)->_cppRefHandle.get();
    }
    return ::djinni::get_objc_proxy<ObjcProxy>(objc);
}

auto LogoutCallback::fromCppOpt(const CppOptType& cpp) -> ObjcType
{
    if (!cpp) {
        return nil;
    }
    if (auto cppPtr = dynamic_cast<ObjcProxy*>(cpp.get())) {
        return cppPtr->djinni_private_get_proxied_objc_object();
    }
    return ::djinni::get_cpp_proxy<CJLogoutCallbackCppProxy>(cpp);
}

}  // namespace djinni_generated

@end
