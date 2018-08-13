//
//  callback_objc_impl.h
//  _idx_ares_36218299_ios_min11.4
//
//  Created by jeasonzhu on 2018/8/13.
//

#import <Foundation/Foundation.h>
#import "djinni-src/CJLoginCallback.h"

typedef void(^ObjcLoginCallback)(int32_t errcode, NSString *errmsg, NSString *loginTicket);

@interface CJLoginCallbackObjcImpl : NSObject<CJLoginCallback>

+ (instancetype)callbackWithBlock:(ObjcLoginCallback)block;

- (instancetype)initWithBlock:(ObjcLoginCallback)block;

@end
