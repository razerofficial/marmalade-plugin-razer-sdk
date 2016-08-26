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

#include "RazerSDK_ExtensionProduct.h"
#include "RazerSDK_JSONObject.h"

#include <string>

using namespace std;

namespace RazerSDK
{
	void ExtensionProduct::Init()
	{
		memset(CurrencyCode, 0, 256);
		memset(Description, 0, 256);
		memset(DeveloperName, 0, 256);
		memset(Identifier, 0, 256);
		LocalPrice = 0;
		memset(Name, 0, 256);
		OriginalPrice = 0;
		PriceInCents = 0;
		PriceInCents = 0;
		memset(ProductType, 0, 256);
	}

	void ExtensionProduct::Copy(const Product& rhs)
	{
		Init();
		sprintf(CurrencyCode, "%s", rhs.CurrencyCode.c_str());
		sprintf(Description, "%s", rhs.Description.c_str());
		sprintf(DeveloperName, "%s", rhs.DeveloperName.c_str());
		sprintf(Identifier, "%s", rhs.Identifier.c_str());
		LocalPrice = rhs.LocalPrice;
		sprintf(Name, "%s", rhs.Name.c_str());
		OriginalPrice = rhs.OriginalPrice;
		PercentOff = rhs.PercentOff;
		PriceInCents = rhs.PriceInCents;
		sprintf(ProductType, "%s", rhs.ProductType.c_str());
	}

	void Product::Init()
	{
		CurrencyCode = "";
		Description = "";
		DeveloperName = "";
		Identifier = "";
		LocalPrice = 0;
		Name = "";
		OriginalPrice = 0;
		PercentOff = 0;
		PriceInCents = 0;
		ProductType = "";
	}

	void Product::Copy(const ExtensionProduct& rhs)
	{
		Init();
		CurrencyCode = string(rhs.CurrencyCode);
		Description = string(rhs.Description);
		DeveloperName = string(rhs.DeveloperName);
		Identifier = string(rhs.Identifier);
		LocalPrice = rhs.LocalPrice;
		Name = string(rhs.Name);
		OriginalPrice = rhs.OriginalPrice;
		PercentOff = rhs.PercentOff;
		PriceInCents = rhs.PriceInCents;
		ProductType = string(rhs.ProductType);
	}

#if defined(ANDROID)
	void Product::ParseJSON(const org_json_JSONObject::JSONObject& jsonObject)
	{
		Init();

		std::string field;

		field = "currencyCode";
		if (jsonObject.has(field))
		{
			CurrencyCode = jsonObject.getString(field);
		}

		field = "description";
		if (jsonObject.has(field))
		{
			Description = jsonObject.getString(field);
		}

		field = "developerName";
		if (jsonObject.has(field))
		{
			DeveloperName = jsonObject.getString(field);
		}

		field = "identifier";
		if (jsonObject.has(field))
		{
			Identifier = jsonObject.getString(field);
		}

		field = "localPrice";
		if (jsonObject.has(field))
		{
			LocalPrice = jsonObject.getDouble(field);
		}

		field = "name";
		if (jsonObject.has(field))
		{
			Name = jsonObject.getString(field);
		}

		field = "originalPrice";
		if (jsonObject.has(field))
		{
			OriginalPrice = jsonObject.getDouble(field);
		}

		field = "percentOff";
		if (jsonObject.has(field))
		{
			PercentOff = jsonObject.getDouble(field);
		}
		
		field = "priceInCents";
		if (jsonObject.has(field))
		{
			PriceInCents = jsonObject.getInt(field);
		}

		field = "productType";
		if (jsonObject.has(field))
		{
			ProductType = jsonObject.getString(field);
		}
	}
#endif
}