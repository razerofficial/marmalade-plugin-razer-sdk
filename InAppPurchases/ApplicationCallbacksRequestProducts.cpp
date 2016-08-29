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
#include "ApplicationCallbacksRequestProducts.h"
#include "RazerSDK_CallbackSingleton.h"
#include "RazerSDK_CallbacksRequestProducts.h"
#include "RazerSDK_ExtensionProduct.h"

#include "IwDebug.h"

#include <stdio.h>

using namespace RazerSDK;
using namespace std;

void RequestProductsOnSuccess(s3eRequestProductsSuccessEvent* event)
{
#if ENABLE_VERBOSE_LOGGING
	IwTrace(DEFAULT, ("void RequestProductsOnSuccess(event)"));
#endif
	if (event)
	{
		vector<Product> products;
		for (int index = 0; index < event->m_productsLength; ++index)
		{
			ExtensionProduct eProduct = event->m_products[index];
			Product product;
			product.Copy(eProduct);
			products.push_back(product);

#if ENABLE_VERBOSE_LOGGING
			IwTrace(DEFAULT, (product.Name.c_str()));
#endif
		}
		Application::s_ui.m_callbacksRequestProducts->OnSuccess(products);
	}
	else
	{
		Application::s_ui.m_callbacksRequestProducts->OnFailure(-1, "Success event is null");
	}
}

void RequestProductsOnFailure(s3eRequestProductsFailureEvent* event)
{
#if ENABLE_VERBOSE_LOGGING
	IwTrace(DEFAULT, ("void RequestProductsOnFailure(event)"));
#endif
	if (event)
	{
		Application::s_ui.m_callbacksRequestProducts->OnFailure(event->m_errorCode, event->m_errorMessage);
	}
	else
	{
		Application::s_ui.m_callbacksRequestProducts->OnFailure(-1, "Failure event is null");
	}
}

void RequestProductsOnCancel(s3eRequestProductsCancelEvent* event)
{
#if ENABLE_VERBOSE_LOGGING
	IwTrace(DEFAULT, ("void RequestProductsOnCancel(event)"));
#endif
	if (event)
	{
		Application::s_ui.m_callbacksRequestProducts->OnCancel();
	}
	else
	{
		Application::s_ui.m_callbacksRequestProducts->OnFailure(-1, "Cancel event is null");
	}
}

s3eCallback ApplicationCallbacksRequestProducts::GetSuccessEvent()
{
	return (s3eCallback)RequestProductsOnSuccess;
}

s3eCallback ApplicationCallbacksRequestProducts::GetFailureEvent()
{
	return (s3eCallback)RequestProductsOnFailure;
}

s3eCallback ApplicationCallbacksRequestProducts::GetCancelEvent()
{
	return (s3eCallback)RequestProductsOnCancel;
}

void ApplicationCallbacksRequestProducts::OnSuccess(const vector<Product>& products)
{
#if ENABLE_VERBOSE_LOGGING
	IwTrace(DEFAULT, ("OnSuccess"));
#endif
	
	Application::s_ui.SetMessage("ApplicationCallbacksRequestProducts::OnSuccess");

	Application::s_ui.ClearProducts();

	for (unsigned int index = 0; index < products.size(); index++)
	{
		Application::s_ui.AddProduct(products[index]);
	}
}

void ApplicationCallbacksRequestProducts::OnFailure(int errorCode, const string& errorMessage)
{
#if ENABLE_VERBOSE_LOGGING
	char buffer[256];
	sprintf(buffer, "OnFailure errorCode=%d errorMessage=%s", errorCode, errorMessage.c_str());
	IwTrace(DEFAULT, (buffer));
#endif
	
	string msg = "ApplicationCallbacksRequestProducts::OnFailure";
	msg.append(" errorMessage=");
	msg.append(errorMessage);
	Application::s_ui.SetMessage(msg);
}

void ApplicationCallbacksRequestProducts::OnCancel()
{
#if ENABLE_VERBOSE_LOGGING
	IwTrace(DEFAULT, ("OnCancel"));
#endif
	
	Application::s_ui.SetMessage("ApplicationCallbacksRequestProducts::OnCancel");
}