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

typedef enum {
    LoginViewStateLogout = 0,
    LoginViewStateLogined = 1
} LoginViewState;

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
        [self checkLogin:userName withTicket:loginTicket];
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
    [self login:userName password:password];
}

- (IBAction)onClickRegister:(id)sender {
    NSString *userName = self.userNameTextField.text;
    NSString *password = self.passwordTextField.text;
    if (userName.length > 0 && password.length > 0) {
        __block typeof(self) weakSelf = self;
        [weakSelf.loginSdk registerUser:userName
                               password:password
                                     cb:[[CJRegisterUserCallbackObjcImpl alloc] initWithBlock:^(int32_t errcode, NSString *errmsg) {
            if (errcode == 0) {
                [weakSelf login:userName password:password];
            } else {
                // Error Alert Here
                NSLog(@"register error: %d, errmsg: %@", errcode, errmsg);
            }
        }]];
    }
}

- (IBAction)onClickLogout:(id)sender {
    NSString *userName = [[NSUserDefaults standardUserDefaults] objectForKey:@"userName"];
    NSString *sessionKey = [[NSUserDefaults standardUserDefaults] objectForKey:@"sessionKey"];
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
                          withTicket:loginTicket];
            } else {
                NSLog(@"login error: %d, errmsg: %@", errcode, errmsg);
            }
        }]];
    }

}

- (void)checkLogin:(NSString *)userName withTicket:(NSString *)loginTicket {
    __block typeof(self) weakSelf = self;

    if (userName.length > 0 && loginTicket.length > 0) {
        [self.loginSdk checkLogin:userName
                      loginTicket:loginTicket
                               cb:[[CJCheckLoginCallbackObjcImpl alloc] initWithBlock:^(int32_t errcode, NSString *errmsg, NSString *sessionKey) {
            if (errcode == 0) {
                NSLog(@"checkLogin Success");
                [[NSUserDefaults standardUserDefaults] setObject:sessionKey
                                                          forKey:@"sessionKey"];
                [[NSUserDefaults standardUserDefaults] synchronize];
                // jump to logined View
                dispatch_async(dispatch_get_main_queue(), ^{
                    weakSelf.currentViewState = LoginViewStateLogined;
                    [weakSelf syncViewState];
                });
                
                [self connect:userName withSessionKey:sessionKey];
            } else {
                NSLog(@"checkLogin error: %d, errmsg: %@", errcode, errmsg);
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
                });
            } else {
                // Error Alert Here
                NSLog(@"logout error: %d, errmsg: %@", errcode, errmsg);
            }
        }]];
    }
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
