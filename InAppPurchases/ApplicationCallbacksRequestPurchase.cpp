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
#include "ApplicationCallbacksRequestPurchase.h"
#include "RazerSDK_CallbacksRequestPurchase.h"

#include "IwDebug.h"

#include <stdio.h>

using namespace RazerSDK;
using namespace std;

void RequestPurchaseOnSuccess(s3eRequestPurchaseSuccessEvent* event)
{
#if ENABLE_VERBOSE_LOGGING
	IwTrace(DEFAULT, ("void RequestPurchaseOnSuccess(event)"));
#endif
	if (event)
	{
		ExtensionPurchaseResult ePurchaseResult = event->m_purchaseResult;
		PurchaseResult purchaseResult;
		purchaseResult.Init();
		purchaseResult.Copy(ePurchaseResult);

		Application::s_ui.m_callbacksRequestPurchase->OnSuccess(purchaseResult);
	}
	else
	{
		Application::s_ui.m_callbacksRequestPurchase->OnFailure(-1, "Success event is null");
	}
}

void RequestPurchaseOnFailure(s3eRequestPurchaseFailureEvent* event)
{
#if ENABLE_VERBOSE_LOGGING
	IwTrace(DEFAULT, ("void RequestPurchaseOnFailure(event)"));
#endif
	if (event)
	{
		Application::s_ui.m_callbacksRequestPurchase->OnFailure(event->m_errorCode, event->m_errorMessage);
	}
	else
	{
		Application::s_ui.m_callbacksRequestPurchase->OnFailure(-1, "Failure event is null");
	}
}

void RequestPurchaseOnCancel(s3eRequestPurchaseCancelEvent* event)
{
#if ENABLE_VERBOSE_LOGGING
	IwTrace(DEFAULT, ("void RequestPurchaseOnCancel(event)"));
#endif
	if (event)
	{
		Application::s_ui.m_callbacksRequestPurchase->OnCancel();
	}
	else
	{
		Application::s_ui.m_callbacksRequestPurchase->OnFailure(-1, "Cancel event is null");
	}
}

s3eCallback ApplicationCallbacksRequestPurchase::GetSuccessEvent()
{
	return (s3eCallback)RequestPurchaseOnSuccess;
}

s3eCallback ApplicationCallbacksRequestPurchase::GetFailureEvent()
{
	return (s3eCallback)RequestPurchaseOnFailure;
}

s3eCallback ApplicationCallbacksRequestPurchase::GetCancelEvent()
{
	return (s3eCallback)RequestPurchaseOnCancel;
}

void ApplicationCallbacksRequestPurchase::OnSuccess(const PurchaseResult& purchaseResult)
{
#if ENABLE_VERBOSE_LOGGING
	IwTrace(DEFAULT, ("OnSuccess"));
#endif

	string message = "ApplicationCallbacksRequestPurchase::OnSuccess: ";
	message.append(purchaseResult.Identifier);

	Application::s_ui.SetMessage(message);
}

void ApplicationCallbacksRequestPurchase::OnFailure(int errorCode, const string& errorMessage)
{
#if ENABLE_VERBOSE_LOGGING
	char buffer[256];
	sprintf(buffer, "OnFailure errorCode=%d errorMessage=%s", errorCode, errorMessage.c_str());
	IwTrace(DEFAULT, (buffer));
#endif
	
	string msg = "ApplicationCallbacksRequestPurchase::OnFailure";
	msg.append(" errorMessage=");
	msg.append(errorMessage);
	Application::s_ui.SetMessage(msg);
}

void ApplicationCallbacksRequestPurchase::OnCancel()
{
#if ENABLE_VERBOSE_LOGGING
	IwTrace(DEFAULT, ("OnCancel"));
#endif
	
	Application::s_ui.SetMessage("ApplicationCallbacksRequestPurchase::OnCancel");
}