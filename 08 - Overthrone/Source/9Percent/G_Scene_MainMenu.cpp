/******************************************************************************/
/*!
\file   G_Scene_MainMenu.cpp
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Marc Alviz Evangelista (90%)
\par    email: marcalviz.e\@digipen.edu
\par    DigiPen login: marcalviz.e

\author Kendrick Sim Hean Guan (5%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\author Ryan Chan Kuan Fu (5%)
\par    email: c.kuanfuryan\@digipen.edu
\par    DigiPen login: c.kuanfuryan

\brief
  This is the implementation file for all member functions of class Scene_MainMenu

All content � 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#include "pch.h"
#include "G_Scene_MainMenu.h"
#include "E_Engine.h"
#include <iostream>
#include "E_AudioManager.h"

static bool musicPlaying = false;

namespace Game {

	Scene_MainMenu::Scene_MainMenu()
		: atConfirm(false)
	{
	}

	void Scene_MainMenu::Init()
	{
		menuCam.reset(new Camera());
		titleText = TextBuilder::CreateText("Title Here");
		background = ObjectBuilder::CreateSquare(Transform(0.0f, 0.0f, 0.0f, 1600.0f, 900.0f), SpriteBuilder::CreateSpriteSquare("Textures/UI/MainMenu.png"));

		playBtn = ButtonBuilder::CreateButton("", 0.25f, MakeColor(255, 0, 0, 255), Transform(0.f, 300.f, 0.f, 190.f, 250.f),
			"Textures/UI/PlayButton.png", std::bind(&Scene_MainMenu::MoveToState, this, std::placeholders::_1), BUTTON_PLAY);

		settingsBtn = ButtonBuilder::CreateButton("", 0.35f, MakeColor(255, 0, 0, 255), Transform(-450.f, 190.f, 0.f, 200.f, 152.f),
			"Textures/UI/Options.png", std::bind(&Scene_MainMenu::MoveToState, this, std::placeholders::_1), BUTTON_SETTINGS);

		creditsBtn = ButtonBuilder::CreateButton("", 0.4f, MakeColor(255, 0, 0, 255), Transform(-550.f, -40.f, 0.f, 200.f, 152.f),
			"Textures/UI/Credits.png", std::bind(&Scene_MainMenu::MoveToState, this, std::placeholders::_1), BUTTON_CREDITS);

		controlsBtn = ButtonBuilder::CreateButton("", 0.15f, MakeColor(255, 0, 0, 255), Transform(450.f, 190.f, 0.f, 200.f, 152.f),
			"Textures/UI/HowToPlay.png", std::bind(&Scene_MainMenu::MoveToState, this, std::placeholders::_1), BUTTON_TUTORIAL);

		quitBtn = ButtonBuilder::CreateButton("", 0.45f, MakeColor(255, 0, 0, 255), Transform(550.f, -40.f, 0.f, 200.f, 152.f),
			"Textures/UI/Quit.png", std::bind(&Scene_MainMenu::MoveToState, this, std::placeholders::_1), BUTTON_CONFIRM);

		yes = ButtonBuilder::CreateButton("Yes", 0.45f, MakeColor(32, 33, 47, 255), Transform(120.f, -150.f, 0.f, 186.f, 96.f),
			"Textures/UI/Pause/Blanksprite.png", std::bind(&Scene_MainMenu::MoveToState, this, std::placeholders::_1), BUTTON_QUIT);


		no = ButtonBuilder::CreateButton("No", 0.45f, MakeColor(32, 33, 47, 255), Transform(-100.f, -150.f, 0.f, 186.f, 96.f),
			"Textures/UI/Pause/Blanksprite.png", std::bind(&Scene_MainMenu::MoveToState, this, std::placeholders::_1), BUTTON_NO);

		confirm = ObjectBuilder::CreateSquare(Transform(0.0f, -100.0f, 0.0f, 550.0f, 340.0f), SpriteBuilder::CreateSpriteSquare("Textures/UI/Pause/Confirm.png"));

		atConfirm = false;

		yes->SetActive(false);
		no->SetActive(false);
		yes->SetInteractable(false);
		no->SetInteractable(false);
		btnList.insert(btnList.end(), playBtn);
		btnList.insert(btnList.end(), settingsBtn);
		btnList.insert(btnList.end(), controlsBtn);
		btnList.insert(btnList.end(), creditsBtn);
		btnList.insert(btnList.end(), quitBtn);
		btnList.insert(btnList.end(), yes);
		btnList.insert(btnList.end(), no);


		if (!musicPlaying)
		{
			Engine::AudioManager::playAudio(Engine::BGM_MENU, true);
			musicPlaying = true;
		}
	}
	void Scene_MainMenu::Update()
	{
		// mouse input
		if (AEInputCheckTriggered(AEVK_LBUTTON))
		{
			s32 mouseX, mouseY;
			AEInputGetCursorPosition(&mouseX, &mouseY);

			for (sPtr<Button> btn : btnList)
			{
				if (btn->GetInteractable())
				{
					if (btn->ButtonTriggered(mouseX, mouseY))
					{
						btn->InvokeFunction();
					}
				}
			}
		}
	}
	void Scene_MainMenu::Render()
	{
		Singleton<CameraManager>::Instance()->SetActiveCam(menuCam);

		background->Render();
		if (atConfirm)
		{
			confirm->Render();
		}
		for (sPtr<Button> btn : btnList)
		{
			if (btn->GetActive())
			{
				btn->Render();
			}
		}

		Singleton<CameraManager>::Instance()->ResetActiveCam();
	}
	void Scene_MainMenu::Exit()
	{
		Singleton<CameraManager>::Instance()->ResetActiveCam();
	}
	void Scene_MainMenu::MoveToState(BUTTON_PRESS state)
	{
		AudioManager::playAudio(Engine::SFX_BUTTON, false);
		switch (state)
		{
		case Engine::BUTTON_PLAY:
		{
			Singleton<ProgramEngine>::Instance()->SwitchToScene(SCENE_GAME);
			Engine::AudioManager::stopAudio();
			musicPlaying = false;
			break;
		}
		case Engine::BUTTON_SETTINGS:
		{
			Singleton<ProgramEngine>::Instance()->SwitchToScene(SCENE_SETTINGS);
			break;
		}
		case Engine::BUTTON_CREDITS:
		{
			Singleton<ProgramEngine>::Instance()->SwitchToScene(SCENE_CREDITS);
			break;
		}
		case Engine::BUTTON_TUTORIAL:
		{
			Singleton<ProgramEngine>::Instance()->SwitchToScene(SCENE_TUTORIAL);
			break;
		}
		case Engine::BUTTON_QUIT:
		{
			Singleton<ProgramEngine>::Instance()->Quit();
			break;
		}
		case Engine::BUTTON_CONFIRM:
		{
			atConfirm = true;
			yes->SetActive(true);
			no->SetActive(true);
			yes->SetInteractable(true);
			no->SetInteractable(true);

			playBtn->SetInteractable(false);
			settingsBtn->SetInteractable(false);
			controlsBtn->SetInteractable(false);
			creditsBtn->SetInteractable(false);
			quitBtn->SetInteractable(false);

			break;
		}
		case Engine::BUTTON_NO:
		{
			atConfirm = false;
			yes->SetActive(false);
			no->SetActive(false);
			yes->SetInteractable(false);
			no->SetInteractable(false);

			playBtn->SetInteractable(true);
			settingsBtn->SetInteractable(true);
			controlsBtn->SetInteractable(true);
			creditsBtn->SetInteractable(true);
			quitBtn->SetInteractable(true);
			break;
		}
		default:
			break;
		}
	}
}