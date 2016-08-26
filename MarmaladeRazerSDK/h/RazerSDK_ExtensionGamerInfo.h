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
	class GamerInfo;
	class ExtensionGamerInfo;

	class GamerInfo
	{
	public:
		std::string Username;
		std::string Uuid;

		void Init();
		void Copy(const ExtensionGamerInfo& rhs);
#if defined(ANDROID)
		void ParseJSON(const org_json_JSONObject::JSONObject& jsonObject);
#endif
	};

	// for passing object from extension to application
	class ExtensionGamerInfo
	{
	public:
		char Username[256];
		char Uuid[256];

		void Init();
		void Copy(const GamerInfo& rhs);
	};
}
