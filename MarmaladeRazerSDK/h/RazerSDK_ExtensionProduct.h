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
	class Product;
	class ExtensionProduct;

	class Product
	{
	public:
		std::string CurrencyCode;
		std::string Description;
		std::string DeveloperName;
		std::string Identifier;
		float LocalPrice;
		std::string Name;
		float OriginalPrice;
		float PercentOff;
		int PriceInCents;
		std::string ProductType;

		void Init();
		void Copy(const ExtensionProduct& rhs);
#if defined(ANDROID)
		void ParseJSON(const org_json_JSONObject::JSONObject& jsonObject);
#endif
	};

	// for passing object from extension to application
	class ExtensionProduct
	{
	public:
		char CurrencyCode[256];
		char Description[256];
		char DeveloperName[256];
		char Identifier[256];
		float LocalPrice;
		char Name[256];
		float OriginalPrice;
		float PercentOff;
		int PriceInCents;
		char ProductType[256];

		void Init();
		void Copy(const Product& rhs);
	};
}
