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

#include "Application.h"
#include "ApplicationCallbacksInitPlugin.h"
#include "ApplicationCallbacksRequestGamerInfo.h"
#include "ApplicationCallbacksRequestProducts.h"
#include "ApplicationCallbacksRequestPurchase.h"
#include "ApplicationCallbacksRequestReceipts.h"
#include "ApplicationCallbacksShutdown.h"
#include "RazerSDK_CallbackSingleton.h"
#include "RazerSDK_Controller.h"
#include "RazerSDK.h"
#include "RazerSDK_ExtensionProduct.h"
#include "RazerSDK_ExtensionPurchaseResult.h"
#include "RazerSDK_ExtensionReceipt.h"
#include "UI.h"

#include <algorithm>
#include <map>
#include <sstream>
#include <string>

#include <EGL/egl.h>
#include <EGL/eglplatform.h>
#include <GLES2/gl2.h>

#include "s3e.h"
#include "Iw2D.h"
#include "IwDebug.h"
#include "IwGx.h"
#include "IwGxFont.h"

using namespace com_razerzone_store_sdk_Controller;
using namespace RazerSDK;
using namespace std;

UI* UI::s_instance = NULL;
CIwGxFont* UI::s_font = NULL;
CIw2DImage* UI::s_activeButton = NULL;
CIw2DImage* UI::s_inactiveButton = NULL;

UI::UI()
{
	s_instance = this;

	m_uiInitialized = false;

	m_callbacksInitPlugin = new ApplicationCallbacksInitPlugin();
	m_callbacksRequestGamerInfo = new ApplicationCallbacksRequestGamerInfo();
	m_callbacksRequestProducts = new ApplicationCallbacksRequestProducts();
	m_callbacksRequestPurchase = new ApplicationCallbacksRequestPurchase();
	m_callbacksRequestReceipts = new ApplicationCallbacksRequestReceipts();
	m_callbacksShutdown = new ApplicationCallbacksShutdown();

	m_productIds.push_back("long_sword");
	m_productIds.push_back("sharp_axe");
	m_productIds.push_back("cool_level");
	m_productIds.push_back("awesome_sauce");
	m_productIds.push_back("__DECLINED__THIS_PURCHASE");

	m_selectedProduct = NULL;

	m_uiChanged = false;
}

CIwGxFont* UI::GetFont()
{
	return s_font;
}

void UI::SetFont(CIwGxFont* font)
{
	s_font = font;
}

CIw2DImage* UI::GetActiveButton()
{
	return s_activeButton;
}

void UI::SetActiveButton(CIw2DImage* image)
{
	s_activeButton = image;
}

CIw2DImage* UI::GetInactiveButton()
{
	return s_inactiveButton;
}

void UI::SetInactiveButton(CIw2DImage* image)
{
	s_inactiveButton = image;
}

void UI::RenderThreadInitProducts()
{
	// delay creation of new labels for the rendering thread
	if (m_pendingProducts.size() > 0)
	{
		for (unsigned int index = 0; index < m_pendingProducts.size(); ++index)
		{
			char buffer[1024];

			//sprintf(buffer, "Copy product %s", m_pendingProducts[index].Identifier.c_str());
			//LOGI(buffer);

			TextButton* txtProduct = new TextButton();
			Product* newProduct = new Product(m_pendingProducts[index]);
			txtProduct->DataContext = newProduct;

			//sprintf(buffer, "Setting up product ui %s", newProduct->Identifier.c_str());
			//LOGI(buffer);

			sprintf(buffer, "%s (%.2f)", newProduct->Identifier.c_str(), newProduct->LocalPrice);
			txtProduct->SetText(buffer);

			m_products.push_back(txtProduct);
}

		m_uiChanged = true;

		m_pendingProducts.clear();

		const int w = s3eSurfaceGetInt(S3E_SURFACE_WIDTH);
		const int h = s3eSurfaceGetInt(S3E_SURFACE_HEIGHT);

		for (unsigned int index = 0; index < m_products.size(); ++index)
		{
			m_products[index]->SetPosition(200, 500 + index * 125, 1300, 100);
		}
	}
}

