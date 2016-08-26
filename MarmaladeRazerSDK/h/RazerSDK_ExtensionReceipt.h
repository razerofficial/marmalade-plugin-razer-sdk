/*
 * Copyright (C) 2012, 2013 OUYA, Inc.
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

#include "RazerSDK_JsonObject.h"

#include <string>

namespace RazerSDK
{
	class Receipt;
	class ExtensionReceipt;

	class Receipt
	{
	public:
		std::string Currency;
		std::string GamerUuid;
		std::string GeneratedDate;
		std::string Identifier;
		float LocalPrice;
		int PriceInCents;
		std::string PurchaseDate;
		std::string Uuid;

		void Init();
		void Copy(const ExtensionReceipt& rhs);
#if defined(ANDROID)
		void ParseJSON(const org_json_JSONObject::JSONObject& jsonObject);
#endif
	};

	// for passing object from extension to application
	class ExtensionReceipt
	{
	public:
		char Currency[256];
		char GamerUuid[256];
		char GeneratedDate[256];
		char Identifier[256];
		float LocalPrice;
		int PriceInCents;
		char PurchaseDate[256];
		char Uuid[256];

		void Init();
		void Copy(const Receipt& rhs);
	};
}
