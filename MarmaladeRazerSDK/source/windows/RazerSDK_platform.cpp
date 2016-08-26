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

/*
 * windows-specific implementation of the RazerSDK extension.
 * Add any platform-specific functionality here.
 */
/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */
#include "RazerSDK_internal.h"

s3eResult RazerSDKInit_platform()
{
    // Add any platform-specific initialisation code here
    return S3E_RESULT_SUCCESS;
}

void RazerSDKTerminate_platform()
{
    // Add any platform-specific termination code here
}

void OuyaController_startOfFrame_platform()
{
}

bool OuyaController_selectControllerByPlayer_platform(int playerNum)
{
    return false;
}

bool OuyaController_selectControllerByDeviceID_platform(int deviceID)
{
    return false;
}

int OuyaController_getAxisValue_platform(int axis)
{
    return 0;
}

bool OuyaController_getButton_platform(int button)
{
    return false;
}

bool OuyaController_buttonPressedThisFrame_platform(int button)
{
    return false;
}

bool OuyaController_buttonReleasedThisFrame_platform(int button)
{
    return false;
}

bool OuyaController_buttonChangedThisFrame_platform(int button)
{
    return false;
}

int OuyaController_getPlayerNum_platform()
{
    return 0;
}
