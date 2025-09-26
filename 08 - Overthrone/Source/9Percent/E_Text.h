/******************************************************************************/
/*!
\file   E_Text.h
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Kendrick Sim Hean Guan (100%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\brief
  This is an interface file for a text object that can be drawn to the screen
  with various settings.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#pragma once
#include "E_AEEngineInterface.h"
#include "E_Constants.h"
#include <string>
#include <vector>

namespace Engine {

	using namespace Graphics;

	enum TEXT_ALIGNMENT
	{
		TEXT_ALIGN_BOTTOM = 0,
		TEXT_ALIGN_LEFT = 0,
		TEXT_ALIGN_CENTER = 1,
		TEXT_ALIGN_TOP = 2,
		TEXT_ALIGN_RIGHT = 2,
	};

	struct TextSettings
	{
	public:
		s8 fontId;

		f32 fontScale;
		Color textColor;
		Color shadowColor;
		bool hasShadow;
		Vec2 position;
		TEXT_ALIGNMENT hAlign, vAlign;

		TextSettings(s8 fontId, f32 fontScale, const Color& textColor, bool hasShadow, const Vec2& position, TEXT_ALIGNMENT hAlign, TEXT_ALIGNMENT vAlign);

	};

	class IText
	{
	protected:
		TextSettings settings;
		std::string textStr;

		// Get the print size of the textStr owned by this object.
		void GetPrintSize(Vec2* outPrintSize);
		// Get the print size of a custom text.
		void GetPrintSize(const std::string& text, Vec2* outPrintSize);

	public:
		IText(const std::string& textStr, s8 fontId, f32 fontScale, const Color& textColor, bool hasShadow, const Vec2& position, TEXT_ALIGNMENT hAlign, TEXT_ALIGNMENT vAlign);

		const std::string& GetTextStr() const;
		virtual void SetTextStr(const std::string& newText);
		s8 GetFontId() const;
		virtual void SetFontId(s8 newFontId);
		f32 GetFontScale() const;
		virtual void SetFontScale(f32 newScale);
		const Color& GetColor() const;
		virtual void SetColor(const Color& newColor);
		const Color& GetShadowColor() const;
		virtual void SetShadowColor(const Color& newShadowColor);
		bool GetHasShadow() const;
		virtual void SetHasShadow(bool newHasShadow);
		virtual void SetAlpha(f32 newAlpha);
		const Vec2& GetPosition() const;
		virtual void SetPosition(const Vec2& newPosition);
		TEXT_ALIGNMENT GetHAlign() const;
		virtual void SetHAlign(TEXT_ALIGNMENT newHAlign);
		TEXT_ALIGNMENT GetVAlign() const;
		virtual void SetVAlign(TEXT_ALIGNMENT newVAlign);

		virtual void Render() = 0;

	};

	class Text : public IText
	{
	protected:
		Vec2 printSize;
		bool isPrintSizeDirty;
		void CalculatePrintSize();

	public:
		Text(const std::string& text = "Text", f32 fontScale = 1.0f, const Vec2& position = Vec2Zero(), const Color& textColor = Color(), bool hasShadow = false,
			const std::string& fontFile = C_textDefaultFontFile, TEXT_ALIGNMENT horizontalAlignment = TEXT_ALIGN_CENTER, TEXT_ALIGNMENT verticalAlignment = TEXT_ALIGN_CENTER);
		Text(const std::string& text, f32 fontScale, const Vec2& position, const Color& textColor, bool hasShadow, s8 fontId,
			TEXT_ALIGNMENT horizontalAlignment = TEXT_ALIGN_CENTER, TEXT_ALIGNMENT verticalAlignment = TEXT_ALIGN_CENTER);

		virtual void SetTextStr(const std::string& newText) override;
		virtual void SetFontId(s8 newFontId) override;
		virtual void SetFontScale(f32 newScale) override;

		virtual void Render() override;

	};

	class WrapText : public IText
	{
	protected:
		std::vector<Text> textObjs;
		f32 maxWidth;

		void RecalcTextObjs();

	public:
		WrapText(const std::string& text = "Wrap Text", f32 maxWidth = 400.0f, f32 fontScale = 1.0f, const Vec2& position = Vec2Zero(), const Color& textColor = Color(), bool hasShadow = false,
			const std::string& fontFile = C_textDefaultFontFile, TEXT_ALIGNMENT horizontalAlignment = TEXT_ALIGN_CENTER, TEXT_ALIGNMENT verticalAlignment = TEXT_ALIGN_CENTER);
		WrapText(const std::string& text, f32 maxWidth, f32 fontScale, const Vec2& position, const Color& textColor, bool hasShadow, s8 fontId,
			TEXT_ALIGNMENT horizontalAlignment, TEXT_ALIGNMENT verticalAlignment);

		virtual void SetTextStr(const std::string& newText) override;
		virtual void SetFontId(s8 newFontId) override;
		virtual void SetFontScale(f32 newScale) override;
		virtual void SetColor(const Color& newColor) override;
		virtual void SetShadowColor(const Color& newShadowColor) override;
		virtual void SetHasShadow(bool newHasShadow) override;
		virtual void SetAlpha(f32 newAlpha) override;
		virtual void SetPosition(const Vec2& newPosition) override;
		virtual void SetHAlign(TEXT_ALIGNMENT newHAlign) override;
		virtual void SetVAlign(TEXT_ALIGNMENT newVAlign) override;

		virtual void Render() override;

	};

	class TextBuilder
	{
	protected:
		s8 activeFontId;
		std::string activeFontFile;

	public:
		TextBuilder();

		// Set the font to be used by the Text objects created after this call.
		static bool SetFont(const std::string& fontFile);
		static sPtr<Text> CreateText(const std::string& text = "Text", f32 fontScale = 1.0f, const Vec2& position = Vec2Zero(), const Color& textColor = Color(), bool hasShadow = false,
			TEXT_ALIGNMENT horizontalAlignment = TEXT_ALIGN_CENTER, TEXT_ALIGNMENT verticalAlignment = TEXT_ALIGN_CENTER);
		static sPtr<WrapText> CreateWrapText(const std::string& text = "WrapText", f32 maxWidth = 400.0f, f32 fontScale = 1.0f, const Vec2& position = Vec2Zero(),
			const Color& textColor = Color(), bool hasShadow = false, TEXT_ALIGNMENT horizontalAlignment = TEXT_ALIGN_CENTER, TEXT_ALIGNMENT verticalAlignment = TEXT_ALIGN_CENTER);

	};

}

