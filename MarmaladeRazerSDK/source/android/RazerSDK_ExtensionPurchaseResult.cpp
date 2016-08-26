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

#include "RazerSDK_ExtensionPurchaseResult.h"
#include "RazerSDK_JSONObject.h"

#include <string>

using namespace std;

namespace RazerSDK
{
	void ExtensionPurchaseResult::Init()
	{
		memset(Identifier, 0, 256);
	}

	void ExtensionPurchaseResult::Copy(const PurchaseResult& rhs)
	{
		Init();
		sprintf(Identifier, "%s", rhs.Identifier.c_str());
	}

	void PurchaseResult::Init()
	{
		Identifier = "";
	}

	void PurchaseResult::Copy(const ExtensionPurchaseResult& rhs)
	{
		Init();
		Identifier = string(rhs.Identifier);
	}

#if defined(ANDROID)
	void PurchaseResult::ParseJSON(const org_json_JSONObject::JSONObject& jsonObject)
	{
		Init();

		std::string field;

		field = "identifier";
		if (jsonObject.has(field))
		{
			Identifier = jsonObject.getString(field);
		}
	}
#endif
}
