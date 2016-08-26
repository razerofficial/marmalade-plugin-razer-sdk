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
#include "RazerSDK.h"

#include "IwDebug.h"

UI Application::s_ui = UI();

void Application::InitPlugin()
{
	Application::s_ui.SetMessage("Initialize RazerSDK Plugin...");

	std::string secretApiKey = "eyJkZXZlbG9wZXJfaWQiOiIzMTBhOGY1MS00ZDZlLTRhZTUtYmRhMC1iOTM4";
	secretApiKey += "NzhlNWY1ZDAiLCJkZXZlbG9wZXJfcHVibGljX2tleSI6Ik1JR2ZNQTBHQ1Nx";
	secretApiKey += "R1NJYjNEUUVCQVFVQUE0R05BRENCaVFLQmdRQ3A2VENMYzg1cHBGdnY5U1RU";
	secretApiKey += "Q3pYYk9aYVlDczFDNEdrcHNQMEQwaGs4dkoyZ1pqVjJhNUUwMHVTeVBYN1FO";
	secretApiKey += "cytWMlJIUTVLYkRJYUk3eFArS2JsdkZQK2FzZmdNM0M1aFR1c1F6TUxpVmFX";
	secretApiKey += "R1FVYzBFamF5bzNKZGdSN3lVSE9oWkhMU3dCZ2ZGckxVMU5za3pVdzVFQ3ZU";
	secretApiKey += "TGRDSk85bjdDNHhCcnBkZ25LUUlEQVFBQiJ9";

	Plugin_initPlugin(secretApiKey.c_str(),
		Application::s_ui.m_callbacksInitPlugin->GetSuccessEvent(),
		Application::s_ui.m_callbacksInitPlugin->GetFailureEvent());
}
