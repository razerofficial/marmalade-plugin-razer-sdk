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

#pragma once

#include "RazerSDK_ExtensionPurchaseResult.h"

#include "s3eTypes.h"

#include <string>

typedef struct s3eRequestPurchaseSuccessEvent
{
	RazerSDK::ExtensionPurchaseResult m_purchaseResult;
} s3eRequestPurchaseSuccessEvent;

typedef struct s3eRequestPurchaseFailureEvent
{
	int m_errorCode;
	char m_errorMessage[1024];
} s3eRequestPurchaseFailureEvent;

typedef struct s3eRequestPurchaseCancelEvent
{
} s3eRequestPurchaseCancelEvent;

class CallbacksRequestPurchase
{
public:

	void RegisterCallbacks(s3eCallback onSuccess, s3eCallback onFailure, s3eCallback onCancel);

	void OnSuccess(const RazerSDK::PurchaseResult& purchaseResult);

	void OnFailure(int errorCode, const std::string& errorMessage);

	void OnCancel();

private:
	//extension callbacks
	s3eCallback m_onSuccess;
	s3eCallback m_onFailure;
	s3eCallback m_onCancel;

	//event data sent to callbacks can't be temporary
	s3eRequestPurchaseSuccessEvent m_dataRequestPurchaseSuccessEvent;
	s3eRequestPurchaseFailureEvent m_dataRequestPurchaseFailureEvent;
	s3eRequestPurchaseCancelEvent m_dataRequestPurchaseCancelEvent;

	void RegisterCallback(s3eCallback callback, s3eCallback* savedCallback, int callbackType);

	//cache the product object
	static RazerSDK::ExtensionPurchaseResult m_purchaseResult;
};
