//
//  ViewController.m
//  CJLoginDemo
//
//  Created by jeasonzhu on 2018/8/12.
//  Copyright © 2018年 jeasonzhu. All rights reserved.
//

#import "ViewController.h"
#import "CJCjLogin.h"
#import "callback_objc_impl.h"
#import <Toast/Toast.h>

typedef enum {
    LoginViewStateLogout = 0,
    LoginViewStateLogined = 1
} LoginViewState;

typedef void(^CheckLoginComplete)(NSString *sessionKey);

@interface ViewController ()

@property (unsafe_unretained, nonatomic) IBOutlet UILabel *userNameLabel;
@property (unsafe_unretained, nonatomic) IBOutlet UITextField *userNameTextField;
@property (unsafe_unretained, nonatomic) IBOutlet UITextField *passwordTextField;
@property (unsafe_unretained, nonatomic) IBOutlet UILabel *passwordLabel;

@property (unsafe_unretained, nonatomic) IBOutlet UIButton *logoutButton;
@property (unsafe_unretained, nonatomic) IBOutlet UIButton *registerButton;
@property (unsafe_unretained, nonatomic) IBOutlet UIButton *loginButton;

@property (nonatomic, strong) CJCjLogin *loginSdk;
@property (nonatomic, assign) LoginViewState currentViewState;

@end

@implementation ViewController

- (NSString *)getFile:(NSString *)file ofType:(NSString *)type {
    NSString *path = [[NSBundle mainBundle] pathForResource:file ofType:type];
    NSError *error = nil;
    NSString *string = [NSString stringWithContentsOfFile:path
                                                 encoding:NSUTF8StringEncoding
                                                    error:&error];
    if (error) {
        NSLog(@"read file[%@] error: %@", file, [error description]);
    }
    return string;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    self.loginSdk = [CJCjLogin create:@"35.194.225.201:50052"
                           serverCert:[self getFile:@"server-self-signed.cert" ofType:@"pem"]
                                 cert:[self getFile:@"client-self-signed-cert" ofType:@"pem"]
                                  key:[self getFile:@"client.key" ofType:@"pem"]
                          sslOverride:@"Jeason"];
    NSString *sessionKey = [[NSUserDefaults standardUserDefaults] objectForKey:@"sessionKey"];
    NSString *userName = [[NSUserDefaults standardUserDefaults] objectForKey:@"userName"];
    NSString *loginTicket = [[NSUserDefaults standardUserDefaults] objectForKey:@"loginTicket"];

    NSLog(@"sessionKey: %@", sessionKey);
    if (sessionKey.length > 0) { // TODO: svr response session timeout time
        self.currentViewState = LoginViewStateLogined;
        __block typeof(self) weakSelf = self;
        [self checkLogin:userName
              withTicket:loginTicket
              completion:^(NSString *sessionKey) {
                  [weakSelf connect:userName withSessionKey:sessionKey];
              }];
    }
    [self syncViewState];
}

- (void)syncViewState {
    if (self.currentViewState == LoginViewStateLogout) {
        self.userNameLabel.hidden = self.userNameTextField.hidden = NO;
        self.passwordLabel.hidden = self.passwordTextField.hidden = NO;
        self.registerButton.hidden = self.loginButton.hidden = NO;
        
        self.userNameTextField.text = self.passwordTextField.text = @"";
        self.logoutButton.hidden = YES;
    } else {
        self.userNameLabel.hidden = self.userNameTextField.hidden = YES;
        self.passwordLabel.hidden = self.passwordTextField.hidden = YES;
        self.registerButton.hidden = self.loginButton.hidden = YES;
        
        self.logoutButton.hidden = NO;
    }
}

- (IBAction)onClickLogin:(id)sender {
    NSString *userName = self.userNameTextField.text;
    NSString *password = self.passwordTextField.text;
    [self.view makeToastActivity:CSToastPositionCenter];
    [self login:userName password:password];
}

