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

#include "RazerSDK_Bitmap.h"
#include <string>
#include <vector>

class CallbacksInitPlugin;
class CallbacksRequestGamerInfo;
class CallbacksRequestProducts;
class CallbacksRequestPurchase;
class CallbacksRequestReceipts;
class CallbacksShutdown;

class _jclass;
typedef struct _JNIEnv JNIEnv;
typedef _jclass* jclass;

namespace RazerSDK
{
	class Plugin
	{
	public:

		static int InitJNI();

		void SetApp(struct android_app* app);

		static void initPlugin(const std::string& secretApiKey, CallbacksInitPlugin* callbacks);

		static void requestGamerInfo(CallbacksRequestGamerInfo* callbacks);

		static void requestProducts(const std::vector<std::string>& products, CallbacksRequestProducts* callbacks);

		static void requestPurchase(const std::string& purchasable, const std::string& productType, CallbacksRequestPurchase* callbacks);

		static void requestReceipts(CallbacksRequestReceipts* callbacks);

		static void updateSafeArea(float amount);

		static void shutdown(CallbacksShutdown* callbacks);

		static void quit();

		static float getFloat(jobject fFloat);

		static std::vector<android_graphics_Bitmap::Bitmap> getBitmapArray(jobject listBitmaps);

		static std::vector<std::string> getStringArray(jobject listStrings);
	private:

		// cached references
		static jclass jc_Plugin;
	};
}
