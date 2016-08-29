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

#include "Application.h"
#include "ApplicationCallbacksRequestGamerInfo.h"
#include "ApplicationGamerInfo.h"
#include "RazerSDK_CallbacksRequestGamerInfo.h"
#include "RazerSDK_CallbackSingleton.h"
#include "RazerSDK_ExtensionGamerInfo.h"

#include "IwDebug.h"

#include <stdio.h>

#ifdef ENABLE_VERBOSE_LOGGING
#undef ENABLE_VERBOSE_LOGGING
#endif
#define ENABLE_VERBOSE_LOGGING false

using namespace RazerSDK;
using namespace std;

void RequestGamerInfoOnSuccess(s3eRequestGamerInfoSuccessEvent* event)
{
#if ENABLE_VERBOSE_LOGGING
	IwTrace(DEFAULT, ("void RequestGamerInfoOnSuccess(event)"));
#endif
	if (event)
	{
#if ENABLE_VERBOSE_LOGGING
		IwTrace(DEFAULT, ("::RequestGamerInfoOnSuccess: username="));
		IwTrace(DEFAULT, (event->m_gamerInfo.Username));

		IwTrace(DEFAULT, ("::RequestGamerInfoOnSuccess: uuid="));
		IwTrace(DEFAULT, (event->m_gamerInfo.Uuid));
#endif


		GamerInfo gamerInfo;
		gamerInfo.Copy(event->m_gamerInfo);

		Application::s_ui.m_callbacksRequestGamerInfo->OnSuccess(gamerInfo);
	}
	else
	{
		Application::s_ui.m_callbacksRequestGamerInfo->OnFailure(-1, "Success event is null");
	}
}

void RequestGamerInfoOnFailure(s3eRequestGamerInfoFailureEvent* event)
{
#if ENABLE_VERBOSE_LOGGING
	IwTrace(DEFAULT, ("void RequestGamerInfoOnFailure(event)"));
#endif
	if (event)
	{
		Application::s_ui.m_callbacksRequestGamerInfo->OnFailure(event->m_errorCode, event->m_errorMessage);
	}
	else
	{
		Application::s_ui.m_callbacksRequestGamerInfo->OnFailure(-1, "Failure event is null");
	}
}

void RequestGamerInfoOnCancel(s3eRequestGamerInfoCancelEvent* event)
{
#if ENABLE_VERBOSE_LOGGING
	IwTrace(DEFAULT, ("void RequestGamerInfoOnCancel(event)"));
#endif
	if (event)
	{
		Application::s_ui.m_callbacksRequestGamerInfo->OnCancel();
	}
	else
	{
		Application::s_ui.m_callbacksRequestGamerInfo->OnFailure(-1, "Cancel event is null");
	}
}

s3eCallback ApplicationCallbacksRequestGamerInfo::GetSuccessEvent()
{
	return (s3eCallback)RequestGamerInfoOnSuccess;
}

s3eCallback ApplicationCallbacksRequestGamerInfo::GetFailureEvent()
{
	return (s3eCallback)RequestGamerInfoOnFailure;
}

s3eCallback ApplicationCallbacksRequestGamerInfo::GetCancelEvent()
{
	return (s3eCallback)RequestGamerInfoOnCancel;
}

void ApplicationCallbacksRequestGamerInfo::OnSuccess(const GamerInfo& gamerInfo)
{
#if ENABLE_VERBOSE_LOGGING
	IwTrace(DEFAULT, ("ApplicationCallbacksRequestGamerInfo::OnSuccess: username="));
	IwTrace(DEFAULT, (gamerInfo.Username.c_str()));

	IwTrace(DEFAULT, ("ApplicationCallbacksRequestGamerInfo::OnSuccess: uuid="));
	IwTrace(DEFAULT, (gamerInfo.Uuid.c_str()));
#endif

	Application::s_ui.SetMessage("ApplicationCallbacksRequestGamerInfo::OnSuccess");
	Application::s_ui.SetUsername(gamerInfo.Username);
	Application::s_ui.SetGamerUUID(gamerInfo.Uuid);
}

void ApplicationCallbacksRequestGamerInfo::OnFailure(int errorCode, const string& errorMessage)
{
#if ENABLE_VERBOSE_LOGGING
	IwTrace(DEFAULT, ("OnFailure"));

	char buffer[256];
	sprintf(buffer, "OnFailure errorCode=%d errorMessage=%s", errorCode, errorMessage.c_str());
	IwTrace(DEFAULT, (buffer));
#endif

	string msg = "ApplicationCallbacksRequestGamerInfo::OnFailure";
	msg.append(" errorMessage=");
	msg.append(errorMessage);
	Application::s_ui.SetMessage(msg);
}

void ApplicationCallbacksRequestGamerInfo::OnCancel()
{
#if ENABLE_VERBOSE_LOGGING
	IwTrace(DEFAULT, ("OnCancel"));
#endif

	Application::s_ui.SetMessage("ApplicationCallbacksRequestGamerInfo::OnCancel");
}