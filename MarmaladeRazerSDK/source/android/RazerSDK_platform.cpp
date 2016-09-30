/*
 * Copyright (C) 2012-2016 Razer, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * android-specific implementation of the RazerSDK extension.
 * Add any platform-specific functionality here.
 */
/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */
#include "RazerSDK_internal.h"
#include "RazerSDK_CallbackSingleton.h"
#include "RazerSDK_CallbacksInitPlugin.h"
#include "RazerSDK_CallbacksRequestLogin.h"
#include "RazerSDK_CallbacksRequestGamerInfo.h"
#include "RazerSDK_CallbacksRequestProducts.h"
#include "RazerSDK_CallbacksRequestPurchase.h"
#include "RazerSDK_CallbacksRequestReceipts.h"
#include "RazerSDK_CallbacksShutdown.h"
#include "RazerSDK_JSONArray.h"
#include "RazerSDK_JSONObject.h"
#include "RazerSDK_Controller.h"
#include "RazerSDK_Plugin.h"

#include <map>

#include "s3eEdk.h"
#include "s3eEdk_android.h"
#include <jni.h>
#include "IwDebug.h"

#include <android/log.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "RazerSDK_platform"

#ifdef ENABLE_VERBOSE_LOGGING
#undef ENABLE_VERBOSE_LOGGING
#endif
#define ENABLE_VERBOSE_LOGGING false

using namespace com_razerzone_store_sdk_Controller;
using namespace org_json_JSONObject;
using namespace org_json_JSONArray;
using namespace RazerSDK;
using namespace std;

// use string to send char* to invoker
static string g_tempPluginString;

#define MAX_CONTROLLERS 4

//axis states
static vector< map<int, float> > g_axis;

//button states
static vector< map<int, bool> > g_button;
static vector< map<int, bool> > g_buttonDown;
static vector< map<int, bool> > g_buttonUp;
static vector< map<int, bool> > g_lastButtonDown;
static vector< map<int, bool> > g_lastButtonUp;

static CallbacksInitPlugin g_callbacksInitPlugin;
static CallbacksRequestLogin g_callbacksRequestLogin;
static CallbacksRequestGamerInfo g_callbacksRequestGamerInfo;
static CallbacksRequestProducts g_callbacksRequestProducts;
static CallbacksRequestPurchase g_callbacksRequestPurchase;
static CallbacksRequestReceipts g_callbacksRequestReceipts;
static CallbacksShutdown g_callbacksShutdown;

void dispatchGenericMotionEventNative(JNIEnv* env, jobject thiz,
	jint deviceId,
	jint axis,
	jfloat val)
{
#if ENABLE_VERBOSE_LOGGING
	__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "dispatchGenericMotionEventNative: Device=%d axis=%d val=%f", deviceId, axis, val);
#endif
	if (deviceId < 0 ||
		deviceId >= MAX_CONTROLLERS)
	{
		deviceId = 0;
	}
	g_axis[deviceId][axis] = val;
}

void dispatchKeyEventNative(JNIEnv* env, jobject thiz,
	jint deviceId,
	jint keyCode,
	jint action)
{
#if ENABLE_VERBOSE_LOGGING
	__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "dispatchKeyEventNative: Device=%d KeyCode=%d Action=%d", deviceId, keyCode, action);
#endif
	if (deviceId < 0 ||
		deviceId >= MAX_CONTROLLERS)
	{
		deviceId = 0;
	}

	bool buttonDown = action == 0;

	if (g_button[deviceId][keyCode] != buttonDown)
	{
		g_button[deviceId][keyCode] = buttonDown;
		if (buttonDown)
		{
			g_buttonDown[deviceId][keyCode] = true;
		}
		else
		{
			g_buttonUp[deviceId][keyCode] = true;
		}
	}
}

static JNINativeMethod method_table[] = {
	{ "dispatchGenericMotionEventNative", "(IIF)V", (void *)dispatchGenericMotionEventNative }
};

static int method_table_size = sizeof(method_table) / sizeof(method_table[0]);

static JNINativeMethod method_table2[] = {
	{ "dispatchKeyEventNative", "(III)V", (void *)dispatchKeyEventNative }
};

static int method_table_size2 = sizeof(method_table2) / sizeof(method_table2[0]);

