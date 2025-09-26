/******************************************************************************/
/*!
\file   G_Scene_Settings.cpp
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Marc Alviz Evangelista (100%)
\par    email: marcalviz.e\@digipen.edu
\par    DigiPen login: marcalviz.e

\brief
  This is the implementation file for all member functions of class Scene_Settings.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#include "pch.h"
#include "G_Scene_Settings.h"
#include "E_Engine.h"
#include <iostream>
#include "E_AudioManager.h"
#include "E_IOManager.h"


namespace Game{

	Scene_Settings::Scene_Settings()
		: currentTab(SETTINGS_AUDIO)
		, volumeSfx(5)
		, volumeBgm(5)
		, renderSFX(false)
	{
	}

	void Scene_Settings::Init()
	{
		title = TextBuilder::CreateText("Settings", 1.0f, MakeVec2(0.f, 300.f));
		currentTab = SETTINGS_AUDIO;

		// change this to take from the saved volumes
		volumeBgm = sCast<int>(AudioManager::getVolumeBGM() * 10);
		volumeSfx = sCast<int>(AudioManager::getVolumeSFX() * 10);
		background = ObjectBuilder::CreateSquare(Transform(0.0f, 0.0f, 0.0f, 1600.0f, 900.0f), SpriteBuilder::CreateSpriteSquare("Textures/UI/OtherMenus.png"));
		BgmTitle = ObjectBuilder::CreateSquare(Transform(-350.0f, 0.0f, 0.0f, 322.f, 256.0f), SpriteBuilder::CreateSpriteSquare("Textures/UI/Settings/BGMflag.png"));
		SfxTitle = ObjectBuilder::CreateSquare(Transform(-300.0f, 175.0f, 0.0f, 322.f, 220.0f), SpriteBuilder::CreateSpriteSquare("Textures/UI/Settings/SFXflag.png"));
		portrait = ObjectBuilder::CreateSquare(Transform(0.0f, 0.0f, 0.0f, 1600.0f, 900.0f), SpriteBuilder::CreateSpriteSquare("Textures/UI/Settings/portrait.png"));
		terrain = ObjectBuilder::CreateSquare(Transform(0.0f, 0.0f, 0.0f, 1600.0f, 900.0f), SpriteBuilder::CreateSpriteSquare("Textures/UI/Settings/TerrainFM.png"));
		FArmy = ObjectBuilder::CreateSquare(Transform(0.0f, 0.0f, 0.0f, 1600.0f, 900.0f), SpriteBuilder::CreateSpriteSquare("Textures/UI/Settings/FArmy.png"));
		MArmy = ObjectBuilder::CreateSquare(Transform(0.0f, 0.0f, 0.0f, 1600.0f, 900.0f), SpriteBuilder::CreateSpriteSquare("Textures/UI/Settings/MArmy.png"));
		sky = ObjectBuilder::CreateSquare(Transform(0.0f, 0.0f, 0.0f, 1600.0f, 900.0f), SpriteBuilder::CreateSpriteSquare("Textures/UI/Settings/PortraitBg.png"));
		clouds = ObjectBuilder::CreateSquare(Transform(0.0f, 0.0f, 0.0f, 1600.0f, 900.0f), SpriteBuilder::CreateSpriteSquare("Textures/UI/Settings/PortraitClouds.png"));
		renderSFX = true;
		for (int i = 0; i < maxVolume; ++i)
		{
			bgmFlagHeights.push_back(AERandFloat() * 15.f);
			sfxFlagHeights.push_back(AERandFloat() * 15.f);
		}


		InitSettingButtons();
	}

	void Scene_Settings::Update()
	{
		if (AEInputCheckTriggered(AEVK_LBUTTON))
		{
			s32 mouseX, mouseY;
			AEInputGetCursorPosition(&mouseX, &mouseY);

			for (sPtr<Button> btn : btnList)
			{
				if (btn->ButtonTriggered(mouseX, mouseY))
				{
					btn->InvokeFunction();
				}
			}
		}
	}
	
	void Scene_Settings::Render()
	{
		background->Render();
		// Render Order
		// sky
		// sfx flag
		// audio flags
		// Middleground Army
		// Terrain
		// bgm flag
		// audio flags
		// foregroundarmy
		// portrait
		sky->Render();
		clouds->Render();
		SfxTitle->Render();
		renderSFX = true;
		RenderAudioBars();
		MArmy->Render();
		terrain->Render();
		BgmTitle->Render();
		renderSFX = false;
		RenderAudioBars();
		FArmy->Render();
		portrait->Render();
		for (sPtr<Button> btn : btnList)
		{
			btn->Render();
		}
	}

	void Scene_Settings::Exit()
	{
		// Save audio here as an extra
	}

	void Scene_Settings::InitSettingButtons()
	{
		// Back button
		backBtn = ButtonBuilder::CreateButton("", 0.45f, MakeColor(255, 0, 0, 255), Transform(0.f, -325.f, 0.f, 348.f, 128.f),
			"Textures/UI/BackButtonMiddle.png", std::bind(&Scene_Settings::SettingsButtonPress, this, std::placeholders::_1), BUTTON_MAINMENU);

		btnList.insert(btnList.end(), backBtn);

		// Audio
		// - Music

		audioBgmIncBtn = ButtonBuilder::CreateButton("", 0.45f, MakeColor(255, 0, 0, 255), Transform(435.f, 18.f, 0.f, 72.f, 44.f),
			"Textures/UI/Settings/IncFlag.png", std::bind(&Scene_Settings::SettingsButtonPress, this, std::placeholders::_1), BUTTON_BGM_INC);

		btnList.insert(btnList.end(), audioBgmIncBtn);

		audioBgmDecBtn = ButtonBuilder::CreateButton("", 0.45f, MakeColor(255, 0, 0, 255), Transform(-382.f, 20.f, 0.f, 72.f, 44.f),
			"Textures/UI/Settings/DecFlag.png", std::bind(&Scene_Settings::SettingsButtonPress, this, std::placeholders::_1), BUTTON_BGM_DEC);

		btnList.insert(btnList.end(), audioBgmDecBtn);
		// - SFX

		audioSfxIncBtn = ButtonBuilder::CreateButton("", 0.45f, MakeColor(255, 0, 0, 255), Transform(418.f, 189.f, 0.f, 72.f, 44.f),
			"Textures/UI/Settings/IncFlag.png", std::bind(&Scene_Settings::SettingsButtonPress, this, std::placeholders::_1), BUTTON_SFX_INC);

		btnList.insert(btnList.end(), audioSfxIncBtn);

		audioSfxDecBtn = ButtonBuilder::CreateButton("", 0.45f, MakeColor(255, 0, 0, 255), Transform(-402.f, 194.f, 0.f, 72.f, 44.f),
			"Textures/UI/Settings/DecFlag.png", std::bind(&Scene_Settings::SettingsButtonPress, this, std::placeholders::_1), BUTTON_SFX_DEC);

		btnList.insert(btnList.end(), audioSfxDecBtn);

		// Init the squares used
		// save as an object, access their sprites in render
		activeAudio = ObjectBuilder::CreateSquare(
			Transform(0.0f, 0.0f, 0.0f, 42.f, 40.f),
			SpriteBuilder::CreateSpriteSquare("Textures/UI/Settings/FlagOn.png"));

		activeAudio->SetScale(MakeVec2(84.f, 80.f));
		activeAudio->SetColorTint(Color(1.f, 1.f, 0.f, 1.f));

		inactiveAudio = ObjectBuilder::CreateSquare(
			Transform(0.0f, 0.0f, 0.0f, 42.f, 40.f),
			SpriteBuilder::CreateSpriteSquare("Textures/UI/Settings/FlagOff.png"));
		inactiveAudio->SetScale(MakeVec2(84.f, 80.f));
		inactiveAudio->SetColorTint(Color(1.f, 1.f, 0.f, 0.5f));


	}

	void Scene_Settings::SettingsButtonPress(BUTTON_PRESS press)
	{
		Engine::AudioManager::playAudio(Engine::SFX_BUTTON, false);
		switch (press)
		{
		case Engine::BUTTON_MAINMENU:
		{
			Singleton<ProgramEngine>::Instance()->SwitchToScene(SCENE_MAINMENU);
			break;
		}
		case Engine::BUTTON_AUDIO:
		{
			currentTab = SETTINGS_AUDIO;
			audioTabBtn->SetInteractable(false);
			break;
		}
		// Save audio here as well
		case Engine::BUTTON_BGM_DEC:
		{
			if (volumeBgm > 0)
			{
				--volumeBgm;
			}
			break;
		}
		case Engine::BUTTON_BGM_INC:
		{
			if (volumeBgm < maxVolume)
			{
				++volumeBgm;
			}
			break;
		}
		case Engine::BUTTON_SFX_DEC:
		{
			if (volumeSfx > 0.0f)
			{
				--volumeSfx;
			}
			break;
		}
		case Engine::BUTTON_SFX_INC:
		{
			if (volumeSfx < maxVolume)
			{
				++volumeSfx;
			}
			break;
		}
		default:
			break;
		}

		Singleton<Engine::IOManager>::Instance()->WriteSettings(volumeBgm * 0.1f, volumeSfx * 0.1f);

	}
	void Scene_Settings::RenderAudioBars()
	{
		f32 xOffset = activeAudio->GetScale().x;
		Vec2 vecOffsetBgm = MakeVec2(audioBgmDecBtn->GetPosition().x +
			xOffset * 0.15f
			, audioBgmDecBtn->GetPosition().y - 25.f);

		Vec2 vecOffsetSfx = MakeVec2(audioSfxDecBtn->GetPosition().x +
			xOffset * 0.15f
			, audioSfxDecBtn->GetPosition().y - 25.f) ;
		f32 sfxOriginY = vecOffsetSfx.y;
		f32 bgmOriginY = vecOffsetBgm.y;

		for (int j = 0; j < maxVolume; ++j)
		{
			if (!renderSFX)
			{
				vecOffsetBgm.x += xOffset * 0.85f;
				vecOffsetBgm.y = bgmOriginY + bgmFlagHeights[j];
				if (j < volumeBgm)
				{
					// Update position
					activeAudio->SetPosition(vecOffsetBgm);
					// Draw
					activeAudio->Render();
				}
				else
				{
					// Update position
					inactiveAudio->SetPosition(vecOffsetBgm);
					// Draw
					inactiveAudio->Render();
				}
			}
			else
			{
				vecOffsetSfx.x += xOffset * 0.85f;
				vecOffsetSfx.y = sfxOriginY + sfxFlagHeights[j];
				if (j < volumeSfx)
				{
			
					// Update position
					activeAudio->SetPosition(vecOffsetSfx);
					// Draw
					activeAudio->Render();
				}
				else
				{
					// Update position
					inactiveAudio->SetPosition(vecOffsetSfx);
					// Draw
					inactiveAudio->Render();
				}
			}
		}


	}
}