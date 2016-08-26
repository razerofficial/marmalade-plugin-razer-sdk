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

#include "RazerSDK_Color.h"

#include "s3eEdk.h"
#include "s3eEdk_android.h"

#include <android/log.h>
#include <jni.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "android_graphics_Color"

#ifdef ENABLE_VERBOSE_LOGGING
#undef ENABLE_VERBOSE_LOGGING
#endif
#define ENABLE_VERBOSE_LOGGING false

namespace android_graphics_Color
{
	jclass Color::_jcColor = 0;
	jmethodID Color::_jmArgb = 0;

	int Color::InitJNI()
	{
		JNIEnv* env = s3eEdkJNIGetEnv();

		{
			const char* strClass = "android/graphics/Color";
#if ENABLE_VERBOSE_LOGGING
			__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Searching for %s", strClass);
#endif
			// system classes don't use `FAndroidApplication`
			jclass localRef = env->FindClass(strClass);
			if (localRef)
			{
#if ENABLE_VERBOSE_LOGGING
				__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Found %s", strClass);
#endif
				_jcColor = (jclass)env->NewGlobalRef(localRef);
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

	void Color::FindJNI()
	{
		JNIEnv* env = s3eEdkJNIGetEnv();

		{
			const char* strMethod = "argb";
			_jmArgb = env->GetStaticMethodID(_jcColor, strMethod, "(IIII)I");
			if (_jmArgb)
			{
#if ENABLE_VERBOSE_LOGGING
				__android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, "Found %s", strMethod);
#endif
			}
			else
			{
				__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Failed to find %s", strMethod);
				return;
			}
		}
	}

	Color::Color(jobject instance)
	{
		_instance = instance;
	}

	jobject Color::GetInstance() const
	{
		return _instance;
	}

	void Color::Dispose() const
	{
		JNIEnv* env = s3eEdkJNIGetEnv();

		if (_instance)
		{
			env->DeleteGlobalRef(_instance);
		}
	}

	int Color::argb(int alpha, int red, int green, int blue)
	{
		JNIEnv* env = s3eEdkJNIGetEnv();
		
		FindJNI();

		if (!_jcColor) {
			__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "_jcColor is null");
			return 0;
		}

		if (!_jmArgb) {
			__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "_jmArgb is null");
			return 0;
		}

		int arg1 = alpha;
		int arg2 = red;
		int arg3 = green;
		int arg4 = blue;
		jint result = env->CallStaticIntMethod(_jcColor, _jmArgb, arg1, arg2, arg3, arg4);
		return result;
	}
}
