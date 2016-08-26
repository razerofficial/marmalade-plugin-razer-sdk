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

#include "TextButton.h"
#include "UI.h"

#include "IwGx.h"
#include "IwGxFont.h"
#include "Iw2D.h"

void TextButton::Init()
{
	DataContext = NULL;
	m_text = "";
	m_active = false;
	m_x = 0;
	m_y = 0;
	m_width = 0;
	m_height = 0;
}

TextButton::TextButton()
{
	Init();
}

TextButton::TextButton(int x, int y, int width, int height, const std::string& text)
{
	Init();

	m_x = x;
	m_y = y;
	m_width = width;
	m_height = height;
	m_text = text;
}

int TextButton::GetWidth()
{
	return m_width;
}

void TextButton::SetPosition(int x, int y, int width, int height)
{
	m_x = x;
	m_y = y;
	m_width = width;
	m_height = height;
}

void TextButton::SetActive(bool active)
{
	m_active = active;
}

bool TextButton::GetActive()
{
	return m_active;
}

const std::string& TextButton::GetText()
{
	return m_text;
}

void TextButton::SetText(const std::string& text)
{
	m_text = text;
}

void TextButton::CheckHover(int x, int y)
{
	if (m_x <= x &&
		x <= (m_x + m_width) &&
		m_y <= y &&
		y <= (m_y + m_height))
	{
		SetActive(true);
	}
	else
	{
		SetActive(false);
	}
}

void TextButton::Render()
{
	CIw2DImage* image = 0;
	if (m_active)
	{
		image = UI::GetActiveButton();
	}
	else
	{
		image = UI::GetInactiveButton();
	}
	if (image)
	{
		CIwFVec2 pos;
		pos.x = m_x;
		pos.y = m_y;
		Iw2DDrawImage(image, pos, CIwFVec2(m_width, m_height));
	}

	UI::DrawText(m_text, m_x, m_y+10, m_width, m_height, IW_GX_FONT_ALIGN_CENTRE, IW_GX_FONT_ALIGN_MIDDLE);
}

void TextButton::Destroy()
{
}