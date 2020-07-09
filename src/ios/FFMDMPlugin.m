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
    // MKlibrary 객체 초기화
    if (!library) {
        library = [[MKLibrary alloc] init];
        library.delgate = self;
    }
    
    pluginResult = nil;
    callbackId = nil;
    loginData = nil;
    
    callbackId = command.callbackId;
    id params = nil;
    NSArray* arguments = command.arguments;
    
    NSError *error;
    if(arguments.count > 0)
        params = [NSJSONSerialization JSONObjectWithData:[[arguments objectAtIndex:0]dataUsingEncoding:NSUTF8StringEncoding]
                                                 options:NSJSONReadingMutableLeaves
                                                   error:&error];
    
    
    NSUserDefaults *userDefaults = [NSUserDefaults standardUserDefaults];
    
    
    [library mangobanana:"17121500"
                  domain:"mdm30ssl.eland.co.kr"
                    port:"44300"
                  userID:@""
                deviceID:[userDefaults objectForKey:@"wifimac"]];
    
}


#pragma mark - MKLibraryDelegate
- (void)mangobananaResult:(NSString *)code message:(NSString *)message {
    
    NSBundle *bundle = [NSBundle mainBundle];
    NSDictionary *info = [bundle infoDictionary];
    
    
    loginData = [NSDictionary dictionaryWithObjectsAndKeys:message, @"message",
                 [info objectForKey:@"CFBundleIdentifier"], @"packageName",
                 code, @"codeKey",
                 [[[NSLocale preferredLanguages] objectAtIndex:0] substringWithRange:NSMakeRange(0,2)], @"locale",
                 nil];
    
    pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsDictionary:loginData];
    [self.commandDelegate sendPluginResult:pluginResult callbackId:callbackId];
    
}


@end
