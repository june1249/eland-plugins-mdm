//
//  MKLibrary.h
//  MKLibrary
//
//  Created by JuYoung Kim on 12. 6. 1..
//
//  Modified by vivifier on 13. 11. 15..
//
//	Modified by glass002 on 18. 2. 13..
//  Copyright©JiranSecurity. All Rights Reserved
// 

#import <Foundation/Foundation.h>
#import <CoreFoundation/CoreFoundation.h>
#import <UIKit/UIKit.h>
#import <objc/runtime.h>

@protocol MKLibraryDelegate;

/**
 
 이 라이브러리는 MDM 구축을 위해 응용 어플리케이션에 제공하는 라이브러리로 
 응용 어플리케이션에서 MDM과의 단말 인증을 이용하는 방법에 대해서 설명합니다.
 
 <br>
 
 
 # 지원 환경 #
 
 iOS 7.0 이상
 
 <br>
 
 
 # 파일 구성 #
 
 라이브러리를 정상적으로 사용하기 위해 반드시 아래 파일들이 반드시 포함되어야 합니다.
 
 | 파일명					| 설명			|
 |----------------------|---------------|
 | libMKLibrary.a		| 라이브러리 파일	|
 | MKLibrary.h			| 헤더 파일		|
 | JSCheckDevice.crt	| MDM 인증서 파일	|
 
 
 # 포함해야할 프레임워크 #
 
 라이브러리를 정상적으로 사용하기 위해 반드시 포함해야 할 프레임워크는 다음과 같습니다.
 
 - `SystemConfiguration.framework`
 - `UIKit.framework`              
 - `Foundation.framework`
 - `Security.framework`
 - `libz.1.2.5.tbd`
 
 <br>
 
 
 # 필수 확인사항 #
 
 다음 메서드들은 반드시 `deviceID` 파라미터에 WiFi-MAC이나 UDID 값을 전달해서 호출해야 합니다.
 
 - `mangobanana:domain:port:userID:deviceID:`
 - `appWillResignActive:domain:port:deviceID:`
 - `appDidBecomeActive:domain:port:deviceID:`
 
 `deviceID` 파라미터를 누락하거나 `nil`로 전송하면 다음 오류코드와 오류메시지가 돌아옵니다.
 
 | 오류코드		| 오류메시지					|
 |--------------|---------------------------|
 | 0xD8000021	| 디바이스 ID 파라미터가 필요합니다.	|
 
 
 # WiFi-MAC이나 UDID를 얻는 방법 #
 
 WiFi-MAC 이나 UDID는 UrlScheme으로 MDM 앱을 호출하여 응답받는 방법으로 얻을 수 있습니다.
 이 과정이 정상적으로 진행되려면 다음 사항을 충족해야합니다.
 
 - MDM 앱이 설치되어 있어야 합니다.
 - MDM 앱이 최신버전이어야 합니다. (앱 인증을 위한 UrlScheme이 포함된 버전)
 - MDM 앱이 정상적인 상태여야 합니다. (로그인 및 기기등록 과정이 정상적으로 완료된 상태. MDM 프로파일 또한 정상적인 상태)
 - 앱인증을 수행하는 앱도 WiFi-MAC이나 UDID를 전달받을 수 있도록 UrlScheme이 존재해야 합니다.
 
 이 조건을 모두 만족한다면 `deviceID` 파라미터에 전달할 WiFi-MAC이나 UDID값을 MDM 앱으로부터 얻어올 수 있습니다.
 
 
 <br>
 
 # UrlScheme 등록 #
 
 UrlScheme은 Xcode의 TARGETS - Info 탭 - URL Types 항목에서 추가할 수 있습니다.
 
 이미 UrlScheme을 사용하고 있다면 Sub URL을 통해 WiFi-MAC이나 UDID를 전달받아도 되고
 다른 이름의 UrlScheme을 하나 더 추가하여 전달받아도 됩니다.
 
 
 <br>
 
 # MDM 앱 UrlScheme 호출 #
 
 WiFi-MAC이나 UDID를 얻기 위해 MDM 앱을 다음 방법으로 호출합니다.
 
    NSURL *mdmUrl = [NSURL URLWithString:@"mobilekeeper://"];
    NSURL *mdmUdidUrl = [NSURL URLWithString:@"mobilekeeperinfo://getudid?delegatescheme=yoururlscheme://responseudid="];
 
    // MDM 설치됨
    if ([[UIApplication sharedApplication] canOpenURL:mdmUrl]) {
 
        // iOS 7 지원 MDM 확인
        if ([[UIApplication sharedApplication] canOpenURL:mdmUdidUrl]) {
 
            // Url Scheme 호출
            [[UIApplication sharedApplication] openURL:mdmUdidUrl];
 
        } else {
            // MDM 앱 업데이트 필요
        }
 
    } else {
        // MDM 앱 설치 필요
    }
 
 응답받을 UrlScheme을 `yoururlscheme://responseudid=` 부분에 넣습니다. 
 MDM 앱이 정상적으로 설치되어 있다고 가정하고
 
    @"mobilekeeperinfo://getudid?delegatescheme=응답받을 UrlScheme"
 
 을 호출하면 MDM 앱에서는 `응답받을 UrlScheme`의 뒤에 UDID를 붙여서 업무 앱을 다시 호출합니다.
 WiFi-MAC을 얻으려면 `getudid` 대신 `getwifimac` 을 호출하면 됩니다.
 
    @"mobilekeeperinfo://getwifimac?delegatescheme=응답받을 UrlScheme"
 
 openURL을 사용하여 UDID 요청시 10초 정도의 딜레이가 발생할 경우는 아래와 같이 수정해 주십시오.

     dispatch_async(dispatch_get_main_queue(), ^{
        [[UIApplication sharedApplication] openURL:mdmUdidUrl];
     });"
 
 <br>
 
 # UIApplicationDelegate 의 application:openURL:sourceApplication:annotation: 구현 #
 
 MDM 앱에서 업무앱을 UrlScheme으로 다시 호출하면 UIApplicationDelegate의 openUrl 메서드가 호출됩니다.
 이 프로토콜을 구현하여 WiFi-MAC 이나 UDID를 추출합니다.

     // iOS8 이하용
     - (BOOL)application:(UIApplication *)application openURL:(NSURL *)url 
                                            sourceApplication:(NSString *)sourceApplication 
                                                   annotation:(id)annotation
     {
        NSString *strURL = [url absoluteString];
     
        if (!strURL) {
            NSLog(@"url is nil");
            return NO;
        }
 
        // 일반적으로 sourceApplication 에는 UrlScheme을 호출한 앱(MDM Agent)의 앱 아이디가 넘어옵니다.
        // 만일 비정상적으로 이 값이 nil 일 경우 아래 부분을 주석처리하고 진행해도 무방합니다.
        if (!sourceApplication) {
            NSLog(@"sourceApplication is nil");
            return NO;
        }

        NSError *error = NULL;
     
        // URL 파싱을 수월하게 하기 위해 정규표현식 사용
        NSRegularExpression *regex = [NSRegularExpression regularExpressionWithPattern:@"yoururlscheme://(\\w+)=(.+)"
                                                                               options:NSRegularExpressionCaseInsensitive                                                                              
                                                                                 error:&error];
        
        if (error) {
            return NO;
        }
     
        NSTextCheckingResult *match = [regex firstMatchInString:strURL
                                                        options:0
                                                          range:NSMakeRange(0, [strURL length])];
     
        if (!match) {
            return NO;
        }
     
        NSString *param = [strURL substringWithRange:[match rangeAtIndex:1]];
        NSString *value = [strURL substringWithRange:[match rangeAtIndex:2]];
     
        if ([param isEqualToString:@"responseudid"]) {
 
            // MDM 앱이 비정상적일 경우에는 UDID값을 가지고 있지 않습니다.
            // 이 경우 @"(null)" 을 응답합니다.
            if ([value isEqualToString:@"(null)"]) {
                
                NSLog(@"Cannot get UDID");
                return NO;
 
            } else {

                NSLog(@"UDID: %@", value);
 
                // 응답받은 UDID 값을 저장하여 다음 앱 인증에도 사용합니다.
                // 물론 매번 UrlScheme을 호출하여 인증해도 되지만 앱 전환 속도가 상당히 느릴 수 있습니다.
                // 간편하게 저장해서 사용하기 위해 이 예제에서는 NSUserDefaults를 사용합니다.
                NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
 
                [defaults setObject:value forKey:@"UDID"];
                [defaults synchronize];
 
                // 나중에 저장된 값을 확인하기 위해서는 다음과 같이 호출하면 됩니다.
                // NSString *udid = [defaults objectForKey:@"UDID"];
 
                return YES;
            }
        }
     
        return NO;
    }
 
    // iOS9 이상용
    - (BOOL)application:(UIApplication *)app openURL:(NSURL *)url options:(NSDictionary *)options
    {
        NSString * sourceApplication = [options objectForKey:@"UIApplicationOpenURLOptionsSourceApplicationKey"];
 
        NSString *strURL = [url absoluteString];
 
        if (!strURL) {
            NSLog(@"url is nil");
            return NO;
        }
 
        // 일반적으로 sourceApplication 에는 UrlScheme을 호출한 앱(MDM Agent)의 앱 아이디가 넘어옵니다.
        // 만일 비정상적으로 이 값이 nil 일 경우 아래 부분을 주석처리하고 진행해도 무방합니다.
        if (!sourceApplication) {
            NSLog(@"sourceApplication is nil");
            return NO;
        }

        NSError *error = NULL;

        // URL 파싱을 수월하게 하기 위해 정규표현식 사용
        NSRegularExpression *regex = [NSRegularExpression regularExpressionWithPattern:@"yoururlscheme://(\\w+)=(.+)"
                                                                               options:NSRegularExpressionCaseInsensitive
                                                                                 error:&error];

        if (error) {
            return NO;
        }

        NSTextCheckingResult *match = [regex firstMatchInString:strURL
                                                        options:0
                                                          range:NSMakeRange(0, [strURL length])];

        if (!match) {
            return NO;
        }

        NSString *param = [strURL substringWithRange:[match rangeAtIndex:1]];
        NSString *value = [strURL substringWithRange:[match rangeAtIndex:2]];

        if ([param isEqualToString:@"responseudid"]) {
            // MDM 앱이 비정상적일 경우에는 UDID값을 가지고 있지 않습니다.
            // 이 경우 @"(null)" 을 응답합니다.
            if ([value isEqualToString:@"(null)"]) {
                NSLog(@"Cannot get UDID");
                return NO;
            } else {
                NSLog(@"UDID: %@", value);

                // 응답받은 UDID 값을 저장하여 다음 앱 인증에도 사용합니다.
                // 물론 매번 UrlScheme을 호출하여 인증해도 되지만 앱 전환 속도가 상당히 느릴 수 있습니다.
                // 간편하게 저장해서 사용하기 위해 이 예제에서는 NSUserDefaults를 사용합니다.
                NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];

                [defaults setObject:value forKey:@"UDID"];
                [defaults synchronize];

                // 나중에 저장된 값을 확인하기 위해서는 다음과 같이 호출하면 됩니다.
                // NSString *udid = [defaults objectForKey:@"UDID"];
                return YES;
            }
        }
        return NO;
     }
 
 MDM 앱이 비정상적일 경우 (기기등록에 실패했거나 MDM 프로파일이 삭제된 경우)에는 `@"(null)"` 을 리턴합니다.
 이 경우 다시한번 시도를 해보거나 MDM 앱의 설치상태를 확인해야 합니다.
 
 이런 일련의 과정을 거쳐 MDM 앱으로부터 WiFi-MAC이나 UDID를 얻으면
 앱의 저장 공간에 두고 계속 앱 인증시 마다 재사용해도 되고 매번 다시 얻어서 사용해도 되지만
 화면전환이 일어나는 등의 불편함이 있으니 저장해 놓고 사용하는 것을 권장합니다.
 
 
 <br>
 
 # 개정이력 #
 
 | 버전	| 변경일			| 변경내용																													| 작성자		| 승인자		|
 |------|---------------|-----------------------------------------------------------------------------------------------------------------------|-----------|-----------|
 |  1	| 2012.06.07	| 최초작성																													| 김주영		| 염태진		|
 |  2	| 2012.06.26	| 앱 해시방법 변경 및 API 파라미터 변경																							| 김주영		| 염태진		|
 |  3	| 2012.06.29	| 앱 해시 등록 및 검색 API 추가																								| 김주영		| 염태진		|
 |  4	| 2012.07.02	| 앱 해시 시간 및 비동기 방식 추가																								| 김주영		| 염태진		|
 |  5	| 2012.07.02	| Documents/res 해시 및 신한은행요청 추가																						| 김주영		| 염태진		|
 |  6	| 2012.07.19	| 앱 해시 추가 및 변경																										| 김주영		| 염태진		|
 |  7	| 2012.07.24	| 앱 해시 함수 변경 (추가경로)																									| 김주영		| 염태진		|
 |  8	| 2012.09.19	| iOS 6 지원																												| 김주영		| 염태진		|
 |  9	| 2012.10.12	| 연결 Timeout 시간 20초로 수정																								| 김주영		| 염태진		|
 | 10   | 2012.10.18	| Xcode 3 (SDK 4.2) 지원																									| 김주영		| 염태진		|
 | 11	| 2012.10.22	| 연결 Timeout 시간 30초로 수정																								| 김주영		| 염태진		|
 | 12	| 2012.10.26	| 요청한 명령(함수)의 종류를 결과에 출력																							| 김주영		| 염태진		|
 | 13	| 2012.10.31	| libz.1.2.5.dylib 라이브러리 추가																							| 김주영		| 염태진		|
 | 14	| 2013.02.18	| 앱 인증요청에 앱 버전정보 추가																								| 김주영		| 염태진		|
 | 15   | 2013.02.20	| API 2.1 버전에 최적화																										| 김주영		| 염태진		|
 | 16	| 2013.02.28	| 블루투스 활성화 확인 기능 추가																								| 김주영		| 염태진		|
 | 17	| 2013.03.18	| Xcode 3 버전 기기/시뮬레이터 지원<br>시스템에 따라 3가지 파일로 구분																	| 김주영		| 염태진		|
 | 18	| 2013.03.21	| 앱 인증요청 함수 오류 수정																									| 김주영		| 염태진		|
 | 19	| 2013.03.22	| 버전정보 추출 변경																											| 김주영		| 염태진		|
 | 20	| 2013.04.02	| 블루투스 검색 방법 옵션 추가<br>블루투스 검색 비동기 함수 추가																		| 김주영		| 염태진		|
 | 21	| 2013.08.21	| 등록된 해시 조회 함수 오류 수정<br>iOS 7 대응 파라미터 추가 (DeviceID)																| 이영종		| 염태진		|
 | 22	| 2013.11.15	| 인증서버 주소에 포트를 함께 입력하도록 수정																						| 이영종		| 염태진		|
 | 23	| 2013.11.26	| 앱 관리자 확인 메서드 추가<br>해시 등록시 관리자 계정 입력하도록 수정																	| 이영종		| 염태진		|
 | 24	| 2014.08.07	| iOS 8 해시함수 오류 수정																									| 이영종		| 염태진		|
 | 25	| 2014.08.13	| NSData 카테고리 제거 (TBXML)																								| 이영종		| 염태진		|
 | 26	| 2014.09.18	| 앱 포그라운드 / 백그라운드 진입 메서드 추가 (API v2.6.1 이상)<br>탈옥 탐지 방식 개선													| 이영종		| 염태진		|
 | 27   | 2015.06.03	| 앱 정책에서 WiFi / Bluetooth / Rooting 유무 확인 추가<br>라이브러리 버전 확인 메서드 추가<br>앱인증/무결성검사에 사용자 ID인증 여부 추가		| 김도훈		| 염태진		|
 | 28	| 2015.12.22	| End To End Encryption 적용																								| 김도훈		| 염태진		|
 | 29	| 2016.01.20	|  + (BOOL)Get_JailBroken  -> + (NSMutableDictionary \*)jsAfford														| 김도훈		| 염태진		|
 |		|				|  + (NSString \*)Get_WiFiSSID  -> + (NSMutableDictionary \*)jsBelieve													|			|			|
 |		|				|  + (NSMutableDictionary \*)Request_Certification_Sync     -> + (NSMutableDictionary \*)jsEffect						|			|			|
 |		|				|  + (NSMutableDictionary \*)Request_Certification          -> + (NSMutableDictionary \*)jsEncourage					|			|			|
 |		|				|  + (NSMutableDictionary \*)Request_ApplicationCheck_Sync  -> + (NSMutableDictionary \*)jsEither						|			|			|
 |		|				|  + (NSMutableDictionary \*)Request_ApplicationCheck       -> + (NSMutableDictionary \*)jsEffort						|			|			|
 |		|				|  <p style="color:red;font-weight:bold;">############### 반환값에 대한 내용 ###############</p>								|			|			|
 |		|				|  <p style="color:red;">jailbreak 검사	: JSDRA 파라미터의 Value 끝에서 두번째 자리가 E/e면 탈옥, N/n이면 비탈옥</p>					|			|			|
 |		|				|  <p style="color:red;">wifi 사용 여부	: JSES 파라미터의 Value 9 번째 자리 가 E/e 면 WiFi연결상태, N/n 이면 WiFi 연결 아님</p>		|			|			|
 |		|				|  <p style="color:red;">기타 반환값		: error 일 경우는 Error Message는 Resource.xml 참조,<br />Success 일 경우는 ErrorCode에서 0x 를 뺀 뒤 8자리와 Result 앞 8자리가 같을 경우 성공</p>	|			|			|
 | 30	| 2017.10.11	| iOS 11에서 bundlefilepath로 추출되는 파일순서가 바뀌어 추출되는 단말기가 있어 해쉬값이 달라지는 문제 수정  									| 김도훈		| 염태진		|
 | 31	| 2018.02.07	| 버전업 : 3.0.1.0  																										| 강명환		| 염태진		|
 |		|				| 불필요한 API 제거																											|			| 		 	|
 |		|				|  + (NSMutableDictionary *)jsEffect																					|			| 		 	|
 |		|				|  - (void)jsEncourage																									|			| 		 	|
 |		|				|  + (NSMutableDictionary *)jsEither																					|			| 		 	|
 |		|				|  - (void)jsEffort																										|			| 		 	|
 |		|				|  + (NSMutableDictionary *)isAppManagerAccountSync																		|			| 		 	|
 |		|				|  - (void)isAppManagerAccount																							|			| 		 	|
 |		|				|  + (NSMutableDictionary *)Request_Registration_AppHash_Sync															|			| 		 	|
 |		|				|  - (void)Request_Registration_AppHash																					|			| 		 	|
 |		|				|  + (NSMutableDictionary *)Request_Get_RegisteredAppHash_Sync															|			| 		 	|
 |		|				|  - (void)Request_Get_RegisteredAppHash																				|			| 		 	|
 |		|				|  + (NSMutableDictionary *)Send_ScreenCapture_Sync																		|			| 		 	|
 |		|				|  - (void)Send_ScreenCapture																							|			| 		 	|
 |		|				|  + (NSMutableDictionary *)Get_Application_Hash_Execution_Sync															|			| 		 	|
 |		|				|  + (NSInteger)Get_BluetoothEnable:(id)observer Selector:(SEL)selector Option:(NSInteger)option						|			| 		 	|
 |		|				|  + (NSMutableDictionary *)jsBelieve																					|			| 		 	|
 |		|				|  - (void)Get_BluetoothEnable:(NSInteger)option																		|			| 		 	|
 |		|				|  + (NSMutableDictionary *)jsAfford																					|			| 		 	|
 |		|				|  + (NSMutableDictionary *)Request_Get_AppInfo_Policy:(NSString *)host Company:(NSString *)code						|			| 		 	|
 |		|				|  <p style="color:red;font-weight:bold;">############### 반환값에 대한 내용 ###############</p>								|			|			|
 |		|				|  <p style="color:red;">Error 일 경우는 Error Message는 Resource.xml 참조,<br />Success 일 경우는 0x00000000</p>				|			|			|
 */

