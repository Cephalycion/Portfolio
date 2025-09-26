/******************************************************************************/
/*!
\file   E_Button.h
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Marc Alviz Evangelista (100%)
\par    email: marcalviz.e\@digipen.edu
\par    DigiPen login: marcalviz.e

\brief
  This is the interface file for all member functions of class Button.
  Including Utility functions.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#pragma once
#include "pch.h"
#include "E_Object.h"
#include "E_Text.h"
#include <functional>

namespace Engine{

	using namespace Graphics;

	// What UI/Scene will the button open or in general what the button will do
	enum BUTTON_PRESS{
		BUTTON_DEFAULT = 0,

		// Scene States
		BUTTON_MAINMENU,
		BUTTON_PLAY,
		BUTTON_SETTINGS,
		BUTTON_CREDITS,
		BUTTON_TUTORIAL,
		BUTTON_QUIT,
		
		// Game presses
		BUTTON_PAUSE,
		BUTTON_UNPAUSE,
		BUTTON_RESTART,
		BUTTON_CONFIRM,
		BUTTON_NO,

		// Settings tabs
		BUTTON_AUDIO,
		BUTTON_GRAPHICS,

		// Setting presses - Audio
		BUTTON_MASTER_DEC,
		BUTTON_MASTER_INC,
		BUTTON_BGM_INC,
		BUTTON_BGM_DEC,
		BUTTON_SFX_INC,
		BUTTON_SFX_DEC,

		// Credits presses - Pages
		BUTTON_NEXT,
		BUTTON_PREV
	};

	enum BUTTON_STATE {
		BTN_STATE_DEFAULT = 0,
		BTN_STATE_HOVER,
		BTN_STATE_CLICK,
		BTN_STATE_UNINTERACTABLE
	};

	typedef void (*BtnFunction)(BUTTON_PRESS);

	class Button
	{
	protected:

		bool btnInteractable;
		bool btnActive;
		sPtr<Text> btnText; // text the button will use
		sPtr<Object> btnObj; // obj the button will use 
		std::function<void(BUTTON_PRESS)> btnOnclick; // function pointer for the button

		std::vector<sPtr<Texture>> btnTextures; // store textures for the diff button states

		BUTTON_STATE btnState;
		BUTTON_PRESS btnPress;

	public:


		Button();
		Button(const std::string& text, f32 fontScale, const Color& textColor,
			const Transform& transform, const std::string& texturePath, f32 transparency, const Color& colorTint,
			bool active, bool interactable, std::function<void(BUTTON_PRESS)> function, BUTTON_PRESS press);

		sPtr<Text> GetButtonText();
		sPtr<Object>GetButtonObject();
		void SetButtonText(sPtr<Text> textPtr);
		void SetButtonObj(sPtr<Object> objectPtr);
		void SetActive(bool newValue);
		void SetInteractable(bool newValue);
		bool GetActive();
		bool GetInteractable();
		bool ButtonTriggered(s32 x, s32 y);

		Vec2 GetPosition();

		void InvokeFunction();

		void Render();

	};


	class ButtonBuilder
	{
	private:
		ButtonBuilder() = delete;
	public:
		static sPtr<Button> CreateButton(
			const std::string& text = "Text", f32 fontScale = 1.0f, const Color& textColor = Color(),
			const Transform& transform = Transform(0.0f, 0.0f, 0.0f, 100.0f, 100.0f),
			const std::string& texturePath = "", std::function<void(BUTTON_PRESS)> function = nullptr, BUTTON_PRESS press = BUTTON_DEFAULT,
			f32 transparency = 1.0f, const Color& colorTint = Color(),
			bool isActive = true, bool interactable = true);
	};
}