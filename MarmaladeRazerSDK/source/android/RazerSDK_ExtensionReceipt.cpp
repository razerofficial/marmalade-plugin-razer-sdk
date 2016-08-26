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

#include "RazerSDK_ExtensionReceipt.h"
#include "RazerSDK_JSONObject.h"

#include <string>

using namespace std;

namespace RazerSDK
{
	void ExtensionReceipt::Init()
	{
		memset(Currency, 0, 256);
		memset(GamerUuid, 0, 256);
		memset(GeneratedDate, 0, 256);
		memset(Identifier, 0, 256);
		LocalPrice = 0;
		PriceInCents = 0;
		memset(PurchaseDate, 0, 256);
		memset(Uuid, 0, 256);
	}

	void ExtensionReceipt::Copy(const Receipt& rhs)
	{
		Init();
		sprintf(Currency, "%s", rhs.Currency.c_str());
		sprintf(GamerUuid, "%s", rhs.GamerUuid.c_str());
		sprintf(GeneratedDate, "%s", rhs.GeneratedDate.c_str());
		sprintf(Identifier, "%s", rhs.Identifier.c_str());
		LocalPrice = rhs.LocalPrice;
		PriceInCents = rhs.PriceInCents;
		sprintf(PurchaseDate, "%s", rhs.PurchaseDate.c_str());
		sprintf(Uuid, "%s", rhs.Uuid.c_str());
	}

	void Receipt::Init()
	{
		Currency = "";
		GamerUuid = "";
		GeneratedDate = "";
		Identifier = "";
		LocalPrice = 0;
		PriceInCents = 0;
		PurchaseDate = "";
		Uuid = "";
	}

	void Receipt::Copy(const ExtensionReceipt& rhs)
	{
		Init();
		Currency = string(rhs.Currency);
		GamerUuid = string(rhs.GamerUuid);
		GeneratedDate = string(rhs.GeneratedDate);
		Identifier = string(rhs.Identifier);
		LocalPrice = rhs.LocalPrice;
		PriceInCents = rhs.PriceInCents;
		PurchaseDate = string(rhs.PurchaseDate);
		Uuid = string(rhs.Uuid);
	}

#if defined(ANDROID)
	void Receipt::ParseJSON(const org_json_JSONObject::JSONObject& jsonObject)
	{
		std::string field;

		field = "currency";
		if (jsonObject.has(field))
		{
			Currency = jsonObject.getString(field);
		}
		
		field = "gamerUuid";
		if (jsonObject.has(field))
		{
			GamerUuid = jsonObject.getString(field);
		}
		
		field = "generatedDate";
		if (jsonObject.has(field))
		{
			GeneratedDate = jsonObject.getString(field);
		}
		
		field = "identifier";
		if (jsonObject.has(field))
		{
			Identifier = jsonObject.getString(field);
		}

		field = "localPrice";
		if (jsonObject.has(field))
		{
			LocalPrice = (float)jsonObject.getDouble(field);
		}
		
		field = "priceInCents";
		if (jsonObject.has(field))
		{
			PriceInCents = jsonObject.getInt(field);
		}

		field = "purchaseDate";
		if (jsonObject.has(field))
		{
			PurchaseDate = jsonObject.getString(field);
		}

		field = "uuid";
		if (jsonObject.has(field))
		{
			Uuid = jsonObject.getString(field);
		}
	}
#endif
}