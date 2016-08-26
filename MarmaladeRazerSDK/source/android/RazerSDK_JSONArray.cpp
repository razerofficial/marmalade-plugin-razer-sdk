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

#include "RazerSDK_JSONArray.h"
#include "RazerSDK_JSONObject.h"

#include "RazerSDK_internal.h"
#include "s3eEdk.h"
#include "s3eEdk_android.h"

#include <android/log.h>
#include <jni.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "org_json_JSONArray"

#ifdef ENABLE_VERBOSE_LOGGING
#undef ENABLE_VERBOSE_LOGGING
#endif
#define ENABLE_VERBOSE_LOGGING false

namespace org_json_JSONArray
{
	jclass JSONArray::_jcJsonArray = 0;
	jmethodID JSONArray::_mConstruct = 0;
	jmethodID JSONArray::_mConstruct2 = 0;
	jmethodID JSONArray::_mGetInt = 0;
	jmethodID JSONArray::_mGetJsonObject = 0;
	jmethodID JSONArray::_mGetString = 0;
	jmethodID JSONArray::_mLength = 0;
	jmethodID JSONArray::_mPut = 0;
	jmethodID JSONArray::_mToString = 0;

	int JSONArray::InitJNI()
	{
		JNIEnv* env = s3eEdkJNIGetEnv();
		
		{
			const char* strClass = "org/json/JSONArray";
#if ENABLE_VERBOSE_LOGGING
			__android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, "Searching for %s", strClass);
#endif
			jclass localRef = (jclass)env->FindClass(strClass);
			if (localRef)
			{
#if ENABLE_VERBOSE_LOGGING
				__android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, "Found %s", strClass);
#endif
				_jcJsonArray = (jclass)env->NewGlobalRef(localRef);
				env->DeleteLocalRef(localRef);
			}
			else
			{
				__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Failed to find %s", strClass);
				return JNI_ERR;
			}
		}

		return JNI_OK;
	}

	void JSONArray::FindJNI()
	{
		JNIEnv* env = s3eEdkJNIGetEnv();

		{
			const char* strStringConstructor = "<init>";
			_mConstruct = env->GetMethodID(_jcJsonArray, strStringConstructor, "()V");
			if (_mConstruct)
			{
#if ENABLE_VERBOSE_LOGGING
				__android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, "Found %s", strStringConstructor);
#endif
			}
			else
			{
				__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Failed to find %s", strStringConstructor);
			}
		}

		{
			const char* strStringConstructor = "<init>";
			_mConstruct2 = env->GetMethodID(_jcJsonArray, strStringConstructor, "(Ljava/lang/String;)V");
			if (_mConstruct2)
			{
#if ENABLE_VERBOSE_LOGGING
				__android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, "Found %s", strStringConstructor);
#endif
			}
			else
			{
				__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Failed to find %s", strStringConstructor);
			}
		}

		{
			const char* strJsonObjectGetJsonObject = "getJSONObject";
			_mGetJsonObject = env->GetMethodID(_jcJsonArray, strJsonObjectGetJsonObject, "(I)Lorg/json/JSONObject;");
			if (_mGetJsonObject)
			{
	#if ENABLE_VERBOSE_LOGGING
				__android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, "Found %s", strJsonObjectGetJsonObject);
	#endif
			}
			else
			{
				__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Failed to find %s", strJsonObjectGetJsonObject);
			}
		}

		{
			const char* strJsonObjectGetInt = "getInt";
			_mGetInt = env->GetMethodID(_jcJsonArray, strJsonObjectGetInt, "(I)I");
			if (_mGetInt)
			{
#if ENABLE_VERBOSE_LOGGING
				__android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, "Found %s", strJsonObjectGetInt);
#endif
			}
			else
			{
				__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Failed to find %s", strJsonObjectGetInt);
			}
		}

		{
			const char* strJsonObjectGetString = "getString";
			_mGetString = env->GetMethodID(_jcJsonArray, strJsonObjectGetString, "(I)Ljava/lang/String;");
			if (_mGetString)
			{
#if ENABLE_VERBOSE_LOGGING
				__android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, "Found %s", strJsonObjectGetString);
#endif
			}
			else
			{
				__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Failed to find %s", strJsonObjectGetString);
			}
		}

		{
			const char* strJsonObjectLength = "length";
			_mLength = env->GetMethodID(_jcJsonArray, strJsonObjectLength, "()I");
			if (_mLength)
			{
#if ENABLE_VERBOSE_LOGGING
				__android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, "Found %s", strJsonObjectLength);
#endif
			}
			else
			{
				__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Failed to find %s", strJsonObjectLength);
			}
		}

		{
			const char* strJsonObjectLength = "put";
			_mPut = env->GetMethodID(_jcJsonArray, strJsonObjectLength, "(ILjava/lang/Object;)Lorg/json/JSONArray;");
			if (_mPut)
			{
#if ENABLE_VERBOSE_LOGGING
				__android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, "Found %s", strJsonObjectLength);
#endif
			}
			else
			{
				__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Failed to find %s", strJsonObjectLength);
			}
		}

		{
			const char* strJsonObjectLength = "toString";
			_mToString = env->GetMethodID(_jcJsonArray, strJsonObjectLength, "()Ljava/lang/String;");
			if (_mToString)
			{
#if ENABLE_VERBOSE_LOGGING
				__android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, "Found %s", strJsonObjectLength);
#endif
			}
			else
			{
				__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Failed to find %s", strJsonObjectLength);
			}
		}
	}

	JSONArray::JSONArray(jobject jsonArray)
	{
		_instance = jsonArray;
	}

	jobject JSONArray::GetInstance() const
	{
		return _instance;
	}

	JSONArray::JSONArray()
	{
		JNIEnv* env = s3eEdkJNIGetEnv();
		
		FindJNI();

		if (!env)
		{
			__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "JNI must be initialized with a valid environment!");
			return;
		}

		_instance = env->AllocObject(_jcJsonArray);
		if (_instance)
		{
#if ENABLE_VERBOSE_LOGGING
			__android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, "Success allocate JSONArray");
