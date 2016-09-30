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
#include "ApplicationCallbacksRequestLogin.h"
#include "RazerSDK_CallbacksRequestLogin.h"
#include "RazerSDK_CallbackSingleton.h"

#include "IwDebug.h"

#include <stdio.h>

#ifdef ENABLE_VERBOSE_LOGGING
#undef ENABLE_VERBOSE_LOGGING
#endif
#define ENABLE_VERBOSE_LOGGING false

using namespace RazerSDK;
using namespace std;

void RequestLoginOnSuccess(s3eRequestLoginSuccessEvent* event)
{
#if ENABLE_VERBOSE_LOGGING
	IwTrace(DEFAULT, ("void RequestLoginOnSuccess(event)"));
#endif
	if (event)
	{
#if ENABLE_VERBOSE_LOGGING
		IwTrace(DEFAULT, ("::RequestLoginOnSuccess:"));
#endif

		Application::s_ui.m_callbacksRequestLogin->OnSuccess();
	}
	else
	{
		Application::s_ui.m_callbacksRequestLogin->OnFailure(-1, "Success event is null");
	}
}

void RequestLoginOnFailure(s3eRequestLoginFailureEvent* event)
{
#if ENABLE_VERBOSE_LOGGING
	IwTrace(DEFAULT, ("void RequestLoginOnFailure(event)"));
#endif
	if (event)
	{
		Application::s_ui.m_callbacksRequestLogin->OnFailure(event->m_errorCode, event->m_errorMessage);
	}
	else
	{
		Application::s_ui.m_callbacksRequestLogin->OnFailure(-1, "Failure event is null");
	}
}

void RequestLoginOnCancel(s3eRequestLoginCancelEvent* event)
{
#if ENABLE_VERBOSE_LOGGING
	IwTrace(DEFAULT, ("void RequestLoginOnCancel(event)"));
#endif
	if (event)
	{
		Application::s_ui.m_callbacksRequestLogin->OnCancel();
	}
	else
	{
		Application::s_ui.m_callbacksRequestLogin->OnFailure(-1, "Cancel event is null");
	}
}

s3eCallback ApplicationCallbacksRequestLogin::GetSuccessEvent()
{
	return (s3eCallback)RequestLoginOnSuccess;
}

s3eCallback ApplicationCallbacksRequestLogin::GetFailureEvent()
{
	return (s3eCallback)RequestLoginOnFailure;
}

s3eCallback ApplicationCallbacksRequestLogin::GetCancelEvent()
{
	return (s3eCallback)RequestLoginOnCancel;
}

void ApplicationCallbacksRequestLogin::OnSuccess()
{
#if ENABLE_VERBOSE_LOGGING
	IwTrace(DEFAULT, ("ApplicationCallbacksRequestLogin::OnSuccess:"));
#endif

	Application::s_ui.SetMessage("ApplicationCallbacksRequestLogin::OnSuccess");
}

void ApplicationCallbacksRequestLogin::OnFailure(int errorCode, const string& errorMessage)
{
#if ENABLE_VERBOSE_LOGGING
	IwTrace(DEFAULT, ("OnFailure"));

	char buffer[256];
	sprintf(buffer, "OnFailure errorCode=%d errorMessage=%s", errorCode, errorMessage.c_str());
	IwTrace(DEFAULT, (buffer));
#endif

	string msg = "ApplicationCallbacksRequestLogin::OnFailure";
	msg.append(" errorMessage=");
	msg.append(errorMessage);
	Application::s_ui.SetMessage(msg);
}

void ApplicationCallbacksRequestLogin::OnCancel()
{
#if ENABLE_VERBOSE_LOGGING
	IwTrace(DEFAULT, ("OnCancel"));
#endif

	Application::s_ui.SetMessage("ApplicationCallbacksRequestLogin::OnCancel");
}