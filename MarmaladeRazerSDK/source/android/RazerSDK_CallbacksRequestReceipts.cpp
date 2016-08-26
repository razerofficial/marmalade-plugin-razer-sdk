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

#include "RazerSDK_CallbacksRequestReceipts.h"
#include "RazerSDK_CallbackSingleton.h"

#include "RazerSDK_internal.h"
#include "s3eEdk.h"
#include "s3eEdk_android.h"
#include "IwDebug.h"

#include <stdio.h>

#ifdef ENABLE_VERBOSE_LOGGING
#undef ENABLE_VERBOSE_LOGGING
#endif
#define ENABLE_VERBOSE_LOGGING false

RazerSDK::ExtensionReceipt* CallbacksRequestReceipts::m_receipts = NULL;

void CallbacksRequestReceipts::RegisterCallback(s3eCallback callback, s3eCallback* savedCallback, int callbackType)
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

void CallbacksRequestReceipts::RegisterCallbacks(s3eCallback onSuccess, s3eCallback onFailure, s3eCallback onCancel)
{
	RegisterCallback(onSuccess, &m_onSuccess, S3E_RAZERSDK_CALLBACKS_REQUEST_RECEIPTS_ON_SUCCESS);
	RegisterCallback(onFailure, &m_onFailure, S3E_RAZERSDK_CALLBACKS_REQUEST_RECEIPTS_ON_FAILURE);
	RegisterCallback(onCancel, &m_onCancel, S3E_RAZERSDK_CALLBACKS_REQUEST_RECEIPTS_ON_CANCEL);
}

void CallbacksRequestReceipts::OnSuccess(const std::vector<RazerSDK::Receipt>& receipts)
{
#if ENABLE_VERBOSE_LOGGING
	IwTrace(RazerSDK, ("OnSuccess"));
#endif

	if (receipts.size() > 0)
	{
		IwTrace(RazerSDK, ("Allocating receipts"));
		m_receipts = new RazerSDK::ExtensionReceipt[receipts.size()];
		IwTrace(RazerSDK, ("Copying receipts"));
		for (unsigned int index = 0; index < receipts.size(); ++index)
		{
			RazerSDK::Receipt receipt = receipts[index];
			RazerSDK::ExtensionReceipt eReceipt;
			eReceipt.Init();
			eReceipt.Copy(receipt);
			m_receipts[index] = eReceipt;
		}
	}
#if ENABLE_VERBOSE_LOGGING
	else
	{
		IwTrace(RazerSDK, ("No receipts to copy"));
	}
#endif

	s3eRequestReceiptsSuccessEvent event;
	event.m_receipts = m_receipts;
	event.m_receiptsLength = receipts.size();

	IwTrace(RazerSDK, ("Invoking callback"));

	m_dataRequestReceiptsSuccessEvent = event; //don't send a temp pointer
	s3eEdkCallbacksEnqueue(S3E_EXT_RAZERSDK_HASH, S3E_RAZERSDK_CALLBACKS_REQUEST_RECEIPTS_ON_SUCCESS, &m_dataRequestReceiptsSuccessEvent);
}

void CallbacksRequestReceipts::OnFailure(int errorCode, const std::string& errorMessage)
{
#if ENABLE_VERBOSE_LOGGING
	char buffer[256];
	sprintf(buffer, "OnFailure errorCode=%d errorMessage=%s", errorCode, errorMessage.c_str());
	IwTrace(RAZERSDK, (buffer));
#endif

	s3eRequestReceiptsFailureEvent event;
	event.m_errorCode = errorCode;
	sprintf(event.m_errorMessage, "%s", errorMessage.c_str());

	m_dataRequestReceiptsFailureEvent = event; //don't send a temp pointer
	s3eEdkCallbacksEnqueue(S3E_EXT_RAZERSDK_HASH, S3E_RAZERSDK_CALLBACKS_REQUEST_RECEIPTS_ON_FAILURE, &m_dataRequestReceiptsFailureEvent);
}

void CallbacksRequestReceipts::OnCancel()
{
#if ENABLE_VERBOSE_LOGGING
	IwTrace(RazerSDK, ("OnCancel"));
#endif

	s3eRequestReceiptsCancelEvent event;

	m_dataRequestReceiptsCancelEvent = event; //don't send a temp pointer
	s3eEdkCallbacksEnqueue(S3E_EXT_RAZERSDK_HASH, S3E_RAZERSDK_CALLBACKS_REQUEST_RECEIPTS_ON_CANCEL, &m_dataRequestReceiptsCancelEvent);
}