#endif
			_instance = env->NewGlobalRef(_instance);
		}
		else
		{
			__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Failed to allocate JSONArray");
			return;
		}

		if (!_mConstruct)
		{
			__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "_mConstruct is not valid!");
			return;
		}

		env->CallVoidMethod(_instance, _mConstruct);

		if (env->ExceptionCheck())
		{
			env->ExceptionDescribe();
			env->ExceptionClear();
			__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Failed to construct JSONArray");
			return;
		}

#if ENABLE_VERBOSE_LOGGING
		__android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, "Success constructed JSONArray");
#endif
	}

	JSONArray::JSONArray(const std::string& buffer)
	{
		JNIEnv* env = s3eEdkJNIGetEnv();
		
		FindJNI();

		if (!env)
		{
			__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "JNI must be initialized with a valid environment!");
			return;
		}

		_instance = env->AllocObject(_jcJsonArray);
		if (_instance)
		{
#if ENABLE_VERBOSE_LOGGING
			__android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, "Success allocate JSONArray");
#endif
		}
		else
		{
			__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Failed to allocate JSONArray");
			return;
		}

		if (!_mConstruct2)
		{
			__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "_mConstruct2 is not valid!");
			return;
		}

		jstring arg1 = env->NewStringUTF(buffer.c_str());
		env->CallVoidMethod(_instance, _mConstruct2, arg1);

		if (env->ExceptionCheck())
		{
			env->ExceptionDescribe();
			env->ExceptionClear();
			__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Failed to construct JSONArray");
			return;
		}

#if ENABLE_VERBOSE_LOGGING
		__android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, "Success constructed JSONArray");
#endif
		env->DeleteLocalRef(arg1);
	}

	org_json_JSONObject::JSONObject JSONArray::getJSONObject(int index) const
	{
		JNIEnv* env = s3eEdkJNIGetEnv();
		
		FindJNI();

		if (!env)
		{
			__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "JNI must be initialized with a valid environment!");
			return org_json_JSONObject::JSONObject(0);
		}

		if (!_instance)
		{
			__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Instance is not valid!");
			return org_json_JSONObject::JSONObject(0);
		}

		if (!_mGetJsonObject)
		{
			__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "_mGetJsonObject is not valid!");
			return org_json_JSONObject::JSONObject(0);
		}

		jint arg1 = index;
		jobject result = env->CallObjectMethod(_instance, _mGetJsonObject, arg1);

		if (env->ExceptionCheck())
		{
			env->ExceptionDescribe();
			env->ExceptionClear();
			__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Failed to get json object");
			return org_json_JSONObject::JSONObject(0);
		}

		if (result)
		{
#if ENABLE_VERBOSE_LOGGING
			__android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, "Success got json object");
#endif
			return org_json_JSONObject::JSONObject(result);
		}
		else
		{
			return org_json_JSONObject::JSONObject(0);
		}
	}

	int JSONArray::getInt(int index) const
	{
		JNIEnv* env = s3eEdkJNIGetEnv();
		
		FindJNI();

		if (!env)
		{
			__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "JNI must be initialized with a valid environment!");
			return 0;
		}

		if (!_instance)
		{
			__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Instance is not valid!");
			return 0;
		}

		if (!_mGetInt)
		{
			__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "_mGetInt is not valid!");
			return 0;
		}

		jint arg1 = index;
		jint result = env->CallIntMethod(_instance, _mGetInt, arg1);
		if (env->ExceptionCheck())
		{
			env->ExceptionDescribe();
			env->ExceptionClear();
			__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Failed to get int");
			return 0;
		}
		else
		{
#if ENABLE_VERBOSE_LOGGING
			__android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, "Success on get int: %d", result);
