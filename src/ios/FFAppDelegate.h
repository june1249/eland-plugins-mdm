//
//  FFAppDelegate.h
//  FormularLib
//
//  Created by 장재휴 on 12. 11. 7..
//  Copyright (c) 2012년 장재휴. All rights reserved.
//

#import <UIKit/UIKit.h>
dispatch_semaphore_t sema;
NSCondition* condition;
BOOL isFinished;
@interface FFAppDelegate : UIResponder <UIApplicationDelegate>

@property (strong, nonatomic) UIWindow *window;

@property (strong, nonatomic) UIView *inputAccessoryView;

@end
