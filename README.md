# Eland Plugin - MDM
MDM 플러그인은 Cordova Plugin으로 iOS와 Android를 지원하고 있다.

## 설치 방법
```
cordova plugin add https://github.com/june1249/eland-plugins-mdm.git
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
MDM 플러그인 호출 시 성공 콜백은 다음과 같다.
- 패키지명
- 로케일
- 결과 코드
- 결과 메세지
