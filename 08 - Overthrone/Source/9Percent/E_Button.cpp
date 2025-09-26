/******************************************************************************/
/*!
\file   E_Button.cpp
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Marc Alviz Evangelista (100%)
\par    email: marcalviz.e\@digipen.edu
\par    DigiPen login: marcalviz.e

\brief
  This is the implementation file for all member functions of class Button.
  Includign utility functions.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#include "pch.h"
#include "E_Button.h"
#include "E_Engine.h"
#include "E_Singleton.h"
#include <iostream>

namespace Engine {

	Button::Button()
		: btnActive(true),
		btnInteractable(true),
		btnState(BTN_STATE_DEFAULT),
		btnPress(BUTTON_DEFAULT)
	{
	}

	Button::Button(const std::string& text, f32 fontScale, const Color& textColor, const Transform& transform, const std::string& texturePath, f32 transparency, const Color& colorTint,bool isActive, bool interactable, std::function<void(BUTTON_PRESS)> function, BUTTON_PRESS press)
		: btnActive(isActive),
		btnInteractable(interactable),
		btnOnclick(function),
		btnState(BTN_STATE_DEFAULT),
		btnPress(press)
	{
		btnObj = ObjectBuilder::CreateSquare(transform, SpriteBuilder::CreateSpriteSquare(texturePath), transparency, colorTint);
		btnText = TextBuilder::CreateText(text, fontScale, transform.GetPosition(), textColor);
		btnOnclick = function;
	}

	sPtr<Text> Button::GetButtonText()
	{
		return this->btnText;
	}

	sPtr<Object> Button::GetButtonObject()
	{
		return this->btnObj;
	}

	void Button::SetButtonText(sPtr<Text> textPtr)
	{
		this->btnText = textPtr;
	}

	void Button::SetButtonObj(sPtr<Object> objectPtr)
	{
		this->btnObj = objectPtr;
	}

	void Button::SetActive(bool newValue)
	{
		this->btnActive = newValue;
	}

	void Button::SetInteractable(bool newValue)
	{
		this->btnInteractable = newValue;
	}

	bool Button::GetActive()
	{
		return this->btnActive;
	}

	bool Button::GetInteractable()
	{
		return this->btnInteractable;
	}

	bool Button::ButtonTriggered(s32 x, s32 y)
	{
		bool isTriggered = false;
		f32 winWidth, winHeight, xMax, xMin, yMax, yMin;
		xMin = this->btnObj->GetPosition().x - this->btnObj->GetScale().x * 0.5f;
		xMax = this->btnObj->GetPosition().x + this->btnObj->GetScale().x * 0.5f;
		yMin = this->btnObj->GetPosition().y - this->btnObj->GetScale().y * 0.5f;
		yMax = this->btnObj->GetPosition().y + this->btnObj->GetScale().y * 0.5f;
		winWidth = sCast<float>(C_renderWidth);
		winHeight = sCast<float>(C_renderHeight);
		x -= sCast<s32>(winWidth * 0.5f);
		y -= sCast<s32>(winHeight * 0.5f);
		y = -y;

		if (x > xMin &&
			x < xMax &&
			y > yMin &&
			y < yMax)
		{
			isTriggered = true;
		}

		return isTriggered;
	}

	Vec2 Button::GetPosition()
	{
		return this->btnObj->GetPosition();
	}

	void Button::InvokeFunction()
	{
		this->btnOnclick(this->btnPress);
	}

	void Button::Render()
	{
		if (!btnActive)
		{
			return;
		}
		this->btnObj->Render();
		this->btnText->Render();
	}

	sPtr<Button> ButtonBuilder::CreateButton(const std::string& text, f32 fontScale, const Color& textColor,
		  const Transform& transform, const std::string& texturePath, std::function<void(BUTTON_PRESS)> function,
		  BUTTON_PRESS press, f32 transparency, const Color& colorTint, bool isActive, bool interactable)
	{
		return sPtr<Button>(new Button(
			text,
			fontScale,
			textColor,
			transform,
			texturePath,
			transparency,
			colorTint,
			isActive,
			interactable,
			function,
			press
		));
	}
}