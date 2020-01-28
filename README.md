# Eland Plugin - MDM
MDM 플러그인은 Cordova Plugin으로 iOS와 Android를 지원하고 있다.

## 설치 방법
```
cordova plugin add https://github.com/lodosswkor/eland-plugins-mdm.git
```

## 사용방법
* Web
```
cordova.exec(function(winParam) {
    //성공 시
}, function(error) {
   // 실패 시
}, 'MDM', 'check_app' []);
```
### 결과 값
MDM 플러그인 성공 콜백은 다음과 같다.
- 패키지명 (**packageName**)
- 로케일 (**locale**)
- 결과 코드 (**codeKey**)
- 결과 메세지 (**message**)

#### 결과코드, 결과메세지
|결과코드|결과메세지|
|:-:|---|
|0x00000000|loginID|
|0x50000000|loginID|
|0xC3000005|권한이 없습니다.|
|0xD000000E|E-Mail 인증중입니다.|
|0xD4210014|ElandApps가 설치되어 있지 않습니다.|
|0xD4210017|ElandApps가 업데이트 되었습니다.|
|0xDB310023|앱이 업데이트되었습니다.|
