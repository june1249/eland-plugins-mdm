//
//  MKLibrary.h
//  MKLibrary
//
//  Created by JuYoung Kim on 12. 6. 1..
//
//  Modified by vivifier on 13. 11. 15..
//  Copyright (c) 2013년 JiranSoft. All rights reserved.


#import <Foundation/Foundation.h>
#import <CoreFoundation/CoreFoundation.h>

// 결과에서 반환되는 Dictionary Key
#define Dictionary_Key_Result       @"Result"
#define Dictionary_Key_ErrorCode    @"ErrorCode"
#define Dictionary_Key_Message      @"Message"
#define Dictionary_Key_InOut        @"InOut"
#define Dictionary_Key_Hash         @"Hash"
#define Dictionary_Key_DateOfUpdate @"DateOfUpdate"
#define Dictionary_Key_DelayTime    @"DelayTime"
#define Dictionary_Key_Order        @"Order"

@protocol MKLibraryDelegate;

/**
 
 이 라이브러리는 MDM 구축을 위해 응용 어플리케이션에 제공하는 라이브러리로 
 응용 어플리케이션에서 앱의 위변조 체크, 루팅이나 탈옥여부 체크, Wi-Fi 사용 체크, 
 MDM과의 단말 인증을 이용하는 방법에 대해서 설명합니다.
 
 <br>
 
 
 # 지원 환경 #
 
 iOS 4.0 이상
 
 <br>
 
 
 # 파일 구성 #
 
 | 파일명         | 설명            |
 |----------------|-----------------|
 | libMKLibrary.a | 라이브러리 파일 |
 | MKLibrary.h    | 헤더 파일       |
 
 
 # 포함해야할 프레임워크 #
 
 라이브러리를 정상적으로 사용하기 위해 반드시 포함해야 할 프레임워크는 다음과 같습니다.
 
 - `SystemConfiguration.framework`
 - `UIKit.framework`              
 - `Foundation.framework`          
 - `libz.1.2.5.dylib`              
 
 <br>
 
 
 # iOS 7.0 이상 필수 확인사항 #
 
 iOS 7.0 이 나오면서 WifiMac을 더 이상 얻을 수 없도록 변경되었습니다. 
 라이브러리에서 더 이상 디바이스를 구분할 값을 추출 할 수 없으므로 파라미터로 입력받도록 변경되었습니다. 
 물론 iOS 7.0 이하 버전에서는 예전 방식대로 사용 가능합니다.
 
 이에 따라 iOS 7.0 이상 버전에서는 다음 메서드들은 반드시 새로 추가된 `deviceID` 파라미터에
 WifiMac 이나 UDID 값을 전달해서 호출해야 합니다.
 만일 iOS 7.0 이상에서 이전 버전의 메서드 호출 방식을 사용하면 
 오류코드를 리턴하도록 변경되었습니다.
  
 - `Request_Certification_Sync:apploginid:INOut:Company:deviceID:`
 - `Request_Certification:apploginid:INOut:Company:deviceID:`
 - `Request_ApplicationCheck_Sync:apploginid:INOut:Company:Hash:deviceID:`
 - `Request_ApplicationCheck:apploginid:INOut:Company:Hash:deviceID:`
 - `Send_ScreenCapture_Sync:Company:CaptureAllow:deviceID:`
 - `Send_ScreenCapture:Company:CaptureAllow:deviceID:`
 
 iOS 7.0 이상에서 `deviceID` 파라미터를 누락하거나 `nil`로 전송하면 다음 오류코드와 오류메시지가 돌아옵니다.
 
 | 오류코드   | 오류메시지                         |
 |------------|------------------------------------|
 | 0xD8000021 | 디바이스 ID 파라미터가 필요합니다. |
 
 
 # WifiMac이나 UDID를 얻는 방법 #
 
 WifiMac 이나 UDID는 UrlScheme으로 MDM 앱을 호출하여 응답받는 방법으로 얻을 수 있습니다.
 이 과정이 정상적으로 진행되려면 다음 사항을 충족해야합니다.
 
 - MDM 앱이 설치되어 있어야 합니다.
 - MDM 앱이 최신버전이어야 합니다. (앱 인증을 위한 UrlScheme이 포함된 버전)
 - MDM 앱이 정상적인 상태여야 합니다. (로그인 및 기기등록 과정이 정상적으로 완료된 상태. MDM 프로파일 또한 정상적인 상태)
 - 앱인증을 수행하는 앱도 WifiMac이나 UDID를 전달받을 수 있도록 UrlScheme이 존재해야 합니다.
 
 이 조건을 모두 만족한다면 `deviceID` 파라미터에 전달할 WifiMac 이나 UDID값을 MDM 앱으로부터 얻어올 수 있습니다.
 
 
 <br>
 
 # UrlScheme 등록 #
 
 UrlScheme은 Xcode의 TARGETS - Info 탭 - URL Types 항목에서 추가할 수 있습니다.
 
 이미 UrlScheme을 사용하고 있다면 Sub URL을 통해 WifiMac이나 UDID를 전달받아도 되고
 다른 이름의 UrlScheme을 하나 더 추가하여 전달받아도 됩니다.
 
 
 <br>
 
 # MDM 앱 UrlScheme 호출 #
 
 WifiMac이나 UDID를 얻기 위해 MDM 앱을 다음 방법으로 호출합니다.
 
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
 WifiMac을 얻으려면 `getudid` 대신 `getwifimac` 을 호출하면 됩니다.
 
    @"mobilekeeperinfo://getwifimac?delegatescheme=응답받을 UrlScheme"
 
 
 <br>
 
 # UIApplicationDelegate 의 application:openURL:sourceApplication:annotation: 구현 #
 
 MDM 앱에서 업무앱을 UrlScheme으로 다시 호출하면 UIApplicationDelegate의 openUrl 메서드가 호출됩니다.
 이 프로토콜을 구현하여 WifiMac이나 UDID를 추출합니다.
 
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
 
 MDM 앱이 비정상적일 경우 (기기등록에 실패했거나 MDM 프로파일이 삭제된 경우)에는 `@"(null)"` 을 리턴합니다.
 이 경우 다시한번 시도를 해보거나 MDM 앱의 설치상태를 확인해야 합니다.
 
 이런 일련의 과정을 거쳐 MDM 앱으로부터 WifiMac이나 UDID를 얻으면 
 앱의 저장 공간에 두고 계속 앱 인증시 마다 재사용해도 되고 매번 다시 얻어서 사용해도 되지만 
 화면전환이 일어나는 등의 불편함이 있으니 저장해 놓고 사용하는 것을 권장합니다.
 
 
 <br>
 
 # 개정이력 #
 
 | 버전 | 변경일     | 변경내용                                                               | 작성자 | 승인자 |
 |------|------------|------------------------------------------------------------------------|--------|--------|
 |  1   | 2012.06.07 | 최초작성                                                               | 김주영 | 염태진 |
 |  2   | 2012.06.26 | 앱 해시방법 변경 및 API 파라미터 변경                                  | 김주영 | 염태진 |
 |  3   | 2012.06.29 | 앱 해시 등록 및 검색 API 추가                                          | 김주영 | 염태진 |
 |  4   | 2012.07.02 | 앱 해시 시간 및 비동기 방식 추가                                       | 김주영 | 염태진 |
 |  5   | 2012.07.02 | Documents/res 해시 및 신한은행요청 추가                                | 김주영 | 염태진 |
 |  6   | 2012.07.19 | 앱 해시 추가 및 변경                                                   | 김주영 | 염태진 |
 |  7   | 2012.07.24 | 앱 해시 함수 변경 (추가경로)                                           | 김주영 | 염태진 |
 |  8   | 2012.09.19 | iOS 6 지원                                                             | 김주영 | 염태진 |
 |  9   | 2012.10.12 | 연결 Timeout 시간 20초로 수정                                          | 김주영 | 염태진 |
 | 10   | 2012.10.18 | Xcode 3 (SDK 4.2) 지원                                                 | 김주영 | 염태진 |
 | 11   | 2012.10.22 | 연결 Timeout 시간 30초로 수정                                          | 김주영 | 염태진 |
 | 12   | 2012.10.26 | 요청한 명령(함수)의 종류를 결과에 출력                                 | 김주영 | 염태진 |
 | 13   | 2012.10.31 | libz.1.2.5.dylib 라이브러리 추가                                       | 김주영 | 염태진 |
 | 14   | 2013.02.18 | 앱 인증요청에 앱 버전정보 추가                                         | 김주영 | 염태진 |
 | 15   | 2013.02.20 | API 2.1 버전에 최적화                                                  | 김주영 | 염태진 |
 | 16   | 2013.02.28 | 블루투스 활성화 확인 기능 추가                                         | 김주영 | 염태진 |
 | 17   | 2013.03.18 | Xcode 3 버전 기기/시뮬레이터 지원<br>시스템에 따라 3가지 파일로 구분   | 김주영 | 염태진 |
 | 18   | 2013.03.21 | 앱 인증요청 함수 오류 수정                                             | 김주영 | 염태진 |
 | 19   | 2013.03.22 | 버전정보 추출 변경                                                     | 김주영 | 염태진 |
 | 20   | 2013.04.02 | 블루투스 검색 방법 옵션 추가<br>블루투스 검색 비동기 함수 추가         | 김주영 | 염태진 |
 | 21   | 2013.08.21 | 등록된 해시 조회 함수 오류 수정<br>iOS 7 대응 파라미터 추가 (DeviceID) | 이영종 | 염태진 |
 | 22   | 2013.11.15 | 인증서버 주소에 포트를 함께 입력하도록 수정                            | 이영종 | 염태진 |
 | 23   | 2013.11.26 | 앱 관리자 확인 메서드 추가<br>해시 등록시 관리자 계정 입력하도록 수정  | 이영종 | 염태진 | 
 | 24   | 2014.08.07 | iOS 8 해시함수 오류 수정                                               | 이영종 | 염태진 |
 | 25   | 2014.08.13 | NSData 카테고리 제거 (TBXML)                                           | 이영종 | 염태진 | 
 
 */

