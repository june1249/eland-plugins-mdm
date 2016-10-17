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

    }, function(error) {
    
    }, 'MDM', 'check_app' []);
```
