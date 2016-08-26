CALL RazerSDK_setup.cmd
CALL RazerSDK_android.mkb
IF EXIST lib\android\armv7\libRazerSDK.so COPY /Y lib\android\armv7\libRazerSDK.so lib\android\libRazerSDK.so