@interface MkLibrary : NSObject {
    NSInteger myErrorcode;
    NSInteger bluetooth_Option;
}


///---------------------------------------------------------------------------------------
/// @name 탈옥(JailBreaking) 체크
///---------------------------------------------------------------------------------------

/** 디바이스의 탈옥(JailBreaking) 여부를 체크합니다.
 
 사용법 예제입니다.
 
    if ([MkLibrary Get_JailBroken]) {
        NSLog(@"탈옥된 기기입니다.");
    } else {
        NSLog(@"일반 기기입니다.");
    }
 
 @return 탈옥 여부.
 
 | 응답값 | 설명        |
 |--------|-------------|
 | `YES`  | 탈옥된 기기 |
 | `NO`   | 일반 기기   |

 */
+ (BOOL)Get_JailBroken;


///---------------------------------------------------------------------------------------
/// @name 와이파이(Wi-Fi) 사용여부 체크
///---------------------------------------------------------------------------------------

/** 와이파이(Wi-Fi)를 사용하고 있는지 체크합니다.
 
 사용법 예제입니다.
 
    NSString *connectedAp = [MkLibrary Get_WifiSSID];
 
    if (connectedAp) {
        NSLog(@"와이파이에 연결되어있으며 현재 연결된 AP는 %@입니다.", connectedAp);
    } else {
        NSLog(@"와이파이에 연결되어있지 않습니다.");
    }
 
 @return 연결된 AP의 SSID.
 
 | 응답값    | 설명                          |
 |-----------|-------------------------------|
 | `nil`     | 와이파이에 연결되어 있지 않음 |
 | NOT `nil` | 연결된 AP의 SSID              |

 */
+ (NSString *)Get_WifiSSID;


///---------------------------------------------------------------------------------------
/// @name 블루투스 활성화 유무 검색
///---------------------------------------------------------------------------------------

