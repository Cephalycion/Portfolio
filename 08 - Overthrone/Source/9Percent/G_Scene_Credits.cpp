/******************************************************************************/
/*!
\file   G_Scene_Credits.cpp
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Marc Alviz Evangelista (100%)
\par    email: marcalviz.e\@digipen.edu
\par    DigiPen login: marcalviz.e

\brief
  This is the implementation file for all member functions of class Scene_Credits.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#include "pch.h"
#include "G_Scene_Credits.h"
#include "E_Engine.h"
#include "E_Singleton.h"
#include "E_AudioManager.h"
#include <iostream>

namespace Game {

	Scene_Credits::Scene_Credits()
		: currPage(1)
		, maxPages(3)
	{
	}

	void Scene_Credits::Init()
	{
		background = ObjectBuilder::CreateSquare(Transform(0.0f, 0.0f, 0.0f, 1600.0f, 900.0f), SpriteBuilder::CreateSpriteSquare("Textures/UI/OtherMenus.png"));

		page1 = ObjectBuilder::CreateSquare(Transform(0.0f, 0.0f, 0.0f, 1600.0f, 900.0f), SpriteBuilder::CreateSpriteSquare("Textures/UI/Credits/Page1.png"));
		page2 = ObjectBuilder::CreateSquare(Transform(0.0f, 0.0f, 0.0f, 1600.0f, 900.0f), SpriteBuilder::CreateSpriteSquare("Textures/UI/Credits/Page2.png"));
		page3 = ObjectBuilder::CreateSquare(Transform(0.0f, 0.0f, 0.0f, 1600.0f, 900.0f), SpriteBuilder::CreateSpriteSquare("Textures/UI/Credits/Page3.png"));

		backBtn = ButtonBuilder::CreateButton("", 0.45f, MakeColor(255, 0, 0, 255), Transform(0.f, -325.f, 0.f, 348.f, 128.f),
			"Textures/UI/BackButtonMiddle.png", std::bind(&Scene_Credits::CreditsButtonPress, this, std::placeholders::_1), BUTTON_MAINMENU);

		nextBtn = ButtonBuilder::CreateButton("", 0.45f, MakeColor(255, 0, 0, 255), Transform(450.f, -325.f, 0.f, 362.f, 122.f),
			"Textures/UI/Credits/NextButtonRight.png", std::bind(&Scene_Credits::CreditsButtonPress, this, std::placeholders::_1), BUTTON_NEXT);
		prevBtn = ButtonBuilder::CreateButton("", 0.45f, MakeColor(255, 0, 0, 255), Transform(-450.f, -325.f, 0.f, 362.f, 122.f),
			"Textures/UI/Credits/PrevButtonLeft.png", std::bind(&Scene_Credits::CreditsButtonPress, this, std::placeholders::_1), BUTTON_PREV);

		maxPages = 3;
		currPage = 1;
		prevBtn->SetActive(false);
		btnList.insert(btnList.end(), nextBtn);
		btnList.insert(btnList.end(), prevBtn);
		btnList.insert(btnList.end(), backBtn);
	}

	void Scene_Credits::Update()
	{
		if (AEInputCheckTriggered(AEVK_LBUTTON))
		{
			s32 mouseX, mouseY;
			AEInputGetCursorPosition(&mouseX, &mouseY);

			for (sPtr<Button> btn : btnList)
			{
				if (btn->GetActive())
				{
					if (btn->ButtonTriggered(mouseX, mouseY))
					{
						btn->InvokeFunction();
					}
				}
			}
		}
	}
	
	void Scene_Credits::Render()
	{
		background->Render();

		switch (currPage)
		{
		case 1:
		{
			page1->Render();
			break;
		}
		case 2:
		{
			page2->Render();
			break;
		}
		case 3:
		{
			page3->Render();
			break;
		}
		default:
			break;
		}

		for (sPtr<Button> btn : btnList)
		{
			btn->Render();
		}
	}
	
	void Scene_Credits::Exit()
	{
	}

	void Scene_Credits::CreditsButtonPress(BUTTON_PRESS press)
	{
		AudioManager::playAudio(Engine::SFX_BUTTON, false);
		switch (press)
		{
		case Engine::BUTTON_MAINMENU:
		{
			Singleton<ProgramEngine>::Instance()->SwitchToScene(SCENE_MAINMENU);
			break;
		}
		case Engine::BUTTON_NEXT:
		{
			if (currPage < maxPages)
			{
				++currPage;
			}
			break;
		}
		case Engine::BUTTON_PREV:
		{
			if (currPage > 1)
			{
				--currPage;
			}
			break;
		}
		default:
			break;
		}
		if (currPage == 3)
		{
			nextBtn->SetActive(false);
		}
		else
		{
			nextBtn->SetActive(true);
		}
		if (currPage == 1)
		{
			prevBtn->SetActive(false);
		}
		else
		{
			prevBtn->SetActive(true);
		}
	}

}