@interface MKLibrary : NSObject<UIAlertViewDelegate> {
	
}

///---------------------------------------------------------------------------------------
/// @name 업무앱 인증 + 무결성 확인
///---------------------------------------------------------------------------------------

/** 업무앱 인증과 앱의 무결성 검사를 함께 수행합니다.
 
 사용법 예제입니다.

	 - (void)someMethod {
		MKLibrary library = [[MKLibrary alloc] init];
		library.delgate = self;
 
		[library mangobanana:@"13090100"
					HostDomain:@"mdm.company.com:12345"
					  HostPort:@"44300"
						UserID:@"userID"
					  DeviceID:@"deviceID"];
	 }
 
	 - (void)mangobananaResult:(NSString *)code message:(NSString *)message {
		 NSLog(@"결과코드: %@", code);
		 NSLog(@"응답메시지: %@", message);
	 }

 @param companyCode 인증서버 코드
 @param domain 인증서버 도메인
 @param port 인증서버 포트번호
 @param userID 사용자 아이디
 @param deviceID Wi-Fi MAC 혹은 UDID
 
 @return `[MKLibraryDelegate mangobananaResult:message:]` 로 결과가 전달됩니다.
 
 @see MKLibraryDelegate
 @see [MKLibraryDelegate mangobananaResult:message:]
 */

