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

#include "s3eTypes.h"

#include <string>

typedef struct s3eRequestLoginSuccessEvent
{
} s3eRequestLoginSuccessEvent;

typedef struct s3eRequestLoginFailureEvent
{
	int m_errorCode;
	char m_errorMessage[1024];
} s3eRequestLoginFailureEvent;

typedef struct s3eRequestLoginCancelEvent
{
} s3eRequestLoginCancelEvent;

class CallbacksRequestLogin
{
public:

	void RegisterCallbacks(s3eCallback onSuccess, s3eCallback onFailure, s3eCallback onCancel);

	void OnSuccess();

	void OnFailure(int errorCode, const std::string& errorMessage);

	void OnCancel();

private:
	//extension callbacks
	s3eCallback m_onSuccess;
	s3eCallback m_onFailure;
	s3eCallback m_onCancel;

	//event data sent to callbacks can't be temporary
	s3eRequestLoginSuccessEvent m_dataRequestLoginSuccessEvent;
	s3eRequestLoginFailureEvent m_dataRequestLoginFailureEvent;
	s3eRequestLoginCancelEvent m_dataRequestLoginCancelEvent;

	void RegisterCallback(s3eCallback callback, s3eCallback* savedCallback, int callbackType);
};
