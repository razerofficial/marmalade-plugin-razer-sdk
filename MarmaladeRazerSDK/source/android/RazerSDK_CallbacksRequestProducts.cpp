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

#include "RazerSDK_CallbacksRequestProducts.h"
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

RazerSDK::ExtensionProduct* CallbacksRequestProducts::m_products = NULL;

void CallbacksRequestProducts::RegisterCallback(s3eCallback callback, s3eCallback* savedCallback, int callbackType)
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

void CallbacksRequestProducts::RegisterCallbacks(s3eCallback onSuccess, s3eCallback onFailure, s3eCallback onCancel)
{
	RegisterCallback(onSuccess, &m_onSuccess, S3E_RAZERSDK_CALLBACKS_REQUEST_PRODUCTS_ON_SUCCESS);
	RegisterCallback(onFailure, &m_onFailure, S3E_RAZERSDK_CALLBACKS_REQUEST_PRODUCTS_ON_FAILURE);
	RegisterCallback(onCancel, &m_onCancel, S3E_RAZERSDK_CALLBACKS_REQUEST_PRODUCTS_ON_CANCEL);
}

void CallbacksRequestProducts::OnSuccess(const std::vector<RazerSDK::Product>& products)
{
	//IwTrace(RazerSDK, ("OnSuccess"));

	if (m_products)
	{
		//IwTrace(RazerSDK, ("Cleaning old products"));
		delete [] m_products;
		m_products = NULL;
	}

	if (products.size() > 0)
	{
		//IwTrace(RazerSDK, ("Allocating products"));
		m_products = new RazerSDK::ExtensionProduct[products.size()];
		//IwTrace(RazerSDK, ("Copying products"));
		for (unsigned int index = 0; index < products.size(); ++index)
		{
			RazerSDK::Product product = products[index];
			RazerSDK::ExtensionProduct eProduct;
			eProduct.Init();
			eProduct.Copy(product);
			m_products[index] = eProduct;
		}
	}
#if ENABLE_VERBOSE_LOGGING
	else
	{
		IwTrace(RazerSDK, ("No products to copy"));
	}
#endif

	s3eRequestProductsSuccessEvent event;
	event.m_products = m_products;
	event.m_productsLength = products.size();

	IwTrace(RazerSDK, ("Invoking callback"));

	m_dataRequestProductsSuccessEvent = event; //don't send a temp pointer
	s3eEdkCallbacksEnqueue(S3E_EXT_RAZERSDK_HASH, S3E_RAZERSDK_CALLBACKS_REQUEST_PRODUCTS_ON_SUCCESS, &m_dataRequestProductsSuccessEvent);
}

void CallbacksRequestProducts::OnFailure(int errorCode, const std::string& errorMessage)
{
#if ENABLE_VERBOSE_LOGGING
	char buffer[256];
	sprintf(buffer, "OnFailure errorCode=%d errorMessage=%s", errorCode, errorMessage.c_str());
	IwTrace(RazerSDK, (buffer));
#endif

	s3eRequestProductsFailureEvent event;
	event.m_errorCode = errorCode;
	sprintf(event.m_errorMessage, "%s", errorMessage.c_str());

	m_dataRequestProductsFailureEvent = event; //don't send a temp pointer
	s3eEdkCallbacksEnqueue(S3E_EXT_RAZERSDK_HASH, S3E_RAZERSDK_CALLBACKS_REQUEST_PRODUCTS_ON_FAILURE, &m_dataRequestProductsFailureEvent);
}

void CallbacksRequestProducts::OnCancel()
{
#if ENABLE_VERBOSE_LOGGING
	IwTrace(RazerSDK, ("OnCancel"));
#endif

	s3eRequestProductsCancelEvent event;

	m_dataRequestProductsCancelEvent = event; //don't send a temp pointer
	s3eEdkCallbacksEnqueue(S3E_EXT_RAZERSDK_HASH, S3E_RAZERSDK_CALLBACKS_REQUEST_PRODUCTS_ON_CANCEL, &m_dataRequestProductsCancelEvent);
}
