IF EXIST lib/android/pluginrazersdk-release.aar DEL lib\android\pluginrazersdk-release.aar
IF EXIST lib/android/pluginrazersdk-release.jar DEL lib\android\pluginrazersdk-release.jar
IF EXIST lib/android/store-sdk-standard-release.aar DEL lib\android\store-sdk-standard-release.aar
IF EXIST lib/android/store-sdk-standard-release.jar DEL lib\android\store-sdk-standard-release.jar

COPY ..\pluginrazersdk-release.aar lib\android
COPY ..\pluginrazersdk-release.jar lib\android
COPY ..\store-sdk-standard-release.aar lib\android
COPY ..\store-sdk-standard-release.jar lib\android

CALL RazerSDK_setup.cmd
CALL RazerSDK_android_java.mkb