s3eResult RazerSDKInit_platform()
{
	__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Marmalade Plugin Native Version: 0.0.1");
	IwTrace(RazerSDK, ("Marmalade Plugin Native Version: 0.0.1"));


    // Get the environment from the pointer
    JNIEnv* env = s3eEdkJNIGetEnv();

	for (int index = 0; index < MAX_CONTROLLERS; ++index)
	{
		g_axis.push_back(map<int, float>());
		g_button.push_back(map<int, bool>());
		g_buttonDown.push_back(map<int, bool>());
		g_buttonUp.push_back(map<int, bool>());
		g_lastButtonDown.push_back(map<int, bool>());
		g_lastButtonUp.push_back(map<int, bool>());
	}

	__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Find InputView...");
	jclass clazz = env->FindClass("com/razerzone/store/sdk/engine/marmalade/InputView");
	if (clazz)
	{
		jint ret = env->RegisterNatives(clazz, method_table, method_table_size);
		ret = env->RegisterNatives(clazz, method_table2, method_table_size2);

		const char* strField = "sNativeInitialized";
		jfieldID fieldNativeInitialized = env->GetStaticFieldID(clazz, strField, "Z");
		if (fieldNativeInitialized)
		{
			env->SetStaticBooleanField(clazz, fieldNativeInitialized, true);
			env->DeleteLocalRef(clazz);
			__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Loaded Native Plugin: 003");
		}
		else
		{
			IwTrace(RazerSDK, ("Failed to find sNativeInitialized"));
			goto fail;
		}
	}
	else
	{
		IwTrace(RazerSDK, ("Failed to find InputView"));
		goto fail;
	}

	if (JSONArray::InitJNI() == JNI_ERR)
	{
		goto fail;
	}

	if (JSONObject::InitJNI() == JNI_ERR)
	{
		goto fail;
	}

	if (Controller::InitJNI() == JNI_ERR)
	{
		goto fail;
	}
	
	if (Plugin::InitJNI() == JNI_ERR)
	{
		goto fail;
	}

    // Add any platform-specific initialization code here
    return S3E_RESULT_SUCCESS;

fail:
    jthrowable exc = env->ExceptionOccurred();
    if (exc)
    {
        env->ExceptionDescribe();
        env->ExceptionClear();
        IwTrace(RazerSDK, ("One or more java methods could not be found"));
    }
    return S3E_RESULT_ERROR;

}

void RazerSDKTerminate_platform()
{
    // Add any platform-specific termination code here
}

// get axis value, cast float to int for the application bridge
int Plugin_getAxis(int deviceId, int axis)
{
	if (deviceId < 0 ||
		deviceId >= MAX_CONTROLLERS)
	{
		deviceId = 0;
	}

	map<int, float>::const_iterator search = g_axis[deviceId].find(axis);
	float val = 0.0f;
	if (search != g_axis[deviceId].end())
	{
		val = search->second;
	}
	//__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Plugin_getAxis: Device=%d axis=%d val=%f", deviceId, axis, val);
	return *(reinterpret_cast<int*>(&val));
}

// check if a button is pressed
bool Plugin_isPressed(int deviceId, int keyCode)
{
	if (deviceId < 0 ||
		deviceId >= MAX_CONTROLLERS)
	{
		deviceId = 0;
	}

	map<int, bool>::const_iterator search = g_button[deviceId].find(keyCode);
	bool val = false;
	if (search != g_button[deviceId].end())
	{
		val = search->second;
	}
	//__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Plugin_isPressed: Device=%d KeyCode=%d Action=%d", deviceId, keyCode, val);
	return val;
}

// check if a button was down
bool Plugin_isPressedDown(int deviceId, int keyCode)
{
	if (deviceId < 0 ||
		deviceId >= MAX_CONTROLLERS)
	{
		deviceId = 0;
	}

	map<int, bool>::const_iterator search = g_lastButtonDown[deviceId].find(keyCode);
	if (search != g_lastButtonDown[deviceId].end())
	{
		return search->second;
	}
	return false;
}

// check if a button was up
bool Plugin_isPressedUp(int deviceId, int keyCode)
{
	if (deviceId < 0 ||
		deviceId >= MAX_CONTROLLERS)
	{
		deviceId = 0;
	}

	map<int, bool>::const_iterator search = g_lastButtonUp[deviceId].find(keyCode);
	if (search != g_lastButtonUp[deviceId].end())
	{
		return search->second;
	}
	return false;
}

// clear the button state for detecting up and down
void Plugin_clearButtonStates()
{
	for (int deviceId = 0; deviceId < MAX_CONTROLLERS; ++deviceId)
	{
		g_lastButtonDown[deviceId].clear();
		g_lastButtonUp[deviceId].clear();
		for (map<int, bool>::iterator it = g_buttonDown[deviceId].begin(); it != g_buttonDown[deviceId].end(); ++it)
		{
			int keyCode = it->first;
			g_lastButtonDown[deviceId][keyCode] = g_buttonDown[deviceId][keyCode];
		}
		for (map<int, bool>::iterator it = g_buttonUp[deviceId].begin(); it != g_buttonUp[deviceId].end(); ++it)
		{
			int keyCode = it->first;
			g_lastButtonUp[deviceId][keyCode] = g_buttonUp[deviceId][keyCode];
		}
		g_buttonDown[deviceId].clear();
		g_buttonUp[deviceId].clear();
	}
}