- (void)mangobanana:(NSString *)companyCode domain:(NSString *)domain port:(NSString *)port userID:(NSString *)userID deviceID:(NSString*)deviceID;


///---------------------------------------------------------------------------------------
/// @name 앱 백그라운드 / 포그라운드 진입
///---------------------------------------------------------------------------------------

/** 앱이 백그라운드로 들어갔을 때 호출합니다.

 사용법 예제입니다.

 	- (void)someMethod {
        MKLibrary library = [[MKLibrary alloc] init];
        library.delgate = self;

 		[library appWillResignActive:@"회사코드"
								domain:@"인증서버 도메인"
								  port:@"인증서버 포트번호"
                              deviceID:@"Wi-Fi MAC 또는 UDID"];
    }

    - (void)appWillResignActiveResult:(NSString *)code message:(NSString *)message {
        NSLog(@"결과코드: %@", code);
        NSLog(@"응답메시지: %@", message);
    }
 

 @param companyCode 회사코드
 @param domain 인증서버 도메인
 @param port 인증서버 포트번호
 @param deviceID Wi-Fi MAC 또는 UDID

 @return `[MKLibraryDelegate appWillResignActiveResult:message:]` 로 결과가 전달됩니다.

 @see MKLibraryDelegate
 @see [MKLibraryDelegate appWillResignActiveResult:message:]
 */