void UI::RenderThreadInitReceipts()
{
	// delay creation of new labels for the rendering thread
	if (m_pendingReceipts.size() > 0)
	{
		for (unsigned int index = 0; index < m_pendingReceipts.size(); ++index)
		{
			TextLabel* txtReceipt = new TextLabel("");
			Receipt* newReceipt = new Receipt(m_pendingReceipts[index]);
			txtReceipt->DataContext = newReceipt;

			stringstream buffer;
			buffer << newReceipt->Identifier;
			buffer << " (" << newReceipt->LocalPrice;
			buffer << ")";
			txtReceipt->SetText(buffer.str());

			m_receipts.push_back(txtReceipt);
		}

		m_uiChanged = true;

		m_pendingReceipts.clear();

		const int w = s3eSurfaceGetInt(S3E_SURFACE_WIDTH);
		const int h = s3eSurfaceGetInt(S3E_SURFACE_HEIGHT);

		for (unsigned int index = 0; index < m_receipts.size(); ++index)
		{
			m_receipts[index]->SetPosition(1550, 500 + index * 150, 800, 100);
		}
	}
}

bool UI::InitUI()
{
	if (m_uiInitialized)
	{
		RenderThreadInitProducts();
		RenderThreadInitReceipts();
		return true;
	}

	const int w = s3eSurfaceGetInt(S3E_SURFACE_WIDTH);
	const int h = s3eSurfaceGetInt(S3E_SURFACE_HEIGHT);

	m_uiLabelUsername = TextLabel(200, 200, TEXT_MAX_WIDTH, TEXT_MAX_HEIGHT, "Username:");
	m_uiLabelGamerUuid = TextLabel(1000, 200, TEXT_MAX_WIDTH, TEXT_MAX_HEIGHT, "UUID:");
	m_uiLabelMessage = TextLabel(200, 275, TEXT_MAX_WIDTH, TEXT_MAX_HEIGHT, "Message:");

	int x = 200;
	int y = 350;
	m_uiRequestGamerUUID = TextButton(x, y, 450, 75, "Get GamerUUID");
	
	x += m_uiRequestGamerUUID.GetWidth() + 50;
	m_uiRequestProducts = TextButton(x, y, 350, 75, "Get Products");

	x += m_uiRequestProducts.GetWidth() + 50;
	m_uiRequestReceipts = TextButton(x, y, 400, 75, "Get Receipts");

	x += m_uiRequestReceipts.GetWidth() + 50;
	m_uiShutdown = TextButton(x, y, 350, 75, "Shutdown");

	m_uiInitialized = true;

	// To register the above two callbacks you do this
	s3ePointerRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)UI::PointerButtonEventCallback, NULL);
	s3ePointerRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)UI::PointerMotionEventCallback, NULL);

	return true;
}

void UI::Destroy()
{
	m_uiRequestGamerUUID.Destroy();
	m_uiRequestProducts.Destroy();
	m_uiRequestReceipts.Destroy();
	m_uiShutdown.Destroy();

	ClearProducts();
	ClearReceipts();
}

bool UI::HasUIChanged()
{
	if (m_uiChanged)
	{
		m_uiChanged = false;
		return true;
	}
	return false;
}

void UI::DrawText(const std::string& text, int x, int y, int width, int height, IwGxFontAlignHor hAlign, IwGxFontAlignVer vAlign)
{
	if (UI::GetFont())
	{
		//Flush
		IwGxFlush();

		// Set a pointer to the font to use for rendering
		IwGxFontSetFont(UI::GetFont());

		// Enable emissive lighting and set font colour
		IwGxLightingEmissive(true);
		IwGxFontSetCol(0xffffffff);

		// Specify a bounding rectangle starting at screen position 10, 200 with size 300x100 pixels
		IwGxFontSetRect(CIwRect(x, y, width, height));

		// Set horizontal centring of text
		// Other possible values are IW_GX_FONT_ALIGN_LEFT and IW_GX_FONT_ALIGN_RIGHT
		IwGxFontSetAlignmentHor(hAlign);

		// Set text to be drawn at the top of the bounding rectangle
		// Other possible values are IW_GX_FONT_ALIGN_MIDDLE and IW_GX_FONT_ALIGN_BOTTOM
		IwGxFontSetAlignmentVer(vAlign);

		// Render some text
		IwGxFontDrawText(text.c_str());

		//Flush
		IwGxFlush();
	}
}

