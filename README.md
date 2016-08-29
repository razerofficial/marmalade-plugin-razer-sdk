# Marmalade Game Engine

The [RazerSDK](https://github.com/razerofficial/razer-sdk-docs) can be accessed via the `RazerSDK Marmalade Extension`.

## Forums #

* [Forge TV on Razer Forums](https://insider.razerzone.com/index.php?forums/razer-forge-tv.126/)

* [Marmalade Community Answers](https://answers.madewithmarmalade.com/)

### Resources

* Download [Marmalade](https://www.madewithmarmalade.com)

* Download [NVIDIA Codeworks for Android](https://developer.nvidia.com/AndroidWorks)

* Learn [Marmalade](http://developer.madewithmarmalade.com/learn)

* Marmalade [Docs](http://docs.madewithmarmalade.com/)

* Docs: [Project Options](http://docs.madewithmarmalade.com/display/MD/MKB+build+and+project+options)

* Docs: [Resource Management System](http://www.drmop.com/index.php/2011/10/01/marmalade-sdk-tutorial-marmalades-resource-management-system/)

* Docs: [Handling touch events](http://docs.madewithmarmalade.com/display/MD/Detecting+touch,+key+and+accelerometer+inputs)
 
* Docs: [Drawing text](http://docs.madewithmarmalade.com/display/MD/Drawing+text)
 
## Quick Start

* [Buy Marmalade License](https://www.madewithmarmalade.com/shop)

* [Get Marmalade License Key](https://www.madewithmarmalade.com/my-account/licences)

* [Download and install Marmalade](https://www.madewithmarmalade.com/developer)

* Register "Indie" license in the Marmalade HUB

```
C:\Marmalade\8.5\tools\hub2\hub2.exe
```

* Run as administrator:

```
C:\Marmalade\8.5\s3e\bin\s3eConfig.exe
```

* Reboot to have settings take effect.

**Note: Marmalade works best with JDK version 7.**

* Download and install [JDK7](http://www.oracle.com/technetwork/java/javase/downloads/jdk7-downloads-1880260.html)

## Examples 

### In-App-Purchase Example

The [InAppPurchases](https://github.com/razerofficial/marmalade-plugin-razer-sdk/tree/master/InAppPurchases) sample is a `Marmalade` built and tested with `Visual Studio 2015`.

![image_1](image-md/image_1.png)

* Right-click [InAppPurchases.mkb](https://github.com/razerofficial/marmalade-plugin-razer-sdk/blob/master/InAppPurchases/InAppPurchases.mkb) and select `Open with Visual Studio 2015`

(In Visual Studio)

* Switch the `Solution Configuration` to `GCC Arm Debug` or `GCC Arm Release` and build the project

* Build and run the `GCC AARCH64 Debug` which will launch the `Marmalade System Deployment Tool`

(In Marmalade System Deployment Tool)

* Choose `ARM GCC Debug` and click `Next Stage`. Note: Building the `GCC Arm Debug` before launching the deploy tool will make the option available. Close, build, and relaunch the build tool if the option isn't available.
 
* Choose `Android` and click `Next Stage`.
 
* Choose `Android x86+ARM multi-build` and click `Next Stage`.
 
* Select the `Package` option and click `Deploy All`.

(In Explorer)

* Run the [InAppPurchases\debug_install.cmd](https://github.com/razerofficial/marmalade-plugin-razer-sdk/blob/master/InAppPurchases/debug_install.cmd) script.

## Razer SDK

The `RazerSDK` can be accessed using the [Marmalade RazerSDK Extension](https://github.com/razerofficial/marmalade-plugin-razer-sdk/tree/master/MarmaladeRazerSDK) which provides access to the `C++ API`.

### Marmalade RazerSDK Extension

The Marmalade RazerSDK Extension is a native wrapper around the RazerSDK which makes the Java library accessible to Marmalade application code.

#### Building the Marmalade Extension

To build the extension from source switch to the extension folder [Marmalade RazerSDK Extension](https://github.com/razerofficial/marmalade-plugin-razer-sdk/tree/master/MarmaladeRazerSDK).

##### Windows

In Windows explorer, right-click [RazerSDK.s4e](https://github.com/razerofficial/marmalade-plugin-razer-sdk/blob/master/MarmaladeRazerSDK/RazerSDK.s4e) and click 'Build Android Extension' in the context menu.

Double-click [RazerSDK_android_java.mkb](https://github.com/razerofficial/marmalade-plugin-razer-sdk/blob/master/MarmaladeRazerSDK/RazerSDK_android_java.mkb) to build the Java source.

Double-click [RazerSDK_android.mkb](https://github.com/razerofficial/marmalade-plugin-razer-sdk/blob/master/MarmaladeRazerSDK/RazerSDK_android.mkb) to build the Native source.

### Using the Marmalade RazerSDK Extension in an Application

Files with the MKB extension define the build and deploy behaviour for Marmalade applications.

Within your Marmalade [application's MKB](https://github.com/razerofficial/marmalade-plugin-razer-sdk/blob/master/InAppPurchases/InAppPurchases.mkb) file, add a subproject for the Marmalade RazerSDK extension.

To reference your local build of the application, specify the relative path the to the extension.

```
subprojects
{
	../MarmaladeRazerSDK/RazerSDK
}
```

Your application needs to include the Marmalade RazerSDK header to interface with the extension.

```
#include "RazerSDK.h"
```

#### Initialization

Before calling the Marmalade RazerSDK extension, an application [main](https://github.com/razerofficial/marmalade-plugin-razer-sdk/blob/master/InAppPurchases/Main.cpp) should check if the extension is available.

```
	if (!RazerSDKAvailable())
	{
		IwTrace(DEFAULT, ("RazerSDK is not available, exit!"));
		return 0;
	}
```

If the Marmalade RazerSDK extension is available, [initialize](https://github.com/razerofficial/marmalade-plugin-razer-sdk/blob/master/InAppPurchases/Application.cpp) with the `Secret API Key` found in the [developer portal](http://devs.ouya.tv).

c++
```
void Application::InitPlugin()
{
	Application::s_ui.SetMessage("Initialize RazerSDK Plugin...");

	std::string secretApiKey = "eyJkZXZlbG9wZXJfaWQiOiIzMTBhOGY1MS00ZDZlLTRhZTUtYmRhMC1iOTM4";
	secretApiKey += "NzhlNWY1ZDAiLCJkZXZlbG9wZXJfcHVibGljX2tleSI6Ik1JR2ZNQTBHQ1Nx";
	secretApiKey += "R1NJYjNEUUVCQVFVQUE0R05BRENCaVFLQmdRQ3A2VENMYzg1cHBGdnY5U1RU";
	secretApiKey += "Q3pYYk9aYVlDczFDNEdrcHNQMEQwaGs4dkoyZ1pqVjJhNUUwMHVTeVBYN1FO";
	secretApiKey += "cytWMlJIUTVLYkRJYUk3eFArS2JsdkZQK2FzZmdNM0M1aFR1c1F6TUxpVmFX";
	secretApiKey += "R1FVYzBFamF5bzNKZGdSN3lVSE9oWkhMU3dCZ2ZGckxVMU5za3pVdzVFQ3ZU";
	secretApiKey += "TGRDSk85bjdDNHhCcnBkZ25LUUlEQVFBQiJ9";

	Plugin_initPlugin(secretApiKey.c_str(),
		Application::s_ui.m_callbacksInitPlugin->GetSuccessEvent(),
		Application::s_ui.m_callbacksInitPlugin->GetFailureEvent());
}
```
