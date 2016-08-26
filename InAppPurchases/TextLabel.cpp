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

#include "TextLabel.h"
#include "UI.h"

#include "IwGx.h"
#include "IwGxFont.h"

void TextLabel::Init()
{
	DataContext = NULL;
	m_text = "";
	m_x = 0;
	m_y = 0;
	m_width = 0;
	m_height = 0;
}

TextLabel::TextLabel()
{
	Init();
}

TextLabel::TextLabel(const std::string& text)
{
	Init();
	m_text = text;
}

TextLabel::TextLabel(int x, int y, int width, int height, const std::string& text)
{
	Init();
	m_text = text;
	m_x = x;
	m_y = y;
	m_width = width;
	m_height = height;
}

void TextLabel::SetText(const std::string& text)
{
	m_text = text;
}

void TextLabel::SetPosition(int x, int y, int width, int height)
{
	m_x = x;
	m_y = y;
	m_width = width;
	m_height = height;
}

void TextLabel::Render()
{
	UI::DrawText(m_text, m_x, m_y, m_width, m_height);
}

void TextLabel::Destroy()
{
	m_text = "";
}