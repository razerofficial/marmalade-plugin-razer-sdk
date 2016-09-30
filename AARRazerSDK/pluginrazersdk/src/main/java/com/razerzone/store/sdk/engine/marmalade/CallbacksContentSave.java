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

import com.razerzone.store.sdk.content.GameMod;


public class CallbacksContentSave {

	private final String TAG  = CallbacksContentSave.class.getSimpleName();

	public native void CallbacksContentSaveOnError(GameMod gameMod, int code, String reason);
	public native void CallbacksContentSaveOnSuccess(GameMod gameMod);

	public void onError(GameMod ouyaMod, int code, String reason) {
		Log.e(TAG, "onError code="+code+" reason="+reason);
		
		CallbacksContentSaveOnError(ouyaMod, code, reason);
	}

	public void onSuccess(GameMod gameMod) {
		Log.i(TAG, "onSuccess");

		CallbacksContentSaveOnSuccess(gameMod);
	}
}
