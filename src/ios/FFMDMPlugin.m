//
//  FFShellPlugin.m
//  FormularLib
//
//  Created by 인식 조 on 12. 11. 14..
//  Copyright (c) 2012년 장재휴. All rights reserved.
//

#import "FFMDMPlugin.h"
#import "FFEnvironmentInformationManager.h"
#import "FFNetworkHelper.h"
#import "MKLibrary.h"

@interface FFMDMPlugin (){
}
@property (nonatomic, strong) FFEnvironmentInformationManager *environmentInformationManager;
@end

@implementation FFMDMPlugin
@synthesize environmentInformationManager = _environmentInformationManager;

#pragma mark
- (id)init
{
    if (self = [super init]) {
    }
    return self;
}

- (void) check_app:(CDVInvokedUrlCommand*)command
{
    NSString* callbackId = command.callbackId;
    
    CDVPluginResult* pluginResult = nil;
    NSString* javaScript = nil;
    id params = nil;
    NSArray* arguments = command.arguments;
    
    NSError *error;
    if(arguments.count > 0)
        params = [NSJSONSerialization JSONObjectWithData:[[arguments objectAtIndex:0]dataUsingEncoding:NSUTF8StringEncoding]
                                                 options:NSJSONReadingMutableLeaves
                                                   error:&error];
    
    
    //    NSDictionary *loginData = [NSDictionary dictionaryWithObjectsAndKeys:@"jo_insik", @"message",
    //                               @"com.eland.RBMS", @"packageName",
    //                               @"0x000000", @"codeKey",
    //                               nil];
    NSDictionary *loginData = [self certificateMDM];
    
    pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsDictionary:loginData];
    [self.commandDelegate sendPluginResult:pluginResult callbackId:callbackId];
    
    //    pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK];
    //    javaScript = [pluginResult toSuccessCallbackString:callbackId];
    //    [self writeJavascript:javaScript];
}


#pragma mark - Private Methods

-(NSDictionary *)certificateMDM{
    NSDictionary *loginData = nil;
    NSUserDefaults *userDefaults = [NSUserDefaults standardUserDefaults];
    NSString *deviceId;
    NSBundle *bundle = [NSBundle mainBundle];
    NSDictionary *info = [bundle infoDictionary];
    
    if([[[UIDevice currentDevice] systemVersion] floatValue] >= 7.0)
        deviceId =[userDefaults objectForKey:@"wifimac"];
    else
        deviceId = [FFNetworkHelper macAddress];
    
    //    NSDictionary *param = [NSDictionary dictionaryWithObjectsAndKeys:deviceId, @"deviceId",
    //                           [[NSLocale preferredLanguages] objectAtIndex:0], @"locale",
    //                           [info objectForKey:@"CFBundleIdentifier"], @"appId",
    //                           nil];
    
    NSMutableDictionary *result = [MkLibrary Request_Certification_Sync:@"mdmssl.eland.co.kr:44400"
                                                             apploginid:@"1111"
                                                                  INOut:1
                                                                Company:@"11121400"
                                                               deviceID:deviceId];
    
    NSLog(@"인증결과: %@", result);
    NSLog(@"결과: %@", [result valueForKey:@"Result"]);
    NSLog(@"에러코드: %@", [result valueForKey:@"ErrorCode"]);
    NSLog(@"응답메시지: %@", [result valueForKey:@"Message"]);
    
    loginData = [NSDictionary dictionaryWithObjectsAndKeys:[result valueForKey:@"Message"], @"message",
                 [info objectForKey:@"CFBundleIdentifier"], @"packageName",
                 [result valueForKey:@"ErrorCode"], @"codeKey",
                 [[[NSLocale preferredLanguages] objectAtIndex:0] substringWithRange:NSMakeRange(0,2)], @"locale",
                 nil];
    
    return loginData;
}

@end