void UI::DrawText(const std::string& text, int x, int y, int width, int height)
{
	UI::DrawText(text, x, y, width, height, IW_GX_FONT_ALIGN_LEFT, IW_GX_FONT_ALIGN_TOP);
}

void UI::Render()
{
	m_uiLabelUsername.Render();
	m_uiLabelGamerUuid.Render();
	m_uiLabelMessage.Render();

	m_uiRequestGamerUUID.Render();
	m_uiRequestProducts.Render();
	m_uiRequestReceipts.Render();
	m_uiShutdown.Render();

	for (unsigned int index = 0; index < m_products.size(); ++index)
	{
		m_products[index]->Render();
	}

	for (unsigned int index = 0; index < m_receipts.size(); ++index)
	{
		m_receipts[index]->Render();
	}
}

// Callback function to handle pressing/releasing the screen or a mouse button
int32 UI::PointerButtonEventCallback(s3ePointerEvent* pEvent, void* pUserData)
{
	stringstream msg;
	msg << "PointerButtonEventCallback: ";
	if (pEvent->m_Pressed)
	{
		msg << "Press";
	}
	else
	{
		msg << "Release";
	}
	msg << " at " << pEvent->m_x << ", " << pEvent->m_y;
	s_instance->SetMessage(msg.str());

	// pEvent->m_Button is of type s3ePointerButton and indicates which mouse
	// button was pressed.  For touchscreens this should always have the value
	// S3E_POINTER_BUTTON_SELECT

	if (!pEvent->m_Pressed)
	{
		if (s_instance->m_uiRequestGamerUUID.GetActive())
		{
			s_instance->m_uiRequestGamerUUID.SetActive(false);
			s_instance->DoRequestGamerInfo();
		}

		if (s_instance->m_uiRequestProducts.GetActive())
		{
			s_instance->m_uiRequestProducts.SetActive(false);
			s_instance->DoRequestProducts();
		}

		if (s_instance->m_uiRequestReceipts.GetActive())
		{
			s_instance->m_uiRequestReceipts.SetActive(false);
			s_instance->DoRequestReceipts();
		}

		if (s_instance->m_uiShutdown.GetActive())
		{
			s_instance->m_uiShutdown.SetActive(false);
			s_instance->DoShutdown();
		}

		for (unsigned int index = 0; index < s_instance->m_products.size(); ++index)
		{
			TextButton* textButton = s_instance->m_products[index];
			if (textButton->GetActive())
			{
				textButton->SetActive(false);
				s_instance->m_selectedProduct = (Product*)textButton->DataContext;
					
				s_instance->DoRequestPurchase();
			}
		}
	}

	return 0;
}

// Callback function to handle drags on the touchscreen/mouse movements
int32 UI::PointerMotionEventCallback(s3ePointerMotionEvent* pEvent, void* pUserData)
{
	int x = pEvent->m_x;
	int y = pEvent->m_y;

	stringstream msg;
	msg << "PointerMotionEventCallback: Moved to ";
	msg << x << ", " << y;
	s_instance->SetMessage(msg.str());

	s_instance->m_uiRequestGamerUUID.CheckHover(x, y);
	s_instance->m_uiRequestProducts.CheckHover(x, y);
	s_instance->m_uiRequestReceipts.CheckHover(x, y);
	s_instance->m_uiShutdown.CheckHover(x, y);

	for (unsigned int index = 0; index < s_instance->m_products.size(); ++index)
	{
		s_instance->m_products[index]->CheckHover(x, y);
	}

	return 0;
}

void UI::DoRequestGamerInfo()
{
	SetMessage("Fetching gamer uuid...");
	Plugin_requestGamerInfo(
		Application::s_ui.m_callbacksRequestGamerInfo->GetSuccessEvent(),
		Application::s_ui.m_callbacksRequestGamerInfo->GetFailureEvent(),
		Application::s_ui.m_callbacksRequestGamerInfo->GetCancelEvent());
}

