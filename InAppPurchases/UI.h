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

#define TEXT_MAX_WIDTH 1920
#define TEXT_MAX_HEIGHT 20

#include "RazerSDK_ExtensionProduct.h"
#include "RazerSDK_ExtensionPurchaseResult.h"
#include "RazerSDK_ExtensionReceipt.h"
#include "TextButton.h"
#include "TextLabel.h"

#include <string>
#include <vector>

#include "IwGxFontTypes.h"

class ApplicationCallbacksInitPlugin;
class ApplicationCallbacksRequestLogin;
class ApplicationCallbacksRequestGamerInfo;
class ApplicationCallbacksRequestProducts;
class ApplicationCallbacksRequestPurchase;
class ApplicationCallbacksRequestReceipts;
class ApplicationCallbacksShutdown;

class CIw2DImage;
class CIwGxFont;
class s3ePointerEvent;
class s3ePointerMotionEvent;

class UI
{
public:
	
	UI();

	static CIwGxFont* GetFont();
	static void SetFont(CIwGxFont* font);

	static CIw2DImage* GetActiveButton();
	static void SetActiveButton(CIw2DImage* image);

	static CIw2DImage* GetInactiveButton();
	static void SetInactiveButton(CIw2DImage* image);

	bool InitUI();

	void Destroy();

	bool HasUIChanged();

	static void DrawText(const std::string& text, int x, int y, int width, int height, IwGxFontAlignHor hAlign, IwGxFontAlignVer vAlign);

	static void DrawText(const std::string& text, int x, int y, int width, int height);
	
	void Render();

	static int32 PointerButtonEventCallback(s3ePointerEvent* pEvent, void* pUserData);
	static int32 PointerMotionEventCallback(s3ePointerMotionEvent* pEvent, void* pUserData);

	void DoRequestLogin();
	void DoRequestGamerInfo();
	void DoRequestProducts();
	void DoRequestPurchase();
	void DoRequestReceipts();
	void DoShutdown();

	void SetUsername(const std::string& username);

	void SetGamerUUID(const std::string& uuid);

	void SetMessage(const std::string& message);

	void ClearProducts();

	void ClearReceipts();

	void AddProduct(const RazerSDK::Product& product);

	void AddReceipt(const RazerSDK::Receipt& receipt);

	void Exit();

	ApplicationCallbacksInitPlugin* m_callbacksInitPlugin;
	ApplicationCallbacksRequestLogin* m_callbacksRequestLogin;
	ApplicationCallbacksRequestGamerInfo* m_callbacksRequestGamerInfo;
	ApplicationCallbacksRequestProducts* m_callbacksRequestProducts;
	ApplicationCallbacksRequestPurchase* m_callbacksRequestPurchase;
	ApplicationCallbacksRequestReceipts* m_callbacksRequestReceipts;
	ApplicationCallbacksShutdown* m_callbacksShutdown;

private:

	static UI* s_instance;

	static CIwGxFont* s_font;

	static CIw2DImage* s_activeButton;
	static CIw2DImage* s_inactiveButton;

	std::vector<std::string> m_productIds;

	std::vector<RazerSDK::Product> m_pendingProducts;
	std::vector<TextButton*> m_products;

	std::vector<RazerSDK::Receipt> m_pendingReceipts;
	std::vector<TextLabel*> m_receipts;

	RazerSDK::Product* m_selectedProduct;

	bool m_uiInitialized;

	bool m_uiChanged;

	TextButton m_uiRequestLogin;
	TextButton m_uiRequestGamerInfo;
	TextButton m_uiRequestProducts;
	TextButton m_uiRequestReceipts;
	TextButton m_uiShutdown;
	
	TextLabel m_uiLabelUsername;
	TextLabel m_uiLabelGamerUuid;
	TextLabel m_uiLabelMessage;

	void RenderThreadInitProducts();
	void RenderThreadInitReceipts();
};