/** 블루투스가 활성화되었는지 확인합니다. (동기식)

 사용법 예제입니다.

    #define BLUETOOTH_ENABLED_OPTION 1

    int result = [MkLibrary Get_BluetoothEnable:self Selector:@selector(receivedBluetoothStatus) 
                                                       Option:BLUETOOTH_ENABLED_OPTION];

    switch (result) {
        case -2:
            NSLog(@"[M] 블루투스 활성화 여부를 확인할 수 없습니다.");
            break;

        case -1:
            NSLog(@"[M] 지금은 알 수 없습니다.");
            break;

        case 0:
            if (BLUETOOTH_ENABLED_OPTION == 0) {
                NSLog(@"[M] 블루투스가 비활성화 되어있습니다.");
            } else if (BLUETOOTH_ENABLED_OPTION == 1) {
                NSLog(@"[M] 블루투스가 비활성화 되어있거나 다른 기기와 연결되어 있지 않습니다.");
            }
            break;

        case 1:
            if (BLUETOOTH_ENABLED_OPTION == 0) {
                NSLog(@"[M] 블루투스가 활성화 되어있습니다.");
            } else if (BLUETOOTH_ENABLED_OPTION == 1) {
                NSLog(@"[M] 블루투스가 활성화 되어있고 다른 기기와 연결되어 있습니다.");
            }
            break;
    }

    // Selector 메서드 (블루투스 활성화 여부를 곧바로 알 수 없는 경우 준비가 되면 호출됨)
    - (void)receivedBluetoothStatus {

        int result = [MkLibrary Get_BluetoothEnable:self Selector:nil 
                                                           Option:BLUETOOTH_ENABLED_OPTION];

        switch (result) {
            case -2:
                NSLog(@"[S] 블루투스 활성화 여부를 확인할 수 없습니다.");
                break;

            case 0:
                if (BLUETOOTH_ENABLED_OPTION == 0) {
                    NSLog(@"[S] 블루투스가 비활성화 되어있습니다.");
                } else if (BLUETOOTH_ENABLED_OPTION == 1) {
                    NSLog(@"[S] 블루투스가 비활성화 되어있거나 다른 기기와 연결되어 있지 않습니다.");
                }
                break;

            case 1:
                if (BLUETOOTH_ENABLED_OPTION == 0) {
                    NSLog(@"[S] 블루투스가 활성화 되어있습니다.");
                } else if (BLUETOOTH_ENABLED_OPTION == 1) {
                    NSLog(@"[S] 블루투스가 활성화 되어있고 다른 기기와 연결되어 있습니다.");
                }
                break;
        }
    }

 실행 결과 예시입니다.

    [M] 블루투스가 활성화 되어있고 다른 기기와 연결되어 있습니다.
 

 @param observer 메서드를 호출한 객체
 @param selector 결과를 전달받을 메서드. 블루투스 활성화 여부를 곧바로 알 수 없는 경우 준비가 되면 호출됩니다.
 @param option 검색 방법

 - `0`: 설정에서 활성화 여부를 검색
 - `1`: 활성화 여부 및 다른 블루투스 기기와 연결되었는지 까지 확인

 @return 검색 결과 (_selector_ 파라미터로 전달된 메서드가 호출됨)

 | 결과값 | 설명                                                                                                                                            |
 |:------:|-------------------------------------------------------------------------------------------------------------------------------------------------|
 | `-2`   | 검색 실패 (OS에서 지원하지 않는 경우, _observer_ 파라미터가 `nil`인 경우, _option_ 파라미터가 `0` 또는 `1`이 아닌 경우)                         |
 | `-1`   | `Get_BluetoothEnable:Selector:Option:`을 한번 더 호출해야 함.                                                                                   |
 |  `0`   | _option_ = `0`: 설정에서 블루투스가 비활성화 되어있음<br>_option_ = `1`: 설정에서 블루투스가 비활성화 되어있거나 다른 기기와 연결되어 있지 않음 |
 |  `1`   | _option_ = `0`: 설정에서 블루투스가 활성화 되어있음<br>_option_ = `1`: 설정에서 블루투스가 활성화되어 있고 다른 기기와 연결되어 있음            |

 @see Get_BluetoothEnable:

 @warning 이 메서드는 Private API 를 활용한 기능으로 애플의 앱스토어 등록 시에 등록 거부 사유가 될 수 있습니다.
 */
+ (NSInteger)Get_BluetoothEnable:(id)observer Selector:(SEL)selector Option:(NSInteger)option;


/** 블루투스가 활성화되었는지 확인합니다. (비동기식)

 사용법 예제입니다.

    #define BLUETOOTH_ENABLED_OPTION 0

    - (void)someMethod
    {
        MkLibrary library = [[MkLibrary alloc] init];
        library.delgate = self;

        [library Get_BluetoothEnable:BLUETOOTH_ENABLED_OPTION];
    }

    - (void)Result_Bluetooth_Enable:(NSInteger)enable
    {
        switch (enable) {
            case -2:
                NSLog(@"블루투스 활성화 여부를 확인할 수 없습니다.");
                break;

            case 0:
                if (BLUETOOTH_ENABLED_OPTION == 0) {
                    NSLog(@"블루투스가 비활성화 되어있습니다.");
                } else if (BLUETOOTH_ENABLED_OPTION == 1) {
                    NSLog(@"블루투스가 비활성화 되어있거나 다른 기기와 연결되어 있지 않습니다.");
                }
                break;

            case 1:
                if (BLUETOOTH_ENABLED_OPTION == 0) {
                    NSLog(@"블루투스가 활성화 되어있습니다.");
                } else if (BLUETOOTH_ENABLED_OPTION == 1) {
                    NSLog(@"블루투스가 활성화 되어있고 다른 기기와 연결되어 있습니다.");
                }
                break;
        }
    }

 실행 결과 예시입니다.

    블루투스가 활성화 되어있습니다.


 @param option 검색 방법

 - `0`: 설정에서 활성화 여부를 검색
 - `1`: 활성화 여부 및 다른 블루투스 기기와 연결되었는지 까지 확인

 @return `[MKLibraryDelegate Result_Bluetooth_Enable:]` 로 결과가 전달됩니다.

 @see delgate
 @see [MKLibraryDelegate Result_Bluetooth_Enable:]
 @see Get_BluetoothEnable:Selector:Option:

 @warning 이 메서드는 Private API 를 활용한 기능으로 애플의 앱스토어 등록 시에 등록 거부 사유가 될 수 있습니다.
 */
- (void)Get_BluetoothEnable:(NSInteger)option;


///---------------------------------------------------------------------------------------
/// @name 앱 해시값 계산 (SHA256)
///---------------------------------------------------------------------------------------

/** 앱의 무결성 검사를 위해 해시값을 계산합니다. (동기식)

 사용법 예제입니다.

    NSMutableDictionary *result = [MkLibrary Get_Application_Hash_Sync:nil];

    NSLog(@"앱 해시 결과: %@", result);
    NSLog(@"해시에 걸린 시간: %@", [result valueForKey:@"DelayTime"]);
    NSLog(@"해시값: %@", [result valueForKey:@"Hash"]);

 실행 결과 예시입니다.

    앱 해시 결과: {
        DelayTime = "0.07690805196762085";
        Hash = 9fd56052b1857c9e8bbca46e4d83f858b313206399b91ed235f287e3b83f4029;
    }
    해시에 걸린 시간: 0.07690805196762085
    해시값: 9fd56052b1857c9e8bbca46e4d83f858b313206399b91ed235f287e3b83f4029

 @param subpath 추가 경로

 - `nil`: 앱 자체의 해시값만 계산
 - NOT `nil`: 추가로 해시값 계산에 포함할 리소스 폴더 경로.

 @result 계산 결과

 | Key         | Value                                       |
 |-------------|---------------------------------------------|
 | `Hash`      | 앱 해시 결과                                |
 | `DelayTime` | 앱 해시 계산에 걸린 시간 (단위: 초, Double) |

 @see Get_Application_Hash:
 @see Get_Application_Hash_Execution_Sync

 */
+ (NSMutableDictionary *)Get_Application_Hash_Sync:(NSString *)subpath;


/** 앱의 무결성 검사를 위해 해시값을 계산합니다. (비동기식)

 사용법 예제입니다.

    - (void)someMethod
    {
        MkLibrary library = [[MkLibrary alloc] init];
        library.delgate = self;

        NSArray *documentDir = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        [library Get_Application_Hash:[documentDir objectAtIndex:0]];
    }

    - (void)Result_Application_Hash:(NSDictionary *)appHash
    {
        NSLog(@"앱 해시 결과: %@", appHash);
        NSLog(@"해시에 걸린 시간: %@", [appHash valueForKey:@"DelayTime"]);
        NSLog(@"해시값: %@", [appHash valueForKey:@"Hash"]);
    }

 실행 결과 예시입니다.

    앱 해시 결과: {
        DelayTime = "0.07690805196762085";
        Hash = 9fd56052b1857c9e8bbca46e4d83f858b313206399b91ed235f287e3b83f4029;
    }
    해시에 걸린 시간: 0.07690805196762085
    해시값: 9fd56052b1857c9e8bbca46e4d83f858b313206399b91ed235f287e3b83f4029


 @param subpath 추가 경로

 - `nil`: 앱 자체의 해시값만 계산
 - NOT `nil`: 추가로 해시값 계산에 포함할 리소스 폴더 경로

 @result `[MKLibraryDelegate Result_Application_Hash:]` 로 결과가 전달됩니다.

 @see delgate
 @see [MKLibraryDelegate Result_Application_Hash:]
 @see Get_Application_Hash_Sync:
 @see Get_Application_Hash_Execution_Sync

 @warning 비동기식 해시 계산 메서드는 내부적으로 쓰레드에서 해쉬 계산을 하므로 `[MKLibraryDelegate Result_Application_Hash:]` 에서 UI업데이트를 시도할 경우 앱이 중지될 수 있습니다.

 */
