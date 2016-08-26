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

#include "RazerSDK_CallbacksRequestGamerInfo.h"
#include "RazerSDK_CallbackSingleton.h"

#include "RazerSDK_internal.h"
#include "s3eEdk.h"
#include "s3eEdk_android.h"
#include "IwDebug.h"

#include <stdio.h>

#include <android/log.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "CallbacksRequestGamerInfo"

#ifdef ENABLE_VERBOSE_LOGGING
#undef ENABLE_VERBOSE_LOGGING
#endif
#define ENABLE_VERBOSE_LOGGING false

void CallbacksRequestGamerInfo::RegisterCallback(s3eCallback callback, s3eCallback* savedCallback, int callbackType)
{
	if (*savedCallback)
	{
		//IwTrace(RazerSDK, ("Unregistering Callback"));

		s3eEdkCallbacksUnRegister(
				S3E_EXT_RAZERSDK_HASH,
				S3E_RAZERSDK_CALLBACKS_MAX,
				callbackType,
				*savedCallback,
				NULL);
		*savedCallback = NULL;
	}

	*savedCallback = callback;

	//IwTrace(RazerSDK, ("Registering Callback"));

	s3eEdkCallbacksRegister(
			S3E_EXT_RAZERSDK_HASH,
			S3E_RAZERSDK_CALLBACKS_MAX,
			callbackType,
			*savedCallback,
			NULL,
			S3E_FALSE);
}

void CallbacksRequestGamerInfo::RegisterCallbacks(s3eCallback onSuccess, s3eCallback onFailure, s3eCallback onCancel)
{
	RegisterCallback(onSuccess, &m_onSuccess, S3E_RAZERSDK_CALLBACKS_REQUEST_GAMER_INFO_ON_SUCCESS);
	RegisterCallback(onFailure, &m_onFailure, S3E_RAZERSDK_CALLBACKS_REQUEST_GAMER_INFO_ON_FAILURE);
	RegisterCallback(onCancel, &m_onCancel, S3E_RAZERSDK_CALLBACKS_REQUEST_GAMER_INFO_ON_CANCEL);
}

void CallbacksRequestGamerInfo::OnSuccess(const RazerSDK::GamerInfo& gamerInfo)
{
#if ENABLE_VERBOSE_LOGGING
	__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "OnSuccess: username=%s uuid=%s", gamerInfo.Username.c_str(), gamerInfo.Uuid.c_str());
	
	IwTrace(DEFAULT, ("Application: username="));
	IwTrace(DEFAULT, (gamerInfo.Username.c_str()));

	IwTrace(DEFAULT, ("Application: uuid="));
	IwTrace(DEFAULT, (gamerInfo.Uuid.c_str()));
#endif

	//don't send a temp pointer
	m_dataRequestGamerInfoSuccessEvent.m_gamerInfo.Init();
	m_dataRequestGamerInfoSuccessEvent.m_gamerInfo.Copy(gamerInfo);

	s3eEdkCallbacksEnqueue(S3E_EXT_RAZERSDK_HASH, S3E_RAZERSDK_CALLBACKS_REQUEST_GAMER_INFO_ON_SUCCESS, &m_dataRequestGamerInfoSuccessEvent);
}

void CallbacksRequestGamerInfo::OnFailure(int errorCode, const std::string& errorMessage)
{
#if ENABLE_VERBOSE_LOGGING
	char buffer[256];
	sprintf(buffer, "OnFailure errorCode=%d errorMessage=%s", errorCode, errorMessage.c_str());
	IwTrace(RazerSDK, (buffer));
#endif

	s3eRequestGamerInfoFailureEvent event;
	event.m_errorCode = errorCode;
	sprintf(event.m_errorMessage, "%s", errorMessage.c_str());

	m_dataRequestGamerInfoFailureEvent = event; //don't send a temp pointer
	s3eEdkCallbacksEnqueue(S3E_EXT_RAZERSDK_HASH, S3E_RAZERSDK_CALLBACKS_REQUEST_GAMER_INFO_ON_FAILURE, &m_dataRequestGamerInfoFailureEvent);
}

void CallbacksRequestGamerInfo::OnCancel()
{
#if ENABLE_VERBOSE_LOGGING
	IwTrace(RazerSDK, ("OnCancel"));
#endif

	s3eRequestGamerInfoCancelEvent event;

	m_dataRequestGamerInfoCancelEvent = event; //don't send a temp pointer
	s3eEdkCallbacksEnqueue(S3E_EXT_RAZERSDK_HASH, S3E_RAZERSDK_CALLBACKS_REQUEST_GAMER_INFO_ON_CANCEL, &m_dataRequestGamerInfoCancelEvent);
}