- (void)appWillResignActive:(NSString *)companyCode domain:(NSString *)domain port:(NSString *)port deviceID:(NSString *)deviceID;


/** 앱이 포그라운드로 올라왔을 때 호출합니다.

 사용법 예제입니다.

    - (void)someMethod {
        MKLibrary library = [[MKLibrary alloc] init];
        library.delgate = self;

        [library appDidBecomeActive:@"회사코드"
 							 domain:@"인증서버 도메인"
 							   port:@"인증서버 포트"
                           deviceID:@"Wi-Fi MAC 또는 UDID"];
    }

	- (void)appWillResignActiveResult:(NSString *)code message:(NSString *)message {
		NSLog(@"결과코드: %@", code);
		NSLog(@"응답메시지: %@", message);
	}
 

 @param companyCode 회사코드
 @param domain 인증서버 도메인
 @param port 인증서버 포트번호
 @param deviceID Wi-Fi MAC 또는 UDID

 @return `[MKLibraryDelegate appDidBecomeActiveResult:message:]` 로 결과가 전달됩니다.

 @see MKLibraryDelegate
 @see [MKLibraryDelegate appDidBecomeActiveResult:message:]
 */
- (void)appDidBecomeActive:(NSString *)companyCode domain:(NSString *)domain port:(NSString *)port deviceID:(NSString *)deviceID;