const char* Plugin_getDeviceName(int playerNum)
{
	Controller* controller = Controller::getControllerByPlayer(playerNum);
	if (NULL != controller)
	{
		g_tempPluginString = controller->getDeviceName();
		controller->Dispose();
	}
	else
	{
		g_tempPluginString = "Unavailable";
	}
	return g_tempPluginString.c_str();
}

void Plugin_initPlugin(const char* secretApiKey, s3eCallback onSuccess, s3eCallback onFailure)
{
	IwTrace(RazerSDK, ("Plugin_initPlugin"));

	g_callbacksInitPlugin.RegisterCallbacks(onSuccess, onFailure);
	
	Plugin::initPlugin(secretApiKey, &g_callbacksInitPlugin);
}

void Plugin_requestLogin(s3eCallback onSuccess, s3eCallback onFailure, s3eCallback onCancel)
{
	IwTrace(RazerSDK, ("RazerSDK_platform: Plugin_requestLogin"));

	g_callbacksRequestLogin.RegisterCallbacks(onSuccess, onFailure, onCancel);

	Plugin::requestLogin(&g_callbacksRequestLogin);
}

void Plugin_requestGamerInfo(s3eCallback onSuccess, s3eCallback onFailure, s3eCallback onCancel)
{
	IwTrace(RazerSDK, ("RazerSDK_platform: Plugin_requestGamerInfo"));

	g_callbacksRequestGamerInfo.RegisterCallbacks(onSuccess, onFailure, onCancel);

	Plugin::requestGamerInfo(&g_callbacksRequestGamerInfo);
}

void Plugin_requestProducts(const char* productsJson, s3eCallback onSuccess, s3eCallback onFailure, s3eCallback onCancel)
{
	IwTrace(RazerSDK, ("RazerSDK_platform: Plugin_requestProducts"));

	string msg = "Plugin_requestProducts: productsJson=";
	msg.append(productsJson);
	IwTrace(RazerSDK, (msg.c_str()));

	//convert JSON to product id array
	JSONArray jsonArray = JSONArray(productsJson);

	vector<string> productIds;

	for (int i = 0; i < jsonArray.length(); i++)
	{
		string productId = jsonArray.getString(i);
		productIds.push_back(productId);
	}

	g_callbacksRequestProducts.RegisterCallbacks(onSuccess, onFailure, onCancel);

	Plugin::requestProducts(productIds, &g_callbacksRequestProducts);
}

void Plugin_requestPurchase(const char* purchasable, const char* productType, s3eCallback onSuccess, s3eCallback onFailure, s3eCallback onCancel)
{
	IwTrace(RazerSDK, ("RazerSDK_platform: Plugin_requestPurchase"));

	string msg = "Plugin_requestPurchase: purchasable=";
	msg.append(purchasable);
	IwTrace(RazerSDK, (msg.c_str()));

	g_callbacksRequestPurchase.RegisterCallbacks(onSuccess, onFailure, onCancel);

	Plugin::requestPurchase(purchasable, productType, &g_callbacksRequestPurchase);
}

void Plugin_requestReceipts(s3eCallback onSuccess, s3eCallback onFailure, s3eCallback onCancel)
{
	IwTrace(RazerSDK, ("RazerSDK_platform: Plugin_requestReceipts"));

	g_callbacksRequestReceipts.RegisterCallbacks(onSuccess, onFailure, onCancel);

	Plugin::requestReceipts(&g_callbacksRequestReceipts);
}

void Plugin_shutdown(s3eCallback onSuccess, s3eCallback onFailure, s3eCallback onCancel)
{
	IwTrace(RazerSDK, ("RazerSDK_platform: Plugin_shutdown"));

	g_callbacksShutdown.RegisterCallbacks(onSuccess, onFailure, onCancel);

	Plugin::shutdown(&g_callbacksShutdown);
}

static int g_refCountJSONObject = 0;
static map<int, JSONObject*> g_refJSONObject = map<int, JSONObject*>();

int Plugin_JSONObject_Construct()
{
	JSONObject* jsonObject = new JSONObject();
	g_refJSONObject[g_refCountJSONObject] = jsonObject;
	int result = g_refCountJSONObject;
	++g_refCountJSONObject;
	return result;
}