- (void)Get_Application_Hash:(NSString *)subpath;


/** 앱의 무결성 검사를 위해 해시값을 계산합니다. (동기식, 앱 자체만)

 사용법 예제입니다.

    NSMutableDictionary *result = [MkLibrary Get_Application_Hash_Execution_Sync];

    NSLog(@"앱 해시 결과: %@", result);
    NSLog(@"해시에 걸린 시간: %@", [result valueForKey:@"DelayTime"]);
    NSLog(@"해시값: %@", [result valueForKey:@"Hash"]);

 실행 결과 예시입니다.

    앱 해시 결과: {
        DelayTime = "0.07690805196762085";
        Hash = 9fd56052b1857c9e8bbca46e4d83f858b313206399b91ed235f287e3b83f4029;
    }
    해시에 걸린 시간: 0.07690805196762085
    해시값: 9fd56052b1857c9e8bbca46e4d83f858b313206399b91ed235f287e3b83f4029

 @result 계산 결과

 | Key         | Value                                       |
 |-------------|---------------------------------------------|
 | `Hash`      | 앱 해시 결과                                |
 | `DelayTime` | 앱 해시 계산에 걸린 시간 (단위: 초, Double) |

 @see Get_Application_Hash_Sync:
 @see Get_Application_Hash:

 */
+ (NSMutableDictionary *)Get_Application_Hash_Execution_Sync;


///---------------------------------------------------------------------------------------
/// @name 업무앱 인증
///---------------------------------------------------------------------------------------

/** 업무 앱 인증을 수행합니다. (동기식)

 사용법 예제입니다.

     NSMutableDictionary *result = [MkLibrary Request_Certification_Sync:@"mdm.company.com:12345"
                                                              apploginid:@"userid"
                                                                   INOut:1
                                                                 Company:@"13090100"
                                                                deviceID:@"aa:bb:cc:dd:ee:ff"];

     NSLog(@"인증결과: %@", result);
     NSLog(@"결과: %@", [result valueForKey:@"Result"]);
     NSLog(@"에러코드: %@", [result valueForKey:@"ErrorCode"]);
     NSLog(@"응답메시지: %@", [result valueForKey:@"Message"]);

 실행 결과 예시입니다.

    인증결과: {
        ErrorCode = 0xD4210017;
        InOut = 1;
        Message = "MDM 앱이 업데이트되었습니다.";
        Order = Certification;
        Result = Fail;
    }
    결과: Fail
    에러코드: 0xD4210017
    응답메시지: MDM 앱이 업데이트되었습니다.
 
 @param host 인증서버 호스트 이름 (접속 포트도 지정)
 @param loginId 사용자 아이디
 @param appinout 앱 인증 시점
 
 - `1` (`In`): 앱이 실행되거나 로그인 할 때
 - `2` (`Out`): 앱이 종료되거나 백그라운드로 전환될 때
 
 @param code 회사코드
 @param deviceID (optional) WifiMac 혹은 UDID (iOS 7.0 이상에서는 필수)
 
 @return 인증 결과.
 
 | Key         | Value                                |
 |-------------|--------------------------------------|
 | `Result`    | `Success`: 성공<br> `Fail`: 실패     |
 | `ErrorCode` | 에러코드 (`0x00000000` 형태)         |
 | `Message`   | 응답 메시지                          |
 | `InOut`     | 메서드 호출시 입력했던 _appinout_ 값 |
 | `Order`     | `Certification`                      |
 
 @see Request_Certification:apploginid:INOut:Company:deviceID:
 
 @note _appinout_ 파라미터는 화면캡쳐 차단/해제를 위해 필요한 파라미터이지만 해당 메서드를 분리하면서 아무런 동작도 하지 않습니다.
 
 */

+ (NSMutableDictionary *)Request_Certification_Sync:(NSString *)host apploginid:(NSString *)loginId INOut:(NSInteger)appinout Company:(NSString *)code deviceID:(NSString *)deviceID;
+ (NSMutableDictionary *)Request_Certification_Sync:(NSString *)host apploginid:(NSString *)loginId INOut:(NSInteger)appinout Company:(NSString *)code;


/** 업무 앱 인증을 수행합니다. (비동기식)
 
 사용법 예제입니다.
 
    - (void)someMethod
    {
        MkLibrary library = [[MkLibrary alloc] init];
        library.delgate = self;

        [library Request_Certification:@"mdm.company.com:12345"
                            apploginid:@"userid"
                                 INOut:1
                               Company:@"13090100"
                              deviceID:@"aa:bb:cc:dd:ee:ff"];
    }
 
    - (void)RequestResult:(NSMutableDictionary *)result
    {
        NSLog(@"인증결과: %@", result);
        NSLog(@"결과: %@", [result valueForKey:@"Result"]);
        NSLog(@"에러코드: %@", [result valueForKey:@"ErrorCode"]);
        NSLog(@"응답메시지: %@", [result valueForKey:@"Message"]);
    }
 
 실행 결과 예시입니다.
 
    인증결과: {
        ErrorCode = 0xD4210017;
        InOut = 1;
        Message = "MDM 앱이 업데이트되었습니다.";
        Order = Certification;
        Result = Fail;
    }
    결과: Fail
    에러코드: 0xD4210017
    응답메시지: MDM 앱이 업데이트되었습니다.
 
 @param host 인증서버 호스트 이름 (접속 포트도 지정)
 @param loginId 사용자 아이디
 @param appinout 앱 인증 시점
 
 - `1` (`In`): 앱이 실행되거나 로그인 할 때
 - `2` (`Out`): 앱이 종료되거나 백그라운드로 전환될 때
 
 @param code 회사코드
 @param deviceID (optional) WifiMac 혹은 UDID (iOS 7.0 이상에서는 필수)
 
 @return `[MKLibraryDelegate RequestResult:]` 로 결과가 전달됩니다.
 
 @see delgate
 @see [MKLibraryDelegate RequestResult:]
 @see Request_Certification_Sync:apploginid:INOut:Company:deviceID: 
 
 */
- (void)Request_Certification:(NSString *)host apploginid:(NSString *)loginId INOut:(NSInteger)appinout Company:(NSString *)code deviceID:(NSString *)deviceID;
- (void)Request_Certification:(NSString *)host apploginid:(NSString *)loginId INOut:(NSInteger)appinout Company:(NSString *)code;


