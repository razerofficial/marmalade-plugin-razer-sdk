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
 * Internal header for the RazerSDK extension.
 *
 * This file should be used for any common function definitions etc that need to
 * be shared between the platform-dependent and platform-indepdendent parts of
 * this extension.
 */

/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */


#ifndef RazerSDK_INTERNAL_H
#define RazerSDK_INTERNAL_H

#include "s3eTypes.h"
#include "RazerSDK.h"
#include "RazerSDK_autodefs.h"


/**
 * Initialise the extension.  This is called once then the extension is first
 * accessed by s3eregister.  If this function returns S3E_RESULT_ERROR the
 * extension will be reported as not-existing on the device.
 */
s3eResult RazerSDKInit();

/**
 * Platform-specific initialisation, implemented on each platform
 */
s3eResult RazerSDKInit_platform();

/**
 * Terminate the extension.  This is called once on shutdown, but only if the
 * extension was loader and Init() was successful.
 */
void RazerSDKTerminate();

/**
 * Platform-specific termination, implemented on each platform
 */
void RazerSDKTerminate_platform();
void OuyaController_startOfFrame_platform();

bool OuyaController_selectControllerByPlayer_platform(int playerNum);

bool OuyaController_selectControllerByDeviceID_platform(int deviceID);

int OuyaController_getAxisValue_platform(int axis);

bool OuyaController_getButton_platform(int button);

bool OuyaController_buttonPressedThisFrame_platform(int button);

bool OuyaController_buttonReleasedThisFrame_platform(int button);

bool OuyaController_buttonChangedThisFrame_platform(int button);

int OuyaController_getPlayerNum_platform();


#endif /* !RazerSDK_INTERNAL_H */
