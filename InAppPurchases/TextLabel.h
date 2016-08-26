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

class TextLabel
{
public:
	void* DataContext;

	TextLabel();

	TextLabel(const std::string& text);

	TextLabel(int x, int y, int width, int height, const std::string& text);

	void SetPosition(int x, int y, int width, int height);

	void SetText(const std::string& text);

	void Render();

	void Destroy();

private:

	void Init();
	
	std::string m_text;

	int m_x;
	int m_y;
	int m_width;
	int m_height;
};