///---------------------------------------------------------------------------------------
/// @name 업무앱 인증 + 무결성 확인
///---------------------------------------------------------------------------------------

/** 업무앱 인증과 앱의 무결성 검사를 함께 수행합니다. (동기식)

 사용법 예제입니다.

    NSMutableDictionary *result = [MkLibrary Request_ApplicationCheck_Sync:@"mdm.company.com:12345"
                                                                                  apploginid:@"userid"
                                                                                       INOut:1
                                                                                     Company:@"13090100"
                                                                                        Hash:@"925efe829363f2efac2396cf05f55d2d77146489f67ad77f1fdc1813ea57905a"
                                                                                    deviceID:@"aa:bb:cc:dd:ee:ff"];
    NSLog(@"인증 및 무결성 검사 결과: %@", result);
    NSLog(@"결과: %@", [result valueForKey:@"Result"]);
    NSLog(@"에러코드: %@", [result valueForKey:@"ErrorCode"]);
    NSLog(@"응답메시지: %@", [result valueForKey:@"Message"]);

 실행 결과 예시입니다.

    인증 및 무결성 검사 결과: {
        ErrorCode = 0x00000000;
        InOut = 1;
        Message = Success;
        Order = ApplicationCheck;
        Result = Success;
    }
    결과: Success
    에러코드: 0x00000000
    응답메시지: Success

 @param host 인증서버 호스트 이름 (접속 포트도 지정)
 @param loginId 사용자 아이디
 @param appinout 무결성 검사여부

 - `1` (`In`): 무결성 검사를 수행합니다.
 - `2` (`Out`): 무결성 검사를 수행하지 않습니다.

 @param code 회사코드
 @param hash 앱 해시값

 - `nil`: 무결성 검사를 수행하지 않고 앱인증만 수행합니다.
 - NOT `nil`: 무결성 검사를 수행합니다. `Get_Application_Hash_Sync:` 나 `Get_Application_Hash:` 로 구한 해시값을 입력합니다.

 @param deviceID (optional) WifiMac 혹은 UDID (iOS 7.0 이상에서는 필수)

 @return 인증 및 검사 결과

 | Key         | Value                                |
 |-------------|--------------------------------------|
 | `Result`    | `Success`: 성공<br> `Fail`: 실패     |
 | `ErrorCode` | 에러코드 (`0x00000000` 형태)         |
 | `Message`   | 응답 메시지                          |
 | `InOut`     | 메서드 호출시 입력했던 _appinout_ 값 |
 | `Order`     | `ApplicationCheck`                   |

 @see Request_ApplicationCheck:apploginid:INOut:Company:Hash:deviceID:

 */
+ (NSMutableDictionary *)Request_ApplicationCheck_Sync:(NSString *)host apploginid:(NSString *)loginId INOut:(NSInteger)appinout Company:(NSString *)code Hash:(NSString *)hash deviceID:(NSString *)deviceID;
+ (NSMutableDictionary *)Request_ApplicationCheck_Sync:(NSString *)host apploginid:(NSString *)loginId INOut:(NSInteger)appinout Company:(NSString *)code Hash:(NSString *)hash;


/** 업무앱 인증과 앱의 무결성 검사를 함께 수행합니다. (비동기식)

 사용법 예제입니다.

    - (void)someMethod
    {
        MkLibrary library = [[MkLibrary alloc] init];
        library.delgate = self;

        [library Request_ApplicationCheck:@"mdm.company.com:12345"
                                                 apploginid:@"userid"
                                                      INOut:1
                                                    Company:@"13090100"
                                                       Hash:@"925efe829363f2efac2396cf05f55d2d77146489f67ad77f1fdc1813ea57905a"
                                                   deviceID:@"aa:bb:cc:dd:ee:ff"];
    }

    - (void)RequestResult:(NSMutableDictionary *)result
    {
        NSLog(@"인증 및 무결성 검사 결과: %@", result);
        NSLog(@"결과: %@", [result valueForKey:@"Result"]);
        NSLog(@"에러코드: %@", [result valueForKey:@"ErrorCode"]);
        NSLog(@"응답메시지: %@", [result valueForKey:@"Message"]);
    }

 실행 결과 예시입니다.

    인증 및 무결성 검사 결과: {
        ErrorCode = 0x00000000;
        InOut = 1;
        Message = Success;
        Order = ApplicationCheck;
        Result = Success;
    }
    결과: Success
    에러코드: 0x00000000
    응답메시지: Success

 @param host 인증서버 호스트 이름 (접속 포트도 지정)
 @param loginId 사용자 아이디
 @param appinout 무결성 검사여부

 - `1` (`In`): 무결성 검사를 수행합니다.
 - `2` (`Out`): 무결성 검사를 수행하지 않습니다.

 @param code 회사코드
 @param hash 앱 해시값

 - `nil`: 무결성 검사를 수행하지 않고 앱인증만 수행합니다.
 - NOT `nil`: 무결성 검사를 수행합니다. `Get_Application_Hash_Sync:` 나 `Get_Application_Hash:` 로 구한 해시값을 입력합니다.

 @param deviceID (optional) WifiMac 혹은 UDID (iOS 7.0 이상에서는 필수)

 @return `[MKLibraryDelegate RequestResult:]` 로 결과가 전달됩니다.

 @see delgate
 @see [MKLibraryDelegate RequestResult:]
 @see Request_ApplicationCheck_Sync:apploginid:INOut:Company:Hash:deviceID:

 */
- (void)Request_ApplicationCheck:(NSString *)host apploginid:(NSString *)loginId INOut:(NSInteger)appinout Company:(NSString *)code Hash:(NSString *)hash deviceID:(NSString *)deviceID;
- (void)Request_ApplicationCheck:(NSString *)host apploginid:(NSString *)loginId INOut:(NSInteger)appinout Company:(NSString *)code Hash:(NSString *)hash;


///---------------------------------------------------------------------------------------
/// @name 앱 해시 등록
///---------------------------------------------------------------------------------------

/** 앱 관리자 여부를 확인합니다. 로그인 계정이 앱 해쉬 등록 권한이 있는지를 확인합니다. (동기식)
 
 사용법 예제입니다.

    NSMutableDictionary *result = [MkLibrary isAppManagerAccountSync:@"mdm.company.com:12345"
                                                          apploginid:@"userid"
                                                             Company:@"13090100"];

    NSLog(@"확인결과: %@", result);
    NSLog(@"결과: %@", [result valueForKey:@"Result"]);
    NSLog(@"에러코드: %@", [result valueForKey:@"ErrorCode"]);
    NSLog(@"응답메시지: %@", [result valueForKey:@"Message"]);


 실행 결과 예시입니다.

    확인결과: {
        ErrorCode = 0x00000000;
        Message = Success;
        Order = "AppManagerCheck";
        Result = Success;
    }
    결과: Success
    에러코드: 0x00000000
    응답메시지: Success

 @param host 인증서버 호스트 이름 (접속 포트도 지정)
 @param loginId 사용자 아이디
 @param code 회사코드

 @return 확인 결과

 | Key            | Value                            |
 |----------------|----------------------------------|
 | `Result`       | `Success`: 성공<br> `Fail`: 실패 |
 | `ErrorCode`    | 에러코드 (`0x00000000` 형태)     |
 | `Message`      | 응답 메시지                      |
 | `Order`        | `AppManagerCheck`                |

 @see isAppManagerAccount:apploginid:Company:

 */
