//
//  FFShellPlugin.h
//  FormularLib
//
//  Created by 인식 조 on 12. 11. 14..
//  Copyright (c) 2012년 장재휴. All rights reserved.
//

#import <Cordova/CDVPlugin.h>
#import "MKLibrary.h"

@interface FFMDMPlugin : CDVPlugin <MKLibraryDelegate>
{
    MKLibrary *library;
    NSString *callbackId;
    NSDictionary *loginData;
    CDVPluginResult* pluginResult;
}
@end

