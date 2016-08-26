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

#pragma once

#include <string>

namespace org_json_JSONObject
{
	class JSONObject;
}

class _jclass;
typedef _jclass* jclass;

class _jmethodID;
typedef _jmethodID* jmethodID;

class _jobject;
typedef _jobject* jobject;

namespace org_json_JSONArray
{
	class JSONArray
	{
	public:
		static int InitJNI();
		static void FindJNI();
		JSONArray();
		JSONArray(jobject jsonArray);
		JSONArray(const std::string& json);
		jobject GetInstance() const;
		int length() const;
		org_json_JSONObject::JSONObject getJSONObject(int index) const;
		int getInt(int index) const;
		std::string getString(int index) const;
		JSONArray put(int index, const std::string& value) const;
		JSONArray put(int index, const org_json_JSONObject::JSONObject& jsonObject) const;
		std::string toString() const;

	private:
		static jclass _jcJsonArray;
		static jmethodID _mConstruct;
		static jmethodID _mConstruct2;
		static jmethodID _mGetInt;
		static jmethodID _mGetJsonObject;
		static jmethodID _mGetString;
		static jmethodID _mLength;
		static jmethodID _mPut;
		static jmethodID _mToString;
		jobject _instance;
	};
}