- (IBAction)onClickRegister:(id)sender {
    NSString *userName = self.userNameTextField.text;
    NSString *password = self.passwordTextField.text;
    [self.view makeToastActivity:CSToastPositionCenter];

    if (userName.length > 0 && password.length > 0) {
        __block typeof(self) weakSelf = self;
        [weakSelf.loginSdk registerUser:userName
                               password:password
                                     cb:[[CJRegisterUserCallbackObjcImpl alloc] initWithBlock:^(int32_t errcode, NSString *errmsg) {
            if (errcode == 0 && errmsg.length > 0) {
                [weakSelf login:userName password:password];
            } else {
                // Error Alert Here
                NSString *messae = @"请稍后重试";
                if (errcode == REGISTER_ERROR_USERNAME_INVALID) {
                    messae = @"用户名不合法，只能包含字母数字及下划线";
                } else if (errcode == REGISTER_ERROR_USERNAME_EXISTS) {
                    messae = @"该用户名已被占用";
                } else if (errcode == REGISTER_ERROR_PWD_INVALID) {
                    messae = @"密码至少6位，且不能为纯数字或纯字母";
                }
                NSLog(@"register error: %d, errmsg: %@", errcode, errmsg);
                [self showErrorTitle:@"注册失败"
                             message:messae];
                dispatch_async(dispatch_get_main_queue(), ^{
                    [weakSelf.view hideToastActivity];
                });
            }
        }]];
    }
}

- (IBAction)onClickLogout:(id)sender {
    NSString *userName = [[NSUserDefaults standardUserDefaults] objectForKey:@"userName"];
    NSString *sessionKey = [[NSUserDefaults standardUserDefaults] objectForKey:@"sessionKey"];
    [self.view makeToastActivity:CSToastPositionCenter];
    [self logout:userName withSessionKey:sessionKey];
}

- (void)login:(NSString *)userName password:(NSString *)password {
    if (userName.length > 0 && password.length > 0) {
        __block typeof(self) weakSelf = self;
        [self.loginSdk login:userName
                    password:password
                          cb:[[CJLoginCallbackObjcImpl alloc] initWithBlock:^(int32_t errcode, NSString *errmsg, NSString *loginTicket) {
            if (errcode == 0 && loginTicket.length > 0) {
                [[NSUserDefaults standardUserDefaults] setObject:userName
                                                          forKey:@"userName"];
                [[NSUserDefaults standardUserDefaults] setObject:loginTicket
                                                          forKey:@"loginTicket"];
                [[NSUserDefaults standardUserDefaults] synchronize];
                [weakSelf checkLogin:userName
                          withTicket:loginTicket
                          completion:^(NSString *sessionKey) {
                              [weakSelf connect:userName withSessionKey:sessionKey];
                              dispatch_async(dispatch_get_main_queue(), ^{
                                  [weakSelf.view hideToastActivity];
                              });
                          }];
            } else {
                NSLog(@"login error: %d, errmsg: %@, ticket: %@", errcode, errmsg, loginTicket);
                NSString *message = @"请稍后重试";
                if (errcode == LOGIN_ERROR_PWD_ERROR || errcode == LOGIN_ERROR_USER_NOT_EXISTS) {
                    message = @"请检查账号密码是否正确";
                }
                [self showErrorTitle:@"登录失败"
                             message:message];
                dispatch_async(dispatch_get_main_queue(), ^{
                    [weakSelf.view hideToastActivity];
                });
            }
        }]];
    }

}

- (void)checkLogin:(NSString *)userName withTicket:(NSString *)loginTicket completion:(CheckLoginComplete)handler {
    __block typeof(self) weakSelf = self;

    if (userName.length > 0 && loginTicket.length > 0) {
        [self.loginSdk checkLogin:userName
                      loginTicket:loginTicket
                               cb:[[CJCheckLoginCallbackObjcImpl alloc] initWithBlock:^(int32_t errcode, NSString *errmsg, NSString *sessionKey) {
            if (errcode == 0 && sessionKey.length > 0) {
                NSLog(@"checkLogin Success");
                [[NSUserDefaults standardUserDefaults] setObject:sessionKey
                                                          forKey:@"sessionKey"];
                [[NSUserDefaults standardUserDefaults] synchronize];
                // jump to logined View
                dispatch_async(dispatch_get_main_queue(), ^{
                    weakSelf.currentViewState = LoginViewStateLogined;
                    [weakSelf syncViewState];
                });
                if (handler) {
                    handler(sessionKey);
                }
            } else {
                NSLog(@"checkLogin error: %d, errmsg: %@", errcode, errmsg);
                [self showErrorTitle:@"登录失败"
                             message:@"请重新登录"];
                dispatch_async(dispatch_get_main_queue(), ^{
                    weakSelf.currentViewState = LoginViewStateLogout;
                    [weakSelf syncViewState];
                });

            }
        }]];
    }
}

