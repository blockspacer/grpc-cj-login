//
//  callback_objc_impl.m
//  _idx_ares_36218299_ios_min11.4
//
//  Created by jeasonzhu on 2018/8/13.
//

#import "callback_objc_impl.h"

@interface CJLoginCallbackObjcImpl()

@property (nonatomic, copy) ObjcLoginCallback block;

@end

@implementation CJLoginCallbackObjcImpl

- (instancetype)initWithBlock:(ObjcLoginCallback)block {
    if (self = [super init]) {
        self.block = block;
        return self;
    }
}

+ (instancetype)callbackWithBlock:(ObjcLoginCallback)block {
    CJLoginCallbackObjcImpl *instance = [[CJLoginCallbackObjcImpl alloc] init];
    instance.block = block;
    return instance;
}

- (void)complete:(int32_t)errcode
          errmsg:(nonnull NSString *)errmsg
     loginTicket:(nonnull NSString *)loginTicket {
    if (self.block) {
        self.block(errcode, errmsg, loginTicket);
    }
}

@end
