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
#include "ApplicationCallbacksShutdown.h"
#include "RazerSDK_CallbacksShutdown.h"

#include "IwDebug.h"

#include <stdio.h>

void ShutdownOnSuccess(s3eShutdownSuccessEvent* event)
{
#if ENABLE_VERBOSE_LOGGING
	IwTrace(DEFAULT, ("void ShutdownOnSuccess(event)"));
#endif
	if (event)
	{
		Application::s_ui.m_callbacksShutdown->OnSuccess();
	}
	else
	{
		Application::s_ui.m_callbacksShutdown->OnFailure(-1, "Success event is null");
	}
}

void ShutdownOnFailure(s3eShutdownFailureEvent* event)
{
#if ENABLE_VERBOSE_LOGGING
	IwTrace(DEFAULT, ("void ShutdownOnFailure(event)"));
#endif
	if (event)
	{
		Application::s_ui.m_callbacksShutdown->OnFailure(event->m_errorCode, event->m_errorMessage);
	}
	else
	{
		Application::s_ui.m_callbacksShutdown->OnFailure(-1, "Failure event is null");
	}
}

void ShutdownOnCancel(s3eShutdownCancelEvent* event)
{
#if ENABLE_VERBOSE_LOGGING
	IwTrace(DEFAULT, ("void ShutdownOnCancel(event)"));
#endif
	if (event)
	{
		Application::s_ui.m_callbacksShutdown->OnCancel();
	}
	else
	{
		Application::s_ui.m_callbacksShutdown->OnFailure(-1, "Cancel event is null");
	}
}

s3eCallback ApplicationCallbacksShutdown::GetSuccessEvent()
{
	return (s3eCallback)ShutdownOnSuccess;
}

s3eCallback ApplicationCallbacksShutdown::GetFailureEvent()
{
	return (s3eCallback)ShutdownOnFailure;
}

s3eCallback ApplicationCallbacksShutdown::GetCancelEvent()
{
	return (s3eCallback)ShutdownOnCancel;
}

void ApplicationCallbacksShutdown::OnSuccess()
{
#if ENABLE_VERBOSE_LOGGING
	IwTrace(DEFAULT, ("OnSuccess"));
#endif
	
	Application::s_ui.SetMessage("ApplicationCallbacksShutdown::OnSuccess");

	Application::s_ui.Exit();
}

void ApplicationCallbacksShutdown::OnFailure(int errorCode, const std::string& errorMessage)
{
#if ENABLE_VERBOSE_LOGGING
	char buffer[256];
	sprintf(buffer, "OnFailure errorCode=%d errorMessage=%s", errorCode, errorMessage.c_str());
	IwTrace(DEFAULT, (buffer));
#endif
	
	std::string msg = "ApplicationCallbacksShutdown::OnFailure";
	msg.append(" errorMessage=");
	msg.append(errorMessage);
	Application::s_ui.SetMessage(msg);
}

void ApplicationCallbacksShutdown::OnCancel()
{
#if ENABLE_VERBOSE_LOGGING
	IwTrace(DEFAULT, ("OnCancel"));
#endif
	
	Application::s_ui.SetMessage("ApplicationCallbacksShutdown::OnCancel");
}
