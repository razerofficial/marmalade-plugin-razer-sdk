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

#include "RazerSDK_CallbacksRequestPurchase.h"
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

RazerSDK::ExtensionPurchaseResult CallbacksRequestPurchase::m_purchaseResult = RazerSDK::ExtensionPurchaseResult();

void CallbacksRequestPurchase::RegisterCallback(s3eCallback callback, s3eCallback* savedCallback, int callbackType)
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

void CallbacksRequestPurchase::RegisterCallbacks(s3eCallback onSuccess, s3eCallback onFailure, s3eCallback onCancel)
{
	RegisterCallback(onSuccess, &m_onSuccess, S3E_RAZERSDK_CALLBACKS_REQUEST_PURCHASE_ON_SUCCESS);
	RegisterCallback(onFailure, &m_onFailure, S3E_RAZERSDK_CALLBACKS_REQUEST_PURCHASE_ON_FAILURE);
	RegisterCallback(onCancel, &m_onCancel, S3E_RAZERSDK_CALLBACKS_REQUEST_PURCHASE_ON_CANCEL);
}

void CallbacksRequestPurchase::OnSuccess(const RazerSDK::PurchaseResult& purchaseResult)
{
#if ENABLE_VERBOSE_LOGGING
	IwTrace(RazerSDK, ("OnSuccess"));
#endif

	RazerSDK::ExtensionPurchaseResult ePurchaseResult;
	ePurchaseResult.Init();
	ePurchaseResult.Copy(purchaseResult);
	m_purchaseResult = ePurchaseResult;

	s3eRequestPurchaseSuccessEvent event;
	event.m_purchaseResult = m_purchaseResult;

	m_dataRequestPurchaseSuccessEvent = event; //don't send a temp pointer
	s3eEdkCallbacksEnqueue(S3E_EXT_RAZERSDK_HASH, S3E_RAZERSDK_CALLBACKS_REQUEST_PURCHASE_ON_SUCCESS, &m_dataRequestPurchaseSuccessEvent);
}

void CallbacksRequestPurchase::OnFailure(int errorCode, const std::string& errorMessage)
{
#if ENABLE_VERBOSE_LOGGING
	char buffer[256];
	sprintf(buffer, "OnFailure errorCode=%d errorMessage=%s", errorCode, errorMessage.c_str());
	IwTrace(RazerSDK, (buffer));
#endif

	s3eRequestPurchaseFailureEvent event;
	event.m_errorCode = errorCode;
	sprintf(event.m_errorMessage, "%s", errorMessage.c_str());

	m_dataRequestPurchaseFailureEvent = event; //don't send a temp pointer
	s3eEdkCallbacksEnqueue(S3E_EXT_RAZERSDK_HASH, S3E_RAZERSDK_CALLBACKS_REQUEST_PURCHASE_ON_FAILURE, &m_dataRequestPurchaseFailureEvent);
}

void CallbacksRequestPurchase::OnCancel()
{
#if ENABLE_VERBOSE_LOGGING
	IwTrace(RazerSDK, ("OnCancel"));
#endif

	s3eRequestPurchaseCancelEvent event;

	m_dataRequestPurchaseCancelEvent = event; //don't send a temp pointer
	s3eEdkCallbacksEnqueue(S3E_EXT_RAZERSDK_HASH, S3E_RAZERSDK_CALLBACKS_REQUEST_PURCHASE_ON_CANCEL, &m_dataRequestPurchaseCancelEvent);
}
