//
//  main.m
//  CJLoginDemo
//
//  Created by jeasonzhu on 2018/8/12.
//  Copyright © 2018年 jeasonzhu. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "AppDelegate.h"

int main(int argc, char * argv[]) {
    @autoreleasepool {
//        CJCjLogin *login = [CJCjLogin create];
//        CJLoginCallbackObjcImpl *cb = [[CJLoginCallbackObjcImpl alloc] initWithBlock:^(int32_t errcode, NSString *errmsg, NSString *loginTicket) {
//            NSLog(@"errcode:%d, errmsg:%@, loginTicket:%@", errcode, errmsg, loginTicket);
//        }];
//        [login login:@"jeason"
//            password:@"zhu"
//                  cb:cb];
        return UIApplicationMain(argc, argv, nil, NSStringFromClass([AppDelegate class]));
    }
}
