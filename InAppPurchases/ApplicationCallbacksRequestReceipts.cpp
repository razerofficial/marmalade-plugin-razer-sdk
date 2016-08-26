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
#include "ApplicationCallbacksRequestReceipts.h"
#include "RazerSDK_CallbacksRequestReceipts.h"
#include "RazerSDK_ExtensionReceipt.h"

#include "IwDebug.h"

#include <stdio.h>

void RequestReceiptsOnSuccess(s3eRequestReceiptsSuccessEvent* event)
{
#if ENABLE_VERBOSE_LOGGING
	IwTrace(DEFAULT, ("void RequestReceiptsOnSuccess(event)"));
#endif
	if (event)
	{
		std::vector<RazerSDK::Receipt> receipts;
		for (int index = 0; index < event->m_receiptsLength; ++index)
		{
			RazerSDK::ExtensionReceipt eReceipt = event->m_receipts[index];
			RazerSDK::Receipt receipt;
			receipt.Init();
			receipt.Copy(eReceipt);
			receipts.push_back(receipt);

#if ENABLE_VERBOSE_LOGGING
			IwTrace(DEFAULT, (receipt.Identifier.c_str()));
#endif
		}
		Application::s_ui.m_callbacksRequestReceipts->OnSuccess(receipts);
	}
	else
	{
		Application::s_ui.m_callbacksRequestReceipts->OnFailure(-1, "Success event is null");
	}
}

void RequestReceiptsOnFailure(s3eRequestReceiptsFailureEvent* event)
{
#if ENABLE_VERBOSE_LOGGING
	IwTrace(DEFAULT, ("void RequestReceiptsOnFailure(event)"));
#endif
	if (event)
	{
		Application::s_ui.m_callbacksRequestReceipts->OnFailure(event->m_errorCode, event->m_errorMessage);
	}
	else
	{
		Application::s_ui.m_callbacksRequestReceipts->OnFailure(-1, "Failure event is null");
	}
}

void RequestReceiptsOnCancel(s3eRequestReceiptsCancelEvent* event)
{
#if ENABLE_VERBOSE_LOGGING
	IwTrace(DEFAULT, ("void RequestReceiptsOnCancel(event)"));
#endif
	if (event)
	{
		Application::s_ui.m_callbacksRequestReceipts->OnCancel();
	}
	else
	{
		Application::s_ui.m_callbacksRequestReceipts->OnFailure(-1, "Cancel event is null");
	}
}

s3eCallback ApplicationCallbacksRequestReceipts::GetSuccessEvent()
{
	return (s3eCallback)RequestReceiptsOnSuccess;
}

s3eCallback ApplicationCallbacksRequestReceipts::GetFailureEvent()
{
	return (s3eCallback)RequestReceiptsOnFailure;
}

s3eCallback ApplicationCallbacksRequestReceipts::GetCancelEvent()
{
	return (s3eCallback)RequestReceiptsOnCancel;
}

void ApplicationCallbacksRequestReceipts::OnSuccess(const std::vector<RazerSDK::Receipt>& receipts)
{
#if ENABLE_VERBOSE_LOGGING
	IwTrace(DEFAULT, ("OnSuccess"));
#endif
	
	Application::s_ui.SetMessage("ApplicationCallbacksRequestReceipts::OnSuccess");

	Application::s_ui.ClearReceipts();

	for (unsigned int index = 0; index < receipts.size(); ++index)
	{
		Application::s_ui.AddReceipt(receipts[index]);
	}
}

void ApplicationCallbacksRequestReceipts::OnFailure(int errorCode, const std::string& errorMessage)
{
#if ENABLE_VERBOSE_LOGGING
	char buffer[256];
	sprintf(buffer, "OnFailure errorCode=%d errorMessage=%s", errorCode, errorMessage.c_str());
	IwTrace(DEFAULT, (buffer));
#endif
	
	std::string msg = "ApplicationCallbacksRequestReceipts::OnFailure";
	msg.append(" errorMessage=");
	msg.append(errorMessage);
	Application::s_ui.SetMessage(msg);
}

void ApplicationCallbacksRequestReceipts::OnCancel()
{
#if ENABLE_VERBOSE_LOGGING
	IwTrace(DEFAULT, ("OnCancel"));
#endif
	
	Application::s_ui.SetMessage("ApplicationCallbacksRequestReceipts::OnCancel");
}