+ (NSMutableDictionary *)isAppManagerAccountSync:(NSString *)host apploginid:(NSString *)loginId Company:(NSString *)code;

/** 앱 관리자 여부를 확인합니다. 로그인 계정이 앱 해쉬 등록 권한이 있는지를 확인합니다. (비동기식)
 
 사용법 예제입니다.

    - (void)someMethod
    {
        MkLibrary library = [[MkLibrary alloc] init];
        library.delgate = self;

        [library isAppManagerAccount:@"mdm.company.com:12345"
                                            apploginid:@"userid"
                                               Company:@"13090100"];
    }

    - (void)RequestResult:(NSMutableDictionary *)result
    {
        NSLog(@"확인결과: %@", result);
        NSLog(@"결과: %@", [result valueForKey:@"Result"]);
        NSLog(@"에러코드: %@", [result valueForKey:@"ErrorCode"]);
        NSLog(@"응답메시지: %@", [result valueForKey:@"Message"]);
    }

 실행 결과 예시입니다.

    확인결과: {
        ErrorCode = 0x00000000;
        Message = Success;
        Order = "AppManagerCheck";
        Result = Success;
    }
    결과: Success
    에러코드: 0x00000000
    응답메시지: Success

 @param host 인증서버 호스트 이름 (접속 포트도 지정)
 @param loginId 사용자 아이디
 @param code 회사코드

 @return `[MKLibraryDelegate RequestResult:]` 로 결과가 전달됩니다.

 @see delgate
 @see [MKLibraryDelegate RequestResult:]
 @see isAppManagerAccountSync:apploginid:Company:

 */
- (void)isAppManagerAccount:(NSString *)host apploginid:(NSString *)loginId Company:(NSString *)code;


/** 앱 해시를 서버에 등록합니다. (동기식)

 사용법 예제입니다.

    NSMutableDictionary *result = [MkLibrary Request_Registration_AppHash_Sync:@"mdm.company.com:12345"
                                                                                         Company:@"13090100"
                                                                                            Hash:@"925efe829363f2efac2396cf05f55d2d77146489f67ad77f1fdc1813ea57905a"
                                                                                       managerId:"managerid"
                                                                                      managerPwd:"managerpassword"];

    NSLog(@"등록결과: %@", result);
    NSLog(@"결과: %@", [result valueForKey:@"Result"]);
    NSLog(@"에러코드: %@", [result valueForKey:@"ErrorCode"]);
    NSLog(@"응답메시지: %@", [result valueForKey:@"Message"]);
    NSLog(@"등록한 해시: %@", [result valueForKey:@"Hash"]);
    NSLog(@"등록된 시간: %@", [result valueForKey:@"DateOfUpdate"]);

 실행 결과 예시입니다.

    등록결과: {
        DateOfUpdate = "2013-09-03 11:44:56";
        ErrorCode = 0x00000000;
        Hash = 925efe829363f2efac2396cf05f55d2d77146489f67ad77f1fdc1813ea57905a;
        Message = Success;
        Order = "Registration_AppHash";
        Result = Success;
    }
    결과: Success
    에러코드: 0x00000000
    응답메시지: Success
    등록한 해시: 925efe829363f2efac2396cf05f55d2d77146489f67ad77f1fdc1813ea57905a
    등록된 시간: 2013-09-03 11:44:56

 @param host 인증서버 호스트 이름 (접속 포트도 지정)
 @param code 회사코드
 @param hash 앱 해시값
 @param managerId 앱 관리자 아이디
 @param managerPwd 앱 관리자 비밀번호

 @return 등록 결과

 | Key            | Value                            |
 |----------------|----------------------------------|
 | `Result`       | `Success`: 성공<br> `Fail`: 실패 |
 | `ErrorCode`    | 에러코드 (`0x00000000` 형태)     |
 | `Message`      | 응답 메시지                      |
 | `Hash`         | 등록한 해시값                    |
 | `DateOfUpdate` | 등록 또는 수정한 날짜            |
 | `Order`        | `Registration_AppHash`           |

 @see Request_Registration_AppHash:Company:Hash:managerId:managerPwd:

 */
+ (NSMutableDictionary *)Request_Registration_AppHash_Sync:(NSString *)host Company:(NSString *)code Hash:(NSString *)hash managerId:(NSString *)managerId managerPwd:(NSString *)managerPwd;
+ (NSMutableDictionary *)Request_Registration_AppHash_Sync:(NSString *)host Company:(NSString *)code Hash:(NSString *)hash;


/** 앱 해시를 서버에 등록합니다. (비동기식)

 사용법 예제입니다.

    - (void)someMethod
    {
        MkLibrary library = [[MkLibrary alloc] init];
        library.delgate = self;

        [library Request_Registration_AppHash:@"mdm.company.com:12345"
                                                        Company:@"13090100"
                                                           Hash:@"925efe829363f2efac2396cf05f55d2d77146489f67ad77f1fdc1813ea57905a"
                                                      managerId:"managerid"
                                                     managerPwd:"managerpassword"];
    }

    - (void)RequestResult:(NSMutableDictionary *)result
    {
        NSLog(@"등록결과: %@", result);
        NSLog(@"결과: %@", [result valueForKey:@"Result"]);
        NSLog(@"에러코드: %@", [result valueForKey:@"ErrorCode"]);
        NSLog(@"응답메시지: %@", [result valueForKey:@"Message"]);
        NSLog(@"등록한 해시: %@", [result valueForKey:@"Hash"]);
        NSLog(@"등록된 시간: %@", [result valueForKey:@"DateOfUpdate"]);
    }

 실행 결과 예시입니다.

    등록결과: {
        DateOfUpdate = "2013-09-03 11:48:48";
        ErrorCode = 0x00000000;
        Hash = 925efe829363f2efac2396cf05f55d2d77146489f67ad77f1fdc1813ea57905a;
        Message = Success;
        Order = "Registration_AppHash";
        Result = Success;
    }
    결과: Success
        에러코드: 0x00000000
    응답메시지: Success
    등록한 해시: 925efe829363f2efac2396cf05f55d2d77146489f67ad77f1fdc1813ea57905a
    등록된 시간: 2013-09-03 11:48:48

 @param host 인증서버 호스트 이름 (접속 포트도 지정)
 @param code 회사코드
 @param hash 앱 해시값
 @param managerId 앱 관리자 아이디
 @param managerPwd 앱 관리자 비밀번호

 @return `[MKLibraryDelegate RequestResult:]` 로 결과가 전달됩니다.

 @see delgate
 @see [MKLibraryDelegate RequestResult:]
 @see Request_Registration_AppHash_Sync:Company:Hash:managerId:managerPwd:

 */