#endif
			return (int)result;
		}
	}

	std::string JSONArray::getString(int index) const
	{
		JNIEnv* env = s3eEdkJNIGetEnv();
		
		FindJNI();

		if (!env)
		{
			__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "JNI must be initialized with a valid environment!");
			return std::string();
		}

		if (!_instance)
		{
			__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Instance is not valid!");
			return std::string();
		}

		if (!_mGetString)
		{
			__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "_mGetString is not valid!");
			return std::string();
		}

		jint arg1 = index;
		jstring result = (jstring)env->CallObjectMethod(_instance, _mGetString, arg1);
		if (env->ExceptionCheck())
		{
			env->ExceptionDescribe();
			env->ExceptionClear();
			__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Failed to get string");
			return std::string();
		}
		else
		{
			__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Success on get string");
		}

		if (result)
		{
			const char* nativeString = env->GetStringUTFChars(result, 0);
			std::string retVal = std::string(nativeString);
			env->ReleaseStringUTFChars(result, nativeString);

#if ENABLE_VERBOSE_LOGGING
			__android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, "Get string: %s", retVal.c_str());
#endif
			return retVal;
		}
		else
		{
			return std::string();
		}
	}

	int JSONArray::length() const
	{
		JNIEnv* env = s3eEdkJNIGetEnv();
		
		FindJNI();

		if (!env)
		{
			__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "JNI must be initialized with a valid environment!");
			return 0;
		}

		if (!_instance)
		{
			__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Instance is not valid!");
			return 0;
		}

		if (!_mLength)
		{
			__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "_mLength is not valid!");
			return 0;
		}

		jint result = env->CallIntMethod(_instance, _mLength);
#if ENABLE_VERBOSE_LOGGING
		__android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, "Success on length: %d", result);
#endif
		return result;
	}

	JSONArray JSONArray::put(int index, const std::string& value) const
	{
		JNIEnv* env = s3eEdkJNIGetEnv();
		
		FindJNI();

		if (!env)
		{
			__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "JNI must be initialized with a valid environment!");
			return NULL;
		}

		if (!_instance)
		{
			__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Instance is not valid!");
			return NULL;
		}

		if (!_mPut)
		{
			__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "_mPut is not valid!");
			return NULL;
		}

		jint arg1 = index;
		jstring arg2 = env->NewStringUTF(value.c_str());
		jobject retVal = env->CallObjectMethod(_instance, _mPut, arg1, arg2);
		if (env->ExceptionCheck())
		{
			env->ExceptionDescribe();
			env->ExceptionClear();
			__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Failed to put string");
			return NULL;
		}

		if (retVal)
		{
			JSONArray result = JSONArray(retVal);			
			return result;
		}
		else
		{
			return NULL;
		}
	}

	JSONArray JSONArray::put(int index, const org_json_JSONObject::JSONObject& jsonObject) const
	{
		JNIEnv* env = s3eEdkJNIGetEnv();
		
		FindJNI();

		if (!env)
		{
			__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "JNI must be initialized with a valid environment!");
			return NULL;
		}

		if (!_instance)
		{
			__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Instance is not valid!");
			return NULL;
		}

		if (!_mPut)
		{
			__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "_mPut is not valid!");
			return NULL;
		}
		
		jint arg1 = index;
		jobject arg2 = jsonObject.GetInstance();
		jobject retVal = env->CallObjectMethod(_instance, _mPut, arg1, arg2);
		if (env->ExceptionCheck())
		{
			env->ExceptionDescribe();
			env->ExceptionClear();
			__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Failed to put string");
			return NULL;
		}

		if (retVal)
		{
			JSONArray result = JSONArray(retVal);			
			return result;
		}
		else
		{
			return NULL;
		}
	}

	std::string JSONArray::toString() const
	{
		JNIEnv* env = s3eEdkJNIGetEnv();
		
		FindJNI();

		if (!env)
		{
			__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "JNI must be initialized with a valid environment!");
			return std::string();
		}

		if (!_instance)
		{
			__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Instance is not valid!");
			return std::string();
		}
#if ENABLE_VERBOSE_LOGGING
		else
		{
			__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Instance is valid!");
		}
#endif

		if (!_mToString)
		{
			__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "_mToString is not valid!");
			return std::string();
		}
#if ENABLE_VERBOSE_LOGGING
		else
		{
			__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "_mToString is valid!");
		}
#endif

		jstring retVal = (jstring)env->CallObjectMethod(_instance, _mToString);
		if (env->ExceptionCheck())
		{
			env->ExceptionDescribe();
			env->ExceptionClear();
			__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Failed to toString");
			return std::string();
		}

		if (retVal)
		{
			const char* nativeString = env->GetStringUTFChars(retVal, 0);
			std::string result = std::string(nativeString);
			env->ReleaseStringUTFChars(retVal, nativeString);

#if ENABLE_VERBOSE_LOGGING
			__android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, "toString: %s", result.c_str());
#endif
			return result;
		}
		else
		{
			return std::string();
		}
	}
}
