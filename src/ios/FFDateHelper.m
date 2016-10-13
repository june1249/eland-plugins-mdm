//
//  FFDateHelper.m
//  FormularLib
//
//  Created by 인식 조 on 12. 11. 13..
//  Copyright (c) 2012년 장재휴. All rights reserved.
//

#import "FFDateHelper.h"

@implementation FFDateHelper
+ (NSDate *)dateFromString:(NSString *)dateString
{
    NSDate *date = nil;
    NSDateFormatter *dateFormatter = [[NSDateFormatter alloc]init];
    NSError *error = nil;
    [dateFormatter setDateFormat:@"yyyy-MM-dd'T'HH:mm:ss"];
    [dateFormatter setLocale:[[NSLocale alloc] initWithLocaleIdentifier:@"en_US_POSIX"]];
    if (![dateFormatter getObjectValue:&date forString:dateString range:nil error:&error]) {
        NSLog(@"Date '%@' could not be parsed: %@", dateString, error);
    }
    return date;
}

+ (NSString *) getDate
{
    // get the current date
    NSDate *date = [NSDate date];
    
    // format it
    NSDateFormatter *dateFormat = [[NSDateFormatter alloc]init];
    [dateFormat setDateFormat:@"HH:mm:ss zzz"];
    
    // convert it to a string
    NSString *dateString = [dateFormat stringFromDate:date];
    
    return dateString;
}

@end
