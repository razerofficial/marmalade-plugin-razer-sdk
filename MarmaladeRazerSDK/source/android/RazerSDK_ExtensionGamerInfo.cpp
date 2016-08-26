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

#include "RazerSDK_ExtensionGamerInfo.h"
#include "RazerSDK_JSONObject.h"

#include <string>

using namespace std;

namespace RazerSDK
{
	void ExtensionGamerInfo::Init()
	{
		memset(Username, 0, 256);
		memset(Uuid, 0, 256);
	}

	void ExtensionGamerInfo::Copy(const RazerSDK::GamerInfo& rhs)
	{
		Init();
		sprintf(Username, "%s", rhs.Username.c_str());
		sprintf(Uuid, "%s", rhs.Uuid.c_str());
	}

	void GamerInfo::Init()
	{
		Username = "";
		Uuid = "";
	}

	void GamerInfo::Copy(const RazerSDK::ExtensionGamerInfo& rhs)
	{
		Init();
		Username = string(rhs.Username);
		Uuid = string(rhs.Uuid);
	}

#if defined(ANDROID)
	void GamerInfo::ParseJSON(const org_json_JSONObject::JSONObject& jsonObject)
	{
		Init();

		std::string field;

		field = "username";
		if (jsonObject.has(field))
		{
			Username = jsonObject.getString(field);
		}

		field = "uuid";
		if (jsonObject.has(field))
		{
			Uuid = jsonObject.getString(field);
		}
	}
#endif
}