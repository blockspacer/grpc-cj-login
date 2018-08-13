//
//  callback_objc_impl.h
//  _idx_ares_36218299_ios_min11.4
//
//  Created by jeasonzhu on 2018/8/13.
//

#import <Foundation/Foundation.h>
#import "djinni-src/CJLoginCallback.h"
#import "djinni-src/CJRegisterUserCallback.h"
#import "djinni-src/CJCheckLoginCallback.h"
#import "djinni-src/CJLogoutCallback.h"

typedef void(^ObjcRegisterUserCallback)(int32_t errcode, NSString *errmsg);
typedef void(^ObjcLoginCallback)(int32_t errcode, NSString *errmsg, NSString *loginTicket);
typedef void(^ObjcCheckLoginCallback)(int32_t errcode, NSString *errmsg, NSString *sessionKey);
typedef void(^ObjcLogoutCallback)(int32_t errcode, NSString *errmsg);

@interface CJLoginCallbackObjcImpl : NSObject<CJLoginCallback>

- (instancetype)initWithBlock:(ObjcLoginCallback)block;

@property (nonatomic, copy) ObjcLoginCallback block;

@end

@interface CJRegisterUserCallbackObjcImpl: NSObject<CJRegisterUserCallback>

- (instancetype)initWithBlock:(ObjcRegisterUserCallback)block;

@property (nonatomic, copy) ObjcRegisterUserCallback block;

@end

@interface CJCheckLoginCallbackObjcImpl: NSObject<CJCheckLoginCallback>

- (instancetype)initWithBlock:(ObjcCheckLoginCallback)block;

@property (nonatomic, copy) ObjcCheckLoginCallback block;

@end

@interface CJLogoutCallbackObjcImpl: NSObject<CJLogoutCallback>

- (instancetype)initWithBlock:(ObjcLogoutCallback)block;

@property (nonatomic, copy) ObjcLogoutCallback block;

@end