void UI::DoRequestProducts()
{
	ClearProducts();

	m_selectedProduct = NULL;

	SetMessage("Requesting products...");

	// prepare json
	string productsJson = "[";

	int index = 0;
	for (vector<string>::iterator iter = m_productIds.begin(); iter != m_productIds.end(); ++iter)
	{
		string productId = *iter;
		if (index == 0)
		{
			productsJson.append("\"");
		}
		else
		{
			productsJson.append(", \"");
		}
		productsJson.append(productId);
		productsJson.append("\"");
		++index;
	}

	productsJson.append("]");

#if ENABLE_VERBOSE_LOGGING
	IwTrace(DEFAULT, (productsJson.c_str()));
#endif

	Plugin_requestProducts(productsJson.c_str(),
		Application::s_ui.m_callbacksRequestProducts->GetSuccessEvent(),
		Application::s_ui.m_callbacksRequestProducts->GetFailureEvent(),
		Application::s_ui.m_callbacksRequestProducts->GetCancelEvent());
}

void UI::DoRequestPurchase()
{
	if (NULL == m_selectedProduct)
	{
		SetMessage("No product selected.");
	}
	else
	{
		SetMessage("Requesting purchase...");
		string productType = "ENTITLEMENT";
		Plugin_requestPurchase(m_selectedProduct->Identifier.c_str(),
			productType.c_str(),
			Application::s_ui.m_callbacksRequestPurchase->GetSuccessEvent(),
			Application::s_ui.m_callbacksRequestPurchase->GetFailureEvent(),
			Application::s_ui.m_callbacksRequestPurchase->GetCancelEvent());
	}
}

void UI::DoRequestReceipts()
{
	ClearReceipts();

	SetMessage("Requesting receipts...");
	Plugin_requestReceipts(
		Application::s_ui.m_callbacksRequestReceipts->GetSuccessEvent(),
		Application::s_ui.m_callbacksRequestReceipts->GetFailureEvent(),
		Application::s_ui.m_callbacksRequestReceipts->GetCancelEvent());
}

void UI::DoShutdown()
{
	SetMessage("Shutdown...");
	Plugin_shutdown(
		Application::s_ui.m_callbacksShutdown->GetSuccessEvent(),
		Application::s_ui.m_callbacksShutdown->GetFailureEvent(),
		Application::s_ui.m_callbacksShutdown->GetCancelEvent());
}

