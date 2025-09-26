/******************************************************************************/
/*!
\file   E_Text.cpp
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Kendrick Sim Hean Guan (100%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\brief
  This is the implementation file for a text object that can be drawn to the screen
  with various settings.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#include "pch.h"
#include "E_Text.h"
#include "E_Asset.h"

namespace Engine {

	TextSettings::TextSettings(s8 fontId, f32 fontScale, const Color& textColor, bool hasShadow, const Vec2& position, TEXT_ALIGNMENT hAlign, TEXT_ALIGNMENT vAlign)
		: fontId(fontId)
		, fontScale(fontScale)
		, textColor(textColor)
		, shadowColor(1.0f - textColor.r, 1.0f - textColor.g, 1.0f - textColor.b)
		, hasShadow(hasShadow)
		, position(position)
		, hAlign(hAlign)
		, vAlign(vAlign)
	{
	}


	IText::IText(const std::string& textStr, s8 fontId, f32 fontScale, const Color& textColor, bool hasShadow, const Vec2& position, TEXT_ALIGNMENT hAlign, TEXT_ALIGNMENT vAlign)
		: settings(fontId, fontScale, textColor, hasShadow, position, hAlign, vAlign)
		, textStr(textStr)
	{
	}

	void IText::GetPrintSize(Vec2* outPrintSize)
	{
		GetPrintSize(textStr, outPrintSize);
	}
	void IText::GetPrintSize(const std::string& text, Vec2* outPrintSize)
	{
		// Bug Fix: AEGfxGetPrintSize() doesn't provide correct printSize.y when fontScale is negative.
		AEGfxGetPrintSize(settings.fontId, text.c_str(), std::fabs(settings.fontScale), &outPrintSize->x, &outPrintSize->y);
	}

	const std::string& IText::GetTextStr() const
	{
		return textStr;
	}
	void IText::SetTextStr(const std::string& newText)
	{
		textStr = newText;
	}
	s8 IText::GetFontId() const
	{
		return settings.fontId;
	}
	void IText::SetFontId(s8 newFontId)
	{
		settings.fontId = newFontId;
	}
	f32 IText::GetFontScale() const
	{
		return settings.fontScale;
	}
	void IText::SetFontScale(f32 newScale)
	{
		settings.fontScale = newScale;
	}
	const Color& IText::GetColor() const
	{
		return settings.textColor;
	}
	void IText::SetColor(const Color& newColor)
	{
		settings.textColor = newColor;
	}
	const Color& IText::GetShadowColor() const
	{
		return settings.shadowColor;
	}
	void IText::SetShadowColor(const Color& newShadowColor)
	{
		settings.shadowColor = newShadowColor;
	}
	bool IText::GetHasShadow() const
	{
		return settings.hasShadow;
	}
	void IText::SetHasShadow(bool newHasShadow)
	{
		settings.hasShadow = newHasShadow;
	}
	void IText::SetAlpha(f32 newAlpha)
	{
		settings.textColor.a = newAlpha;
	}
	const Vec2& IText::GetPosition() const
	{
		return settings.position;
	}
	void IText::SetPosition(const Vec2& newPosition)
	{
		settings.position = newPosition;
	}
	TEXT_ALIGNMENT IText::GetHAlign() const
	{
		return settings.hAlign;
	}
	void IText::SetHAlign(TEXT_ALIGNMENT newHAlign)
	{
		settings.hAlign = newHAlign;
	}
	TEXT_ALIGNMENT IText::GetVAlign() const
	{
		return settings.vAlign;
	}
	void IText::SetVAlign(TEXT_ALIGNMENT newVAlign)
	{
		settings.vAlign = newVAlign;
	}


	Text::Text(const std::string& text, f32 fontScale, const Vec2& position, const Color& textColor, bool hasShadow, const std::string& fontFile,
		TEXT_ALIGNMENT horizontalAlignment, TEXT_ALIGNMENT verticalAlignment)
		: IText(text, AssetManager::GetFont(fontFile), fontScale, textColor, hasShadow, position, horizontalAlignment, verticalAlignment)
		, printSize(Vec2Zero())
		, isPrintSizeDirty(true)
	{
	}
	Text::Text(const std::string& text, f32 fontScale, const Vec2& position, const Color& textColor, bool hasShadow, s8 fontId,
		TEXT_ALIGNMENT horizontalAlignment, TEXT_ALIGNMENT verticalAlignment)
		: IText(text, fontId, fontScale, textColor, hasShadow, position, horizontalAlignment, verticalAlignment)
		, printSize(Vec2Zero())
		, isPrintSizeDirty(true)
	{
	}

	void Text::SetFontId(s8 newFontId)
	{
		IText::SetFontId(newFontId);
		isPrintSizeDirty = true;
	}
	void Text::SetFontScale(f32 newScale)
	{
		IText::SetFontScale(newScale);
		isPrintSizeDirty = true;
	}

	void Text::SetTextStr(const std::string& newText)
	{
		IText::SetTextStr(newText);
		isPrintSizeDirty = true;
	}

	void Text::CalculatePrintSize()
	{
		if (settings.fontId < 0)
		{
			Error::RaiseErr("Text::CalculatePrintSize", "Invalid fontId!", ERROR_ERR);
			return;
		}

		GetPrintSize(&printSize);
		isPrintSizeDirty = false;
	}

	void Text::Render()
	{
		if (settings.fontId < 0)
		{
			Error::RaiseErr("Text::Render", "Invalid fontId!", ERROR_ERR);
			return;
		}
		
		if (isPrintSizeDirty)
			CalculatePrintSize();

		static const auto CalculateFinalPositionElement = [](f32 elementValue, TEXT_ALIGNMENT alignment, f32 printLength, f32 fontScale, s32 windowLength) -> f32 {
			// Convert to -1 to 1 space
			elementValue = elementValue * 2.0f / sCast<f32>(windowLength);

			switch (alignment)
			{
			case TEXT_ALIGN_LEFT: // Applies to BOTTOM as well
				if (fontScale > 0.0f)
					return elementValue;
				else
					return elementValue + printLength;
			case TEXT_ALIGN_CENTER:
				if (fontScale > 0.0f)
					return elementValue - printLength * 0.5f;
				else
					return elementValue + printLength * 0.5f;
			case TEXT_ALIGN_RIGHT: // Applies to TOP as well
				if (fontScale > 0.0f)
					return elementValue - printLength;
				else
					return elementValue;
			default:
				Error::RaiseErr("Text::Render::GetPosElementShiftedByAlignment", "Unimplemented alignment value!", ERROR_ERR);
				return elementValue;
			}
		};

		Vec2 finalPosition = {
			CalculateFinalPositionElement(settings.position.x, settings.hAlign, printSize.x, settings.fontScale, C_windowWidth),
			CalculateFinalPositionElement(settings.position.y, settings.vAlign, printSize.y, settings.fontScale, C_windowHeight)
		};

		if (settings.hasShadow)
		{
			AEGfxPrint(settings.fontId, textStr, finalPosition + MakeVec2(3.0f / C_windowWidth, 3.0f / C_windowHeight),
				settings.fontScale, settings.shadowColor);
		}
		AEGfxPrint(settings.fontId, textStr, finalPosition, settings.fontScale, settings.textColor);
	}


	WrapText::WrapText(const std::string& text, f32 maxWidth, f32 fontScale, const Vec2& position, const Color& textColor, bool hasShadow, const std::string& fontFile, TEXT_ALIGNMENT horizontalAlignment, TEXT_ALIGNMENT verticalAlignment)
		: IText(text, AssetManager::GetFont(fontFile), fontScale, textColor, hasShadow, position, horizontalAlignment, verticalAlignment)
		, maxWidth(maxWidth / static_cast<f32>(C_windowWidth))
	{
		RecalcTextObjs();
	}
	WrapText::WrapText(const std::string& text, f32 maxWidth, f32 fontScale, const Vec2& position, const Color& textColor, bool hasShadow, s8 fontId,
		TEXT_ALIGNMENT horizontalAlignment, TEXT_ALIGNMENT verticalAlignment)
		: IText(text, fontId, fontScale, textColor, hasShadow, position, horizontalAlignment, verticalAlignment)
		, maxWidth(maxWidth / static_cast<f32>(C_windowWidth))
	{
		RecalcTextObjs();
	}

	void WrapText::RecalcTextObjs()
	{
		constexpr char breakChar = ' ';
		f32 totalHeight = 0.0f;

		textObjs.clear();

		size_t startPos = 0, endPos = textStr.find_first_of(breakChar), testPos = endPos;
		std::string testBuffer = textStr.substr(startPos, testPos);
		std::string commitBuffer = testBuffer;
		Vec2 printSize;

		do
		{
			testPos = textStr.find_first_of(breakChar, testPos + 1);
			testBuffer = textStr.substr(startPos, testPos - startPos);

			GetPrintSize(testBuffer, &printSize);
			if (printSize.x > maxWidth)
			{
				if (commitBuffer.empty())
				{
					commitBuffer = testBuffer;
					endPos = testPos;
				}

				textObjs.emplace_back(commitBuffer, settings.fontScale, settings.position - MakeVec2(0.0f, totalHeight), settings.textColor, settings.hasShadow, settings.fontId, settings.hAlign, settings.vAlign);
				totalHeight += (printSize.y * 0.65f) * static_cast<f32>(C_windowHeight);
				testBuffer.clear();
				commitBuffer.clear();
				startPos = testPos = endPos + 1;
			}
			else
			{
				commitBuffer = testBuffer;
				endPos = testPos;
			}
		} while (endPos < std::string::npos);
		textObjs.emplace_back(commitBuffer, settings.fontScale, settings.position - MakeVec2(0.0f, totalHeight), settings.textColor, settings.hasShadow, settings.fontId, settings.hAlign, settings.vAlign);

		// The text is currently aligned as TEXT_ALIGN_TOP.
		// Modify text positions depending on the actual alignment
		switch (settings.vAlign)
		{
		case TEXT_ALIGN_CENTER:
		{
			Vec2 offset = MakeVec2(0.0f, totalHeight * 0.5f);
			for (auto& t : textObjs)
				t.SetPosition(t.GetPosition() + offset);
			break;
		}
		case TEXT_ALIGN_BOTTOM:
		{
			Vec2 offset = MakeVec2(0.0f, totalHeight);
			for (auto& t : textObjs)
				t.SetPosition(t.GetPosition() + offset);
			break;
		}
		}
	}

	void WrapText::SetTextStr(const std::string& newText)
	{
		IText::SetTextStr(newText);
		RecalcTextObjs();
	}
	void WrapText::SetFontId(s8 newFontId)
	{
		IText::SetFontId(newFontId);
		for (auto& t : textObjs)
			t.SetFontId(newFontId);
	}
	void WrapText::SetFontScale(f32 newScale)
	{
		IText::SetFontScale(newScale);
		for (auto& t : textObjs)
			t.SetFontScale(newScale);
		RecalcTextObjs();
	}
	void WrapText::SetColor(const Color& newColor)
	{
		IText::SetColor(newColor);
		for (auto& t : textObjs)
			t.SetColor(newColor);
	}
	void WrapText::SetShadowColor(const Color& newShadowColor)
	{
		IText::SetShadowColor(newShadowColor);
		for (auto& t : textObjs)
			t.SetShadowColor(newShadowColor);
	}
	void WrapText::SetHasShadow(bool newHasShadow)
	{
		IText::SetHasShadow(newHasShadow);
		for (auto& t : textObjs)
			t.SetHasShadow(newHasShadow);
	}
	void WrapText::SetAlpha(f32 newAlpha)
	{
		IText::SetAlpha(newAlpha);
		for (auto& t : textObjs)
			t.SetAlpha(newAlpha);
	}
	void WrapText::SetPosition(const Vec2& newPosition)
	{
		Vec2 posDiff = newPosition - settings.position;
		IText::SetPosition(newPosition);
		for (auto& t : textObjs)
			t.SetPosition(t.GetPosition() + posDiff);
	}
	void WrapText::SetHAlign(TEXT_ALIGNMENT newHAlign)
	{
		IText::SetHAlign(newHAlign);
		for (auto& t : textObjs)
			t.SetHAlign(newHAlign);
	}
	void WrapText::SetVAlign(TEXT_ALIGNMENT newVAlign)
	{
		IText::SetVAlign(newVAlign);
		for (auto& t : textObjs)
			t.SetVAlign(newVAlign);
		RecalcTextObjs();
	}

	void WrapText::Render()
	{
		for (auto& t : textObjs)
			t.Render();
	}


	TextBuilder::TextBuilder()
		: activeFontFile(C_textDefaultFontFile)
	{
		activeFontId = AssetManager::GetFont(activeFontFile);
		if (activeFontId < 0)
			Error::RaiseErrFormatted("TextBuilder::TextBuilder", "Failed to open default font file %s!", ERROR_ERR, C_textDefaultFontFile);
	}

	bool TextBuilder::SetFont(const std::string& fontFile)
	{
		if (fontFile == Singleton<TextBuilder>::Instance()->activeFontFile)
			return true;

		s8 fontId = AssetManager::GetFont(fontFile);
		if (fontId < 0)
			return false;

		auto textBuilderInstance = Singleton<TextBuilder>::Instance();
		textBuilderInstance->activeFontId = fontId;
		textBuilderInstance->activeFontFile = fontFile;
		return true;
	}

	sPtr<Text> TextBuilder::CreateText(const std::string& text, f32 fontScale, const Vec2& position, const Color& textColor, bool hasShadow, TEXT_ALIGNMENT horizontalAlignment, TEXT_ALIGNMENT verticalAlignment)
	{
		s8 fontId = Singleton<TextBuilder>::Instance()->activeFontId;
		if (fontId < 0)
			Error::RaiseErr("TextBuilder::CreateText", "FontId is invalid!", ERROR_ERR);
			// Continue making the text, so that the program doesn't crash. It just simply can't render the text until we fix it.

		return sPtr<Text>(new Text(text, fontScale, position, textColor, hasShadow, fontId, horizontalAlignment, verticalAlignment));
	}

	sPtr<WrapText> TextBuilder::CreateWrapText(const std::string& text, f32 maxWidth, f32 fontScale, const Vec2& position, const Color& textColor, bool hasShadow, TEXT_ALIGNMENT horizontalAlignment, TEXT_ALIGNMENT verticalAlignment)
	{
		s8 fontId = Singleton<TextBuilder>::Instance()->activeFontId;
		if (fontId < 0)
			Error::RaiseErr("TextBuilder::CreateText", "FontId is invalid!", ERROR_ERR);
			// Continue making the text, so that the program doesn't crash. It just simply can't render the text until we fix it.

		return sPtr<WrapText>(new WrapText(text, maxWidth, fontScale, position, textColor, hasShadow, fontId, horizontalAlignment, verticalAlignment));
	}

}
