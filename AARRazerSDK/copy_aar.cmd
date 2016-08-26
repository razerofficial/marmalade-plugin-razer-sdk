SET JAVA_HOME=C:\NVPACK\jdk1.7.0_71

SET JAR="%JAVA_HOME%\bin\jar.exe"

COPY /Y pluginrazersdk\libs\store-sdk-standard-release.aar ..\
COPY /Y pluginrazersdk\build\outputs\aar\pluginrazersdk-release.aar ..\

CD ..\

IF EXIST classes.jar DEL classes.JAR
CALL %JAR% -xvf store-sdk-standard-release.aar classes.jar
IF EXIST store-sdk-standard-release.jar DEL store-sdk-standard-release.jar
RENAME classes.jar store-sdk-standard-release.jar

IF EXIST classes.jar DEL classes.JAR
CALL %JAR% -xvf pluginrazersdk-release.aar classes.jar
IF EXIST pluginrazersdk-release.jar DEL pluginrazersdk-release.jar
RENAME classes.jar pluginrazersdk-release.jar

REM RazerSDK Extension

COPY /Y store-sdk-standard-release.aar MarmaladeRazerSDK\lib
COPY /Y store-sdk-standard-release.jar MarmaladeRazerSDK\lib

COPY /Y pluginrazersdk-release.aar MarmaladeRazerSDK\lib
COPY /Y pluginrazersdk-release.jar MarmaladeRazerSDK\lib

REM InAppPurchases

COPY /Y store-sdk-standard-release.aar InAppPurchases\lib\android
COPY /Y store-sdk-standard-release.jar InAppPurchases\lib\android

COPY /Y pluginrazersdk-release.aar InAppPurchases\lib\android
COPY /Y pluginrazersdk-release.jar InAppPurchases\lib\android

if NOT "%NO_PAUSE%"=="true" PAUSE
