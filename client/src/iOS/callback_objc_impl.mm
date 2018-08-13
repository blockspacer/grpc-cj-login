//
//  callback_objc_impl.m
//  _idx_ares_36218299_ios_min11.4
//
//  Created by jeasonzhu on 2018/8/13.
//

#import "callback_objc_impl.h"

@implementation CJLoginCallbackObjcImpl

- (instancetype)initWithBlock:(ObjcLoginCallback)block {
    if (self = [super init]) {
        self.block = block;
    }
    return self;
}

- (void)complete:(int32_t)errcode
          errmsg:(nonnull NSString *)errmsg
     loginTicket:(nonnull NSString *)loginTicket {
    if (self.block) {
        self.block(errcode, errmsg, loginTicket);
    }
}

@end

@implementation CJRegisterUserCallbackObjcImpl

- (instancetype)initWithBlock:(ObjcRegisterUserCallback)block {
    if (self = [super init]) {
        self.block = block;
    }
    return self;
}

- (void)complete:(int32_t)errcode errmsg:(NSString *)errmsg {
    if (self.block) {
        self.block(errcode, errmsg);
    }
}

@end

@implementation CJCheckLoginCallbackObjcImpl

- (instancetype)initWithBlock:(ObjcCheckLoginCallback)block {
    if (self = [super init]) {
        self.block = block;
    }
    return self;
}

- (void)complete:(int32_t)errcode errmsg:(NSString *)errmsg sessionKey:(NSString *)sessionKey {
    if (self.block) {
        self.block(errcode, errmsg, sessionKey);
    }
}

@end

@implementation CJLogoutCallbackObjcImpl

- (instancetype)initWithBlock:(ObjcLogoutCallback)block {
    if (self = [super init]) {
        self.block = block;
    }
    return self;
}

- (void)complete:(int32_t)errcode errmsg:(NSString *)errmsg {
    if (self.block) {
        self.block(errcode, errmsg);
    }
}

@end
