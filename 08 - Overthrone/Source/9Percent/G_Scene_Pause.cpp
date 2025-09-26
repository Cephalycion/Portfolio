/******************************************************************************/
/*!
\file   G_Scene_Pause.cpp
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Marc Alviz Evangelista (100%)
\par    email: marcalviz.e\@digipen.edu
\par    DigiPen login: marcalviz.e

\brief
  This is the implementation file for all member functions of class Scene_Pause

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#include "pch.h"
#include "G_Scene_Pause.h"
#include "E_Engine.h"
#include <iostream>

namespace Game {

	Scene_Pause::Scene_Pause()
		: animTimer(0.45f)
		, animPlayed(false)
		, atConfirm(false)
	{
	}

	void Scene_Pause::Init()
	{
		title = TextBuilder::CreateText("Paused", 0.5f,MakeVec2(0.f, 350.f), MakeColor(255, 0, 0, 255));

		quitBtn = ButtonBuilder::CreateButton("Quit", 0.45f, MakeColor(32, 33, 47, 255), Transform(140.f, -240.f, 0.f, 186.f, 96.f),
			"Textures/UI/Pause/Blanksprite.png", std::bind(&Scene_Pause::PauseButtonPress, this, std::placeholders::_1), BUTTON_CONFIRM);

		unpauseBtn = ButtonBuilder::CreateButton("Unpause", 0.35f, MakeColor(32, 33, 47, 255), Transform(-80.f, -240.f, 0.f, 186.f, 96.f),
			"Textures/UI/Pause/Blanksprite.png", std::bind(&Scene_Pause::PauseButtonPress, this, std::placeholders::_1), BUTTON_UNPAUSE);

		helpBtn = ButtonBuilder::CreateButton("", 0.35f, MakeColor(32, 33, 47, 255), Transform(-80.f, -30.f, 0.f, 186.f, 96.f),
			"Textures/UI/Pause/Blanksprite.png", std::bind(&Scene_Pause::PauseButtonPress, this, std::placeholders::_1), BUTTON_TUTORIAL);
		

		yes = ButtonBuilder::CreateButton("Yes", 0.45f, MakeColor(32, 33, 47, 255), Transform(120.f, -150.f, 0.f, 186.f, 96.f),
			"Textures/UI/Pause/Blanksprite.png", std::bind(&Scene_Pause::PauseButtonPress, this, std::placeholders::_1), BUTTON_MAINMENU);


		no = ButtonBuilder::CreateButton("No", 0.45f, MakeColor(32, 33, 47, 255), Transform(-100.f, -150.f, 0.f, 186.f, 96.f),
			"Textures/UI/Pause/Blanksprite.png", std::bind(&Scene_Pause::PauseButtonPress, this, std::placeholders::_1), BUTTON_NO);

		confirm = ObjectBuilder::CreateSquare(Transform(0.0f, -100.0f, 0.0f, 550.0f, 340.0f), SpriteBuilder::CreateSpriteSquare("Textures/UI/Pause/Confirm.png"));

		pauseAnim = ObjectBuilder::CreateSquare(Transform(0.0f, -100.0f, 0.0f, 600.0f, 800.0f), SpriteBuilder::CreateSpriteSquare("Textures/UI/Pause/Pause.png", true, 2, 3, 0.5f, 6));
		menu = ObjectBuilder::CreateSquare(Transform(0.0f, -100.0f, 0.0f, 600.0f, 800.0f), SpriteBuilder::CreateSpriteSquare("Textures/UI/Pause/PauseMenu.png"));

		animPlayed = false;
		atConfirm = false;
		animTimer = 0.45f;

		yes->SetActive(false);
		no->SetActive(false);
		yes->SetInteractable(false);
		no->SetInteractable(false);
		btnList.insert(btnList.end(), quitBtn);
		btnList.insert(btnList.end(), unpauseBtn);
		btnList.insert(btnList.end(), helpBtn);
		btnList.insert(btnList.end(), yes);
		btnList.insert(btnList.end(), no);
	}

	void Scene_Pause::Update()
	{
		if (animPlayed)
		{
			if (AEInputCheckTriggered(AEVK_ESCAPE) || AEInputCheckTriggered(AEVK_P))
				Singleton<ProgramEngine>::Instance()->PopScene();

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
		else
		{
			animTimer -= Engine::C_frametime;
			if (animTimer < 0.0f)
			{
				animPlayed = true;
			}
			pauseAnim->Update();
		}
	}

	void Scene_Pause::Render()
	{
		title->Render();
		if (animPlayed)
		{
			if (atConfirm)
			{
				confirm->Render();
			}
			else
			{
				menu->Render();
			}
			for (sPtr<Button> btn : btnList)
			{
				if (btn->GetActive())
				{
					btn->Render();
				}
			}
		}
		else
		{
			pauseAnim->Render();
		}
	}

	void Scene_Pause::Exit()
	{
	}

	void Scene_Pause::PauseButtonPress(BUTTON_PRESS press)
	{
		switch (press)
		{
		case Engine::BUTTON_MAINMENU:
		{
			Singleton<ProgramEngine>::Instance()->PopScene();
			Singleton<ProgramEngine>::Instance()->SwitchToScene(SCENE_MAINMENU);
			break;
		}
		case Engine::BUTTON_UNPAUSE:
		{
			Singleton<ProgramEngine>::Instance()->PopScene();
			break;
		}
		case Engine::BUTTON_CONFIRM:
		{
			atConfirm = true;
			unpauseBtn->SetInteractable(false);
			unpauseBtn->SetActive(false);
			quitBtn->SetInteractable(false);
			quitBtn->SetActive(false);
			helpBtn->SetInteractable(false);
			helpBtn->SetActive(false);

			yes->SetActive(true);
			no->SetActive(true);
			yes->SetInteractable(true);
			no->SetInteractable(true);
			break;
		}
		case Engine::BUTTON_NO:
		{
			atConfirm = false;
			unpauseBtn->SetInteractable(true);
			unpauseBtn->SetActive(true);
			quitBtn->SetInteractable(true);
			quitBtn->SetActive(true);
			helpBtn->SetInteractable(true);
			helpBtn->SetActive(true);

			yes->SetActive(false);
			no->SetActive(false);
			yes->SetInteractable(false);
			no->SetInteractable(false);
			break;
		}
		case Engine::BUTTON_TUTORIAL:
		{
			Singleton<ProgramEngine>::Instance()->PushScene(SCENE_TUTORIAL);
			break;
		}
		default:
			break;
		}
	}
}