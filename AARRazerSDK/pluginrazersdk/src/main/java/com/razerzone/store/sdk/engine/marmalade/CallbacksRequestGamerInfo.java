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

package com.razerzone.store.sdk.engine.marmalade;


import android.util.Log;


public class CallbacksRequestGamerInfo {

	private final String TAG  = CallbacksRequestGamerInfo.class.getSimpleName();

	private static final boolean sEnableLogging = false;

	public native void CallbacksRequestGamerInfoOnSuccess(String jsonData);
	public native void CallbacksRequestGamerInfoOnFailure(int errorCode, String errorMessage);
	public native void CallbacksRequestGamerInfoOnCancel();

	public void onSuccess(final String jsonData) {
		if (sEnableLogging) {
			Log.d(TAG, "onSuccess=" + jsonData);
		}
		CallbacksRequestGamerInfoOnSuccess(jsonData);
	}

	public void onFailure(final int errorCode, final String errorMessage) {
		if (sEnableLogging) {
			Log.d(TAG, "onFailure: errorCode=" + errorCode + " errorMessage=" + errorMessage);
		}
		if (null == errorMessage) {
			CallbacksRequestGamerInfoOnFailure(errorCode, "");
		} else {
			CallbacksRequestGamerInfoOnFailure(errorCode, errorMessage);
		}
	}

	public void onCancel() {
		if (sEnableLogging) {
			Log.d(TAG, "onCancel");
		}
		CallbacksRequestGamerInfoOnCancel();
	}
}
