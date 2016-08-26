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

#include "IwGx.h"
#include "IwGxFont.h"
#include "IwResManager.h"
#include "Iw2D.h"
#include "s3e.h"
#include "IwDebug.h"

#include "Application.h"
#include "UI.h"

#include "RazerSDK.h"

using namespace std;

string g_version = "Mamalade Version: 077";

void Render()
{
	IwGxClear();

	UI::DrawText(g_version, 200, 100, TEXT_MAX_WIDTH, 20);

	Application::s_ui.InitUI();

	Application::s_ui.Render();

	IwGxFlush();
	IwGxSwapBuffers();
}

int main()
{
	IwGxInit();
	IwGxSetColClear(0, 0, 0xff, 0xff);
	IwResManagerInit();
	Iw2DInit();
	IwGxFontInit();

	//Load the group containing the example font
	IwGetResManager()->LoadGroup("IwGxFontTTF.group");

	UI::SetFont((CIwGxFont*)IwGetResManager()->GetResNamed("Serif_24", "CIwGxFont"));

	// Load the group with textures
	IwGetResManager()->LoadGroup("tiles.group");

	// Load the textures
	UI::SetActiveButton(Iw2DCreateImageResource("button_active"));
	UI::SetInactiveButton(Iw2DCreateImageResource("button_inactive"));

	// Loading the above group in x86 debug generates `bin` from the group files
	if (!RazerSDKAvailable())
	{
		IwTrace(DEFAULT, ("RazerSDK is not available, exit!"));
		return 0;
	}

    const int textWidth = s3eDebugGetInt(S3E_DEBUG_FONT_SIZE_WIDTH);
    const int textHeight = s3eDebugGetInt(S3E_DEBUG_FONT_SIZE_HEIGHT);
    const int width = s3eSurfaceGetInt(S3E_SURFACE_WIDTH);
    const int height = s3eSurfaceGetInt(S3E_SURFACE_HEIGHT);

	Application::InitPlugin();

	while (!s3eDeviceCheckQuitRequest())
	{
		//IwTrace(DEFAULT, ("Main loop while (!s3eDeviceCheckQuitRequest()"));

		Render();

		// keep polling for input, don't kill the CPU
		s3eDeviceYield(0);
	}

	Application::s_ui.Destroy();

	IwGxFontTerminate();
	Iw2DTerminate();
	IwResManagerTerminate();
	IwGxTerminate();
	return 0;
}