- (void)Request_Registration_AppHash:(NSString *)host Company:(NSString *)code Hash:(NSString *)hash managerId:(NSString *)managerId managerPwd:(NSString *)managerPwd;
- (void)Request_Registration_AppHash:(NSString *)host Company:(NSString *)code Hash:(NSString *)hash;


///---------------------------------------------------------------------------------------
/// @name 등록된 해시 검색
///---------------------------------------------------------------------------------------

/** 서버에 등록된 앱 해시를 검색합니다. (동기식)

 사용법 예제입니다.

    NSMutableDictionary *result = [MkLibrary Request_Get_RegisteredAppHash_Sync:@"mdm.company.com:12345"
                                                                        Company:@"13090100"];
 
    NSLog(@"검색결과: %@", result);
    NSLog(@"결과: %@", [result valueForKey:@"Result"]);
    NSLog(@"에러코드: %@", [result valueForKey:@"ErrorCode"]);
    NSLog(@"응답메시지: %@", [result valueForKey:@"Message"]);
    NSLog(@"등록된 해시: %@", [result valueForKey:@"Hash"]);
    NSLog(@"등록된 시간: %@", [result valueForKey:@"DateOfUpdate"]);
 
 실행 결과 예시입니다.
 
    검색결과: {
        DateOfUpdate = "2013-09-03 11:48:48";
        ErrorCode = 0x00000000;
        Hash = 925efe829363f2efac2396cf05f55d2d77146489f67ad77f1fdc1813ea57905a;
        Message = Success;
        Order = "Get_RegisteredAppHash";
        Result = Success;
    }
    결과: Success
    에러코드: 0x00000000
    응답메시지: Success
    등록된 해시: 925efe829363f2efac2396cf05f55d2d77146489f67ad77f1fdc1813ea57905a
    등록된 시간: 2013-09-03 11:48:48
 
 @param host 인증서버 호스트 이름 (접속 포트도 지정)
 @param code 회사코드
 
 @return 검색 결과
 
 | Key            | Value                                                              |
 |----------------|--------------------------------------------------------------------|
 | `Result`       | `Success`: 성공<br> `Fail`: 실패                                   |
 | `ErrorCode`    | 에러코드 (`0x00000000` 형태)                                       |
 | `Message`      | 응답 메시지                                                        |
 | `Hash`         | 등록되어 있는 해시값 <br> 등록되어 있지 않으면 Key가 포함되지 않음 |
 | `DateOfUpdate` | 등록 또는 수정한 날짜                                              |
 | `Order`        | `Get_RegisteredAppHash`                                            |
 
 @see Request_Get_RegisteredAppHash:Company:
 
 */
+ (NSMutableDictionary *)Request_Get_RegisteredAppHash_Sync:(NSString *)host Company:(NSString *)code;


/** 서버에 등록된 앱 해시를 검색합니다. (비동기식)
 
 사용법 예제입니다.
 
    - (void)someMethod
    {
        MkLibrary library = [[MkLibrary alloc] init];
        library.delgate = self;
 
        [library Request_Get_RegisteredAppHash:@"mdm.company.com:12345"
                                       Company:@"13090100"];
    }
 
    - (void)RequestResult:(NSMutableDictionary *)result
    {
        NSLog(@"검색결과: %@", result);
        NSLog(@"결과: %@", [result valueForKey:@"Result"]);
        NSLog(@"에러코드: %@", [result valueForKey:@"ErrorCode"]);
        NSLog(@"응답메시지: %@", [result valueForKey:@"Message"]);
        NSLog(@"등록된 해시: %@", [result valueForKey:@"Hash"]);
        NSLog(@"등록된 시간: %@", [result valueForKey:@"DateOfUpdate"]);
    }
 
 실행 결과 예시입니다.
 
    검색결과: {
        DateOfUpdate = "2013-09-03 11:48:48";
        ErrorCode = 0x00000000;
        Hash = 925efe829363f2efac2396cf05f55d2d77146489f67ad77f1fdc1813ea57905a;
        Message = Success;
        Order = "Get_RegisteredAppHash";
        Result = Success;
    }
    결과: Success
    에러코드: 0x00000000
    응답메시지: Success
    등록된 해시: 925efe829363f2efac2396cf05f55d2d77146489f67ad77f1fdc1813ea57905a
    등록된 시간: 2013-09-03 11:48:48
 
 @param host 인증서버 호스트 이름 (접속 포트도 지정)
 @param code 회사코드
 
 @return `[MKLibraryDelegate RequestResult:]` 로 결과가 전달됩니다.
 
 @see delgate
 @see [MKLibraryDelegate RequestResult:]
 @see Request_Get_RegisteredAppHash_Sync:Company:
 
 */
- (void)Request_Get_RegisteredAppHash:(NSString *)host Company:(NSString *)code;


///---------------------------------------------------------------------------------------
/// @name 화면캡쳐 차단 / 해제
///---------------------------------------------------------------------------------------

/** 디바이스의 화면캡쳐 기능을 차단하거나 해제 요청을 합니다. (동기식)
 
 사용법 예제입니다.
 
    NSMutableDictionary *result = [MkLibrary Send_ScreenCapture_Sync:@"mdm.company.com:12345"
                                                             Company:@"13090100"
                                                        CaptureAllow:NO
                                                            deviceID:@"aa:bb:cc:dd:ee:ff"];
 
    NSLog(@"차단 결과: %@", result);
    NSLog(@"결과: %@", [result valueForKey:@"Result"]);
    NSLog(@"에러코드: %@", [result valueForKey:@"ErrorCode"]);
    NSLog(@"응답메시지: %@", [result valueForKey:@"Message"]);
 
 실행 결과 예시입니다.
 
    차단 결과: {
        ErrorCode = 0x00000000;
        Message = Success;
        Order = "Send_ScreenCapture";
        Result = Success;
    }
    결과: Success
    에러코드: 0x00000000
    응답메시지: Success
 
 @param host 인증서버 호스트 이름 (접속 포트도 지정)
 @param code 회사코드
 @param allow 차단/해제
 
 - `YES`: 화면캡쳐 차단 해제
 - `NO` : 화면캡쳐 차단
 
 @param deviceID (optional) WifiMac 혹은 UDID (iOS 7.0 이상에서는 필수)
 
 @return 차단/해제 요청 결과
 
 | Key            | Value                            |
 |----------------|----------------------------------|
 | `Result`       | `Success`: 성공<br> `Fail`: 실패 |
 | `ErrorCode`    | 에러코드 (`0x00000000` 형태)     |
 | `Message`      | 응답 메시지                      |
 | `Order`        | `Send_ScreenCapture`             |
 
 @see Send_ScreenCapture:Company:CaptureAllow:deviceID:
 
 */
+ (NSMutableDictionary *)Send_ScreenCapture_Sync:(NSString *)host Company:(NSString *)code CaptureAllow:(BOOL)allow deviceID:(NSString *)deviceID;
+ (NSMutableDictionary *)Send_ScreenCapture_Sync:(NSString *)host Company:(NSString *)code CaptureAllow:(BOOL)allow;


