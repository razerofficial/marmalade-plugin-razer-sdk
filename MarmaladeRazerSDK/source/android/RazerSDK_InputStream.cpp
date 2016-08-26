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

#include "RazerSDK_InputStream.h"

#include "s3eEdk.h"
#include "s3eEdk_android.h"

#include <android/log.h>
#include <jni.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "java_io_InputStream"

#ifdef ENABLE_VERBOSE_LOGGING
#undef ENABLE_VERBOSE_LOGGING
#endif
#define ENABLE_VERBOSE_LOGGING false

namespace java_io_InputStream
{
	jclass InputStream::_jcInputStream = 0;
	jmethodID InputStream::_jmClose = 0;
	jmethodID InputStream::_jmRead = 0;

	int InputStream::InitJNI()
	{
		JNIEnv* env = s3eEdkJNIGetEnv();

		{
			const char* strClass = "java/io/InputStream";
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
				_jcInputStream = (jclass)env->NewGlobalRef(localRef);
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

	void InputStream::FindJNI()
	{
		JNIEnv* env = s3eEdkJNIGetEnv();

		{
			const char* strMethod = "close";
			_jmClose = env->GetMethodID(_jcInputStream, strMethod, "()V");
			if (_jmClose)
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

		{
			const char* strMethod = "read";
			_jmRead = env->GetMethodID(_jcInputStream, strMethod, "([B)I");
			if (_jmRead)
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

	InputStream::InputStream(jobject instance)
	{
		_instance = instance;
	}

	jobject InputStream::GetInstance() const
	{
		return _instance;
	}

	void InputStream::Dispose() const
	{
		JNIEnv* env = s3eEdkJNIGetEnv();

		if (_instance)
		{
			env->DeleteGlobalRef(_instance);
		}
	}

	void InputStream::close() const
	{
		FindJNI();

		JNIEnv* env = s3eEdkJNIGetEnv();
		
		FindJNI();

		if (!_instance) {
			__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "_instance is null");
			return;
		}

		if (!_jmClose) {
			__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "_jmClose is null");
			return;
		}

#if ENABLE_VERBOSE_LOGGING
		__android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, "Invoking close");
#endif
		env->CallVoidMethod(_instance, _jmClose);

		if (env->ExceptionCheck())
		{
			env->ExceptionDescribe();
			env->ExceptionClear();
			__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Failed to close");
		}
	}

	int InputStream::read(signed char buffer[], int length) const
	{
#if ENABLE_VERBOSE_LOGGING
		__android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, "read");
#endif

		if (!buffer)
		{
			__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "buffer is null");
			return 0;
		}
#if ENABLE_VERBOSE_LOGGING
		__android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, "buffer is valid");
#endif

		JNIEnv* env = s3eEdkJNIGetEnv();
		
		FindJNI();

#if ENABLE_VERBOSE_LOGGING
		__android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, "env is valid");
#endif

		if (!_instance) {
			__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "_instance is null");
			return 0;
		}

#if ENABLE_VERBOSE_LOGGING
		__android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, "_instance is valid");
#endif

		if (!_jmRead) {
			__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "_jmRead is null");
			return 0;
		}

#if ENABLE_VERBOSE_LOGGING
		__android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, "_jmRead is valid");
#endif

#if ENABLE_VERBOSE_LOGGING
		__android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, "Building byte array length=%d", length);
#endif
		jbyteArray arg1 = env->NewByteArray(length);


#if ENABLE_VERBOSE_LOGGING
		__android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, "Reading bytes...");
#endif

		int bytesRead = env->CallIntMethod(_instance, _jmRead, arg1);

		if (env->ExceptionCheck())
		{
			env->ExceptionDescribe();
			env->ExceptionClear();
			__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Failed to read");
		}

		if (bytesRead <= 0)
		{
			env->DeleteLocalRef(arg1);
			__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Failed to read any bytes");
			return 0;
		}

#if ENABLE_VERBOSE_LOGGING
		__android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, "Reading array buffer bytes...");
#endif
		env->GetByteArrayRegion(arg1, 0, bytesRead, buffer);
		env->DeleteLocalRef(arg1);
		return bytesRead;
	}
}