void UI::HandleInput()
{
#if false && defined(ANDROID)
	vector<int> released = vector<int>();
	for (int playerNum = 0; playerNum < Controller::MAX_CONTROLLERS; ++playerNum)
	{
		if (Plugin_isPressedUp(playerNum, Controller::BUTTON_MENU))
		{
			released.push_back(Controller::BUTTON_MENU);
		}
		if (Plugin_isPressedUp(playerNum, Controller::BUTTON_DPAD_DOWN))
		{
			released.push_back(Controller::BUTTON_DPAD_DOWN);
		}
		if (Plugin_isPressedUp(playerNum, Controller::BUTTON_DPAD_LEFT))
		{
			released.push_back(Controller::BUTTON_DPAD_LEFT);
		}
		if (Plugin_isPressedUp(playerNum, Controller::BUTTON_DPAD_RIGHT))
		{
			released.push_back(Controller::BUTTON_DPAD_RIGHT);
		}
		if (Plugin_isPressedUp(playerNum, Controller::BUTTON_DPAD_UP))
		{
			released.push_back(Controller::BUTTON_DPAD_UP);
		}
		if (Plugin_isPressedUp(playerNum, Controller::BUTTON_O))
		{
			released.push_back(Controller::BUTTON_O);
		}
	}
	Plugin_clearButtonStates();

	if (find(released.begin(), released.end(), Controller::BUTTON_MENU) != released.end())
	{
		SetDirections();

		//LOGI("Key event, hack, regaining focus...\r\n");
		//Application::m_app->activity->callbacks->onWindowFocusChanged(Application::m_app->activity, true);
		//LOGI("Key event, hack complete***\r\n");
	}

	if (find(released.begin(), released.end(), Controller::BUTTON_DPAD_LEFT) != released.end())
	{
		if (m_selectedButton &&
			m_selectedButton->Left)
		{
			m_selectedButton->SetActive(false);
			m_selectedButton = m_selectedButton->Left;
			m_selectedButton->SetActive(true);

			SetDirections();
		}
	}

	if (find(released.begin(), released.end(), Controller::BUTTON_DPAD_RIGHT) != released.end())
	{
		if (m_selectedButton &&
			m_selectedButton->Right)
		{
			if (find(m_products.begin(), m_products.end(), m_selectedButton) == m_products.end())
			{
				m_selectedButton->SetActive(false);
			}
			m_selectedButton = m_selectedButton->Right;
			m_selectedButton->SetActive(true);

			SetDirections();
		}
	}

	if (find(released.begin(), released.end(), Controller::BUTTON_DPAD_UP) != released.end())
	{
		if (m_selectedButton &&
			m_selectedButton->Up)
		{
			m_selectedButton->SetActive(false);
			m_selectedButton = m_selectedButton->Up;
			m_selectedButton->SetActive(true);

			if (find(m_products.begin(), m_products.end(), m_selectedButton) != m_products.end())
			{
				m_selectedProduct = m_selectedButton;
				m_uiRequestProducts.Left = m_selectedProduct;
				m_uiRequestPurchase.Left = m_selectedProduct;
			}

			SetDirections();
		}
	}

	if (find(released.begin(), released.end(), Controller::BUTTON_DPAD_DOWN) != released.end())
	{
		if (m_selectedButton &&
			m_selectedButton->Down)
		{
			m_selectedButton->SetActive(false);
			m_selectedButton = m_selectedButton->Down;
			m_selectedButton->SetActive(true);

			if (find(m_products.begin(), m_products.end(), m_selectedButton) != m_products.end())
			{
				m_selectedProduct = m_selectedButton;
				m_uiRequestProducts.Left = m_selectedProduct;
				m_uiRequestPurchase.Left = m_selectedProduct;
			}

			SetDirections();
		}
	}

	if (find(released.begin(), released.end(), Controller::BUTTON_O) != released.end())
	{
		if (m_selectedButton)
		{
			if (m_selectedButton == &m_uiRequestGamerUUID)
			{
				DoRequestGamerInfo();
			}
			if (m_selectedButton == &m_uiRequestProducts)
			{
				DoRequestProducts();
			}
			if (m_selectedButton == &m_uiRequestPurchase)
			{
				DoRequestPurchase();
			}
			if (m_selectedButton == &m_uiRequestReceipts)
			{
				DoRequestReceipts();
			}

			if (m_selectedButton == &m_uiShutdown)
			{
				DoShutdown();
			}
		}
	}
#endif
}

void UI::SetUsername(const string& username)
{
	stringstream ss;
	ss << "Username: " << username.c_str();
	m_uiLabelUsername.SetText(ss.str());
}

void UI::SetGamerUUID(const string& uuid)
{
	stringstream ss;
	ss << "UUID: " << uuid.c_str();
	m_uiLabelGamerUuid.SetText(ss.str());
}

void UI::SetMessage(const string& message)
{
	stringstream ss;
	ss << "Message: " << message.c_str();
	m_uiLabelMessage.SetText(ss.str());
}

void UI::ClearProducts()
{
	for (unsigned int index = 0; index < m_products.size(); ++index)
	{
		TextButton* txtProduct = m_products[index];
		txtProduct->Destroy();
		Product* product = (Product*)txtProduct->DataContext;
		if (product)
		{
			delete product;
		}
		delete txtProduct;
	}
	m_products.clear();

	m_selectedProduct = NULL;
}

void UI::ClearReceipts()
{
	for (unsigned int index = 0; index < m_receipts.size(); ++index)
	{
		TextLabel* txtReceipt = m_receipts[index];
		txtReceipt->Destroy();
		Receipt* receipt = (Receipt*)txtReceipt->DataContext;
		if (receipt)
		{
			delete receipt;
		}
		delete txtReceipt;
	}
	m_receipts.clear();
}

void UI::AddProduct(const Product& product)
{
	m_pendingProducts.push_back(product);
}

void UI::AddReceipt(const Receipt& receipt)
{
	m_pendingReceipts.push_back(receipt);
}

void UI::Exit()
{
	ClearProducts();
	ClearReceipts();

	s3eDeviceRequestQuit();
}