void Plugin_JSONObject_Put(int jsonObject, const char* name, const char* value)
{
#if ENABLE_VERBOSE_LOGGING
	__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Plugin_JSONObject_Put: jsonObject=%d", jsonObject);
#endif
	map<int, JSONObject*>::const_iterator search = g_refJSONObject.find(jsonObject);
	if (search != g_refJSONObject.end())
	{
		JSONObject* instance = search->second;
		if (instance)
		{
#if ENABLE_VERBOSE_LOGGING
			__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Plugin_JSONObject_Put JSONObject reference is valid");
#endif
			instance->put(name, value);
		}
		else
		{
			__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Plugin_JSONObject_Put JSONObject reference is invalid");
		}
	}
	else
	{
		__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Plugin_JSONObject_Put failed to find JSONObject reference");
	}
}

const char* Plugin_JSONObject_ToString(int jsonObject)
{
#if ENABLE_VERBOSE_LOGGING
	__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Plugin_JSONObject_ToString: jsonObject=%d", jsonObject);
#endif
	map<int, JSONObject*>::const_iterator search = g_refJSONObject.find(jsonObject);
	if (search != g_refJSONObject.end())
	{
		JSONObject* instance = search->second;
		if (instance)
		{
#if ENABLE_VERBOSE_LOGGING
			__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Plugin_JSONObject_ToString JSONObject reference is valid");
#endif
			g_tempPluginString = instance->toString();
#if ENABLE_VERBOSE_LOGGING
			__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Plugin_JSONObject_ToString jsonData=%s", g_tempPluginString.c_str());
#endif
			return g_tempPluginString.c_str();
		}
		else
		{
			__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Plugin_JSONObject_ToString JSONObject reference is invalid");
		}
	}
	else
	{
		__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Plugin_JSONObject_ToString failed to find JSONObject reference");
	}

	return "";
}

static int g_refCountJSONArray = 0;
static map<int, JSONArray*> g_refJSONArray = map<int, JSONArray*>();

int Plugin_JSONArray_Construct()
{
	JSONArray* jsonArray = new JSONArray();
	g_refJSONArray[g_refCountJSONArray] = jsonArray;
	int result = g_refCountJSONArray;
	++g_refCountJSONArray;
	return result;
}

void Plugin_JSONArray_Put(int jsonArray, int index, int jsonObject)
{
	map<int, JSONArray*>::const_iterator searchJSONArray = g_refJSONArray.find(jsonArray);
	if (searchJSONArray == g_refJSONArray.end())
	{
		__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Plugin_JSONArray_Put JSONArray reference is invalid");
		return;
	}

	map<int, JSONObject*>::const_iterator searchJSONObject = g_refJSONObject.find(jsonObject);
	if (searchJSONObject == g_refJSONObject.end())
	{
		__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Plugin_JSONArray_Put JSONObject reference is invalid");
		return;
	}

	JSONArray* instanceJSONArray = searchJSONArray->second;
	if (!instanceJSONArray)
	{
		__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Plugin_JSONArray_Put JSONArray instance is invalid");
		return;
	}

	JSONObject* instanceJSONObject = searchJSONObject->second;
	if (!instanceJSONObject)
	{
		__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Plugin_JSONArray_Put JSONObject instance is invalid");
		return;
	}

	instanceJSONArray->put(index, instanceJSONObject->GetInstance());
}

void Plugin_JSONArray_PutString(int jsonArray, int index, const char* item)
{
	map<int, JSONArray*>::const_iterator searchJSONArray = g_refJSONArray.find(jsonArray);
	if (searchJSONArray == g_refJSONArray.end())
	{
		__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Plugin_JSONArray_Put JSONArray reference is invalid");
		return;
	} 

	JSONArray* instanceJSONArray = searchJSONArray->second;
	if (!instanceJSONArray)
	{
		__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Plugin_JSONArray_Put JSONArray instance is invalid");
		return;
	}

	string value = item;

	instanceJSONArray->put(index, value);
}

const char* Plugin_JSONArray_ToString(int jsonArray)
{
#if ENABLE_VERBOSE_LOGGING
	__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Plugin_JSONArray_ToString: jsonArray=%d", jsonArray);
#endif
	map<int, JSONArray*>::const_iterator search = g_refJSONArray.find(jsonArray);
	if (search != g_refJSONArray.end())
	{
		JSONArray* instance = search->second;
		if (instance)
		{
#if ENABLE_VERBOSE_LOGGING
			__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Plugin_JSONArray_ToString JSONArray reference is valid");
#endif
			g_tempPluginString = instance->toString();
#if ENABLE_VERBOSE_LOGGING
			__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Plugin_JSONArray_ToString jsonData=%s", g_tempPluginString.c_str());
#endif
			return g_tempPluginString.c_str();
		}
		else
		{
			__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Plugin_JSONArray_ToString JSONArray reference is invalid");
		}
	}
	else
	{
		__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Plugin_JSONArray_ToString failed to find JSONArray reference");
	}

	return "";
}