///---------------------------------------------------------------------------------------
/// @name Library 버전 확인
///---------------------------------------------------------------------------------------

/** 현재 사용중인 앱 인증 Library의 버전을 확인합니다.
 
 사용법 예제입니다.
 
    NSString *libraryVersion = [MKLibrary Request_Get_libVersion];
 
 @return 버전 값.
 */
+ (NSString *) Request_Get_libVersion;


///---------------------------------------------------------------------------------------
/// @name Property
///---------------------------------------------------------------------------------------

/** 메서드 호출 결과를 받을 클래스를 지정합니다.
 
 사용법 예제입니다.
 
    MKLibrary *library = [[MKLibrary alloc] init];
    library.delgate = self;
 
 @see MKLibraryDelegate
 */
@property (nonatomic, assign) id<MKLibraryDelegate> delgate;

@end


/** 메서드 호출 결과를 반환합니다.
 
 다음 메서드를 호출한 후 결과를 리턴받습니다.
 
 - `appWillResignActive:domain:port:deviceID:`
 - `appDidBecomeActive:domain:port:deviceID:`
 - `mangobanana:domain:port:userID:deviceID:`
 */
@protocol MKLibraryDelegate<NSObject>;

@required

/** 앱이 백그라운드로 전환되었음을 알리는 호출에 대한 결과를 반환합니다.
 
 @param code 결과코드
 @param message 메시지
 
 @see [MKLibrary appWillResignActive:domain:port:deviceID:]
 */
- (void)appWillResignActiveResult:(NSString *)code message:(NSString *)message;

/** 앱이 포그라운드로 전환되었음을 알리는 호출에 대한 결과를 반환합니다.
 
 @param code 결과코드
 @param message 메시지
 
 @see [MKLibrary appDidBecomeActive:domain:port:deviceID:]
 */
- (void)appDidBecomeActiveResult:(NSString *)code message:(NSString *)message;

/** 앱인증, 무결성 검사 결과를 반환합니다.
 
 @param code 결과값
 @param message 메시지
 
 @see [MKLibrary mangobanana:domain:port:userID:deviceID:]
 */
- (void)mangobananaResult:(NSString *)code message:(NSString *)message;

@optional


@end
