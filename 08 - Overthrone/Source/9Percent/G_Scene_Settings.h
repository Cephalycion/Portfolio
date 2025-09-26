/******************************************************************************/
/*!
\file   G_Scene_Settings.h
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Marc Alviz Evangelista (100%)
\par    email: marcalviz.e\@digipen.edu
\par    DigiPen login: marcalviz.e

\brief
  This is the interface file for all member functions of class Scene_Settings.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#pragma once
#include "pch.h"
#include "E_SceneBase.h"
#include "E_Button.h"
#include "E_Camera.h"
#include "E_Text.h"
#include "E_Constants.h"


namespace Game {
	using namespace Engine;
	using namespace Engine::Scene;

	// tabs for the different settings
	enum SETTINGS_TABS {
		SETTINGS_DEFAULT = 0,
		SETTINGS_AUDIO,
		SETTINGS_GRAPHICS
	};

	class Scene_Settings : public Scene_Base
	{
	protected:
		sPtr<Text> title;
		sPtr<Button> backBtn;

		SETTINGS_TABS currentTab;

		// Buttons for the tabs
		sPtr<Button> audioTabBtn;
		sPtr<Button> graphicsTabBtn;

		// Audio buttons and vars
		sPtr<Button> audioMasterIncBtn;
		sPtr<Button> audioMasterDecBtn;

		sPtr<Button> audioBgmIncBtn;
		sPtr<Button> audioBgmDecBtn;
		
		sPtr<Button> audioSfxIncBtn;
		sPtr<Button> audioSfxDecBtn;

		// Lists for various setting tabs
		std::vector<sPtr<Button>> audioBtnList;
		std::vector<sPtr<Button>> btnList; // for the back and tab buttons

		sPtr<Object> activeAudio;
		sPtr<Object> inactiveAudio;

		std::vector<f32> sfxFlagHeights;
		std::vector<f32> bgmFlagHeights;

		int volumeSfx;
		int volumeBgm;

		bool renderSFX;

		sPtr<Object> background;
		sPtr<Object> portrait;
		sPtr<Object> terrain;
		sPtr<Object> sky;
		sPtr<Object> clouds;
		sPtr<Object> BgmTitle;
		sPtr<Object> SfxTitle;
		sPtr<Object> FArmy;
		sPtr<Object> MArmy;

		const int maxVolume = 10;

	public:
		Scene_Settings();

		virtual void Init() override;
		virtual void Update() override;
		virtual void Render() override;
		virtual void Exit() override;


		void InitSettingButtons();

		void SettingsButtonPress(BUTTON_PRESS press);

		void RenderAudioBars();

	};
}