/** 디바이스의 화면캡쳐 기능을 차단하거나 해제 요청을 합니다. (비동기식)
 
 사용법 예제입니다.
 
    - (void)someMethod
    {
        MkLibrary library = [[MkLibrary alloc] init];
        library.delgate = self;
 
        [library Send_ScreenCapture:@"mdm.company.com:12345"
                            Company:@"13090100"
                       CaptureAllow:YES
                           deviceID:@"aa:bb:cc:dd:ee:ff"];    
    }
 
    - (void)RequestResult:(NSMutableDictionary *)result
    {
        NSLog(@"해제 결과: %@", result);
        NSLog(@"결과: %@", [result valueForKey:@"Result"]);
        NSLog(@"에러코드: %@", [result valueForKey:@"ErrorCode"]);
        NSLog(@"응답메시지: %@", [result valueForKey:@"Message"]);
    }
 
 실행 결과 예시입니다.
 
    해제 결과: {
        ErrorCode = 0x00000000;
        Message = Success;
        Order = "Send_ScreenCapture";
        Result = Success;
    }
    결과: Success
    에러코드: 0x00000000
    응답메시지: Success
 
 @param host 인증서버 호스트 이름 (접속 포트도 지정)
 @param code 회사코드
 @param allow 차단/해제
 
 - `YES`: 화면캡쳐 차단 해제
 - `NO` : 화면캡쳐 차단
 
 @param deviceID (optional) WifiMac 혹은 UDID (iOS 7.0 이상에서는 필수)
 
 @return `[MKLibraryDelegate RequestResult:]` 로 결과가 전달됩니다.
 
 @see delgate
 @see [MKLibraryDelegate RequestResult:]
 @see Send_ScreenCapture_Sync:Company:CaptureAllow:deviceID:
 
 */
- (void)Send_ScreenCapture:(NSString *)host Company:(NSString *)code CaptureAllow:(BOOL)allow deviceID:(NSString *)deviceID;
- (void)Send_ScreenCapture:(NSString *)host Company:(NSString *)code CaptureAllow:(BOOL)allow;


///---------------------------------------------------------------------------------------
/// @name Property
///---------------------------------------------------------------------------------------

/** 비동기 메서드 호출 결과를 받을 클래스를 지정합니다.
 
 사용법 예제입니다.
 
    MkLibrary *mklib = [[MkLibrary alloc] init]; 
    mklib.delgate = self;
 
 @see MKLibraryDelegate
 */
@property (nonatomic, assign) id<MKLibraryDelegate> delgate;

@end


/** 비동기 메서드 호출 결과를 반환합니다.
 
 다음 메서드를 호출한 후 결과를 리턴받습니다.
 
 - `Request_Certification:apploginid:INOut:Company:deviceID:`
 - `isAppManagerAccount:apploginid:Company:`
 - `Request_Registration_AppHash:Company:Hash:`
 - `Request_Get_RegisteredAppHash:Company:`
 - `Send_ScreenCapture:Company:CaptureAllow:deviceID:`
 - `Request_ApplicationCheck:apploginid:INOut:Company:Hash:deviceID:`
 - `Get_Application_Hash:`
 - `Get_BluetoothEnable:`
 
 */
@protocol MKLibraryDelegate<NSObject>;

@required

/** 앱인증, 무결성, 앱 해시 등록 및 검색, 화면캡쳐 차단 메서드의 호출 결과를 반환합니다.
 
 @param result 결과값
 
 | Key         | Value                                                                        |
 |-------------|------------------------------------------------------------------------------|
 | `Result`    | `Success`: 성공<br> `Fail`: 실패                                             |
 | `ErrorCode` | 에러코드 (`0x00000000` 형태)                                                 |
 | `Message`   | 응답 메시지                                                                  |
 | `InOut`     | 메서드 호출시 입력했던 _appinout_ 값<br> `Send_ScreenCapture` 의 경우 미포함 |
 | `Order`     | 요청 메서드 이름 (아래 표 참조)                                              |
 
 | Order - Value           | 호출한 함수                                                        |
 |-------------------------|--------------------------------------------------------------------|
 | `Registration_AppHash`  | `Request_Registration_AppHash:Company:Hash:managerId:managerPwd:`  |
 | `Get_RegisteredAppHash` | `Request_Get_RegisteredAppHash:Company:`                           |
 | `Send_ScreenCapture`    | `Send_ScreenCapture:Company:CaptureAllow:deviceID:`                |
 | `ApplicationCheck`      | `Request_ApplicationCheck:apploginid:INOut:Company:Hash:deviceID:` |
 | `Certification`         | `Request_Certification:apploginid:INOut:Company:deviceID:`         |
 | `AppManagerCheck`       | `isAppManagerAccount:apploginid:Company:`                          |
 
 @see [MkLibrary isAppManagerAccount:apploginid:Company:]
 @see [MkLibrary Request_Registration_AppHash:Company:Hash:managerId:managerPwd:]
 @see [MkLibrary Request_Get_RegisteredAppHash:Company:]                           
 @see [MkLibrary Send_ScreenCapture:Company:CaptureAllow:deviceID:]                
 @see [MkLibrary Request_ApplicationCheck:apploginid:INOut:Company:Hash:deviceID:] 
 @see [MkLibrary Request_Certification:apploginid:INOut:Company:deviceID:]

 
 */
- (void)RequestResult:(NSMutableDictionary *)result;


/** 앱 해시 결과를 반환합니다.
 
 @param result 결과값
 
 | Key         | Value                                       |
 |-------------|---------------------------------------------|
 | `Hash`      | 앱 해시 결과                                |
 | `DelayTime` | 앱 해시 계산에 걸린 시간 (단위: 초, Double) |
 
 @see [MkLibrary Get_Application_Hash:]
 
 @warning 이 메서드는 쓰레드에서 호출하므로 이 메서드 안에서 UI업데이트를 시도할 경우 앱이 중지될 수 있습니다.
 
 */
- (void)Result_Application_Hash:(NSMutableDictionary *)result;

@optional

/** 블루투스 활성화 여부 결과를 반환합니다.
 
 @param enable 결과
 
 | 결과값 | 설명                                                                                                                                            |
 |:------:|-------------------------------------------------------------------------------------------------------------------------------------------------|
 | `-2`   | 검색 실패 (OS에서 지원하지 않는 경우, _option_ 파라미터가 `0` 또는 `1`이 아닌 경우)                                                             |
 |  `0`   | _option_ = `0`: 설정에서 블루투스가 비활성화 되어있음<br>_option_ = `1`: 설정에서 블루투스가 비활성화 되어있거나 다른 기기와 연결되어 있지 않음 |
 |  `1`   | _option_ = `0`: 설정에서 블루투스가 활성화 되어있음<br>_option_ = `1`: 설정에서 블루투스가 활성화되어 있고 다른 기기와 연결되어 있음            |
 
 @see [MkLibrary Get_BluetoothEnable:]
 
 */
- (void)Result_Bluetooth_Enable:(NSInteger)enable;


@end