- (void)connect:(NSString *)userName withSessionKey:(NSString *)sessionKey {
    if (userName.length > 0 && sessionKey.length > 0) {
        __weak typeof (self) weakSelf = self;
        [self.loginSdk connect:userName
                    sessionKey:sessionKey
                            cb:[[CJConnectCallbackImpl alloc] initWithBlock:^(int32_t messageType, NSString *content) {
            if (messageType == 1 || messageType == 2) {
                NSLog(@"logout!");
                [[NSUserDefaults standardUserDefaults] removeObjectForKey:@"userName"];
                [[NSUserDefaults standardUserDefaults] removeObjectForKey:@"loginTicket"];
                [[NSUserDefaults standardUserDefaults] removeObjectForKey:@"sessionKey"];
                [[NSUserDefaults standardUserDefaults] synchronize];

                [self showErrorTitle:@"退出登录"
                             message:@"该账号在其他地方登录"];
                
                dispatch_async(dispatch_get_main_queue(), ^{
                    weakSelf.currentViewState = LoginViewStateLogout;
                    [weakSelf syncViewState];
                });
            }
        }]];
    }
}

- (void)logout:(NSString *)userName withSessionKey:(NSString *)sessionKey {
    if (userName.length > 0 && sessionKey.length > 0) {
        __weak typeof (self) weakSelf = self;
        [self.loginSdk logout:userName
                   sessionKey:sessionKey
                           cb:[[CJLogoutCallbackObjcImpl alloc] initWithBlock:^(int32_t errcode, NSString *errmsg) {
            if (errcode == 0) {
                [[NSUserDefaults standardUserDefaults] removeObjectForKey:@"userName"];
                [[NSUserDefaults standardUserDefaults] removeObjectForKey:@"loginTicket"];
                [[NSUserDefaults standardUserDefaults] removeObjectForKey:@"sessionKey"];
                [[NSUserDefaults standardUserDefaults] synchronize];
                dispatch_async(dispatch_get_main_queue(), ^{
                    weakSelf.currentViewState = LoginViewStateLogout;
                    [weakSelf syncViewState];
                    [weakSelf.view hideToastActivity];
                });
            } else {
                NSString *loginTicket = [[NSUserDefaults standardUserDefaults] objectForKey:@"loginTicket"];
                if (errcode == SYSTEM_SESSION_TIMEOUT && loginTicket.length > 0) {
                    [weakSelf checkLogin:userName
                              withTicket:loginTicket
                              completion:^(NSString *sessionKey) {
                        [weakSelf logout:userName withSessionKey:sessionKey];
                    }];
                } else {
                    // Error Alert Here
                    NSLog(@"logout error: %d, errmsg: %@", errcode, errmsg);
                    [self showErrorTitle:@"退出登录失败"
                                 message:@"稍后重试"];
                    dispatch_async(dispatch_get_main_queue(), ^{
                        [weakSelf.view hideToastActivity];
                    });
                }
            }
        }]];
    }
}

- (void)showErrorTitle:(NSString *)title message:(NSString *)message {
    dispatch_async(dispatch_get_main_queue(), ^{
        UIAlertController *alert = [UIAlertController alertControllerWithTitle:title
                                                                       message:message preferredStyle:UIAlertControllerStyleAlert];
        UIAlertAction *okAction = [UIAlertAction actionWithTitle:@"好的" style:UIAlertActionStyleDefault handler:nil];
        [alert addAction:okAction];
        [self presentViewController:alert
                           animated:YES
                         completion:nil];

    });
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
