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
#include "ApplicationCallbacksInitPlugin.h"
#include "RazerSDK_CallbacksInitPlugin.h"
#include "RazerSDK_CallbackSingleton.h"

#include "IwDebug.h"

#include <stdio.h>

void InitPluginOnSuccess(s3eInitPluginSuccessEvent* event)
{
#if ENABLE_VERBOSE_LOGGING
	IwTrace(DEFAULT, ("void InitPluginOnSuccess(event)"));
#endif
	if (event)
	{
		Application::s_ui.m_callbacksInitPlugin->OnSuccess();
	}
	else
	{
		Application::s_ui.m_callbacksInitPlugin->OnFailure(-1, "Success event is null");
	}
}

void InitPluginOnFailure(s3eInitPluginFailureEvent* event)
{
#if ENABLE_VERBOSE_LOGGING
	IwTrace(DEFAULT, ("void InitPluginOnFailure(event)"));
#endif
	if (event)
	{
		Application::s_ui.m_callbacksInitPlugin->OnFailure(event->m_errorCode, event->m_errorMessage);
	}
	else
	{
		Application::s_ui.m_callbacksInitPlugin->OnFailure(-1, "Failure event is null");
	}
}

s3eCallback ApplicationCallbacksInitPlugin::GetSuccessEvent()
{
	return (s3eCallback)InitPluginOnSuccess;
}

s3eCallback ApplicationCallbacksInitPlugin::GetFailureEvent()
{
	return (s3eCallback)InitPluginOnFailure;
}

void ApplicationCallbacksInitPlugin::OnSuccess()
{
#if ENABLE_VERBOSE_LOGGING
	IwTrace(DEFAULT, ("OnSuccess"));
#endif

	Application::s_ui.SetMessage("ApplicationCallbacksInitPlugin::OnSuccess");
}

void ApplicationCallbacksInitPlugin::OnFailure(int errorCode, const std::string& errorMessage)
{
#if ENABLE_VERBOSE_LOGGING
	IwTrace(DEFAULT, ("OnFailure"));

	char buffer[256];
	sprintf(buffer, "OnFailure errorCode=%d errorMessage=%s", errorCode, errorMessage.c_str());
	IwTrace(DEFAULT, (buffer));
#endif

	std::string msg = "ApplicationCallbacksInitPlugin::OnFailure";
	msg.append(" errorMessage=");
	msg.append(errorMessage);
	Application::s_ui.SetMessage(msg);

	Application::InitPlugin();
}
