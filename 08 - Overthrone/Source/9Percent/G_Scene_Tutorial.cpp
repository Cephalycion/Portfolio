/******************************************************************************/
/*!
\file   G_Scene_Tutorial.cpp
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Marc Alviz Evangelista (100%)
\par    email: marcalviz.e\@digipen.edu
\par    DigiPen login: marcalviz.e

\brief
  This is the implementation file for all member functions of class Scene_Tutorial,
  which is the How To Play screen.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#include "pch.h"
#include "G_Scene_Tutorial.h"
#include "E_Engine.h"
#include "E_Singleton.h"
#include "E_AudioManager.h"
#include <iostream>


namespace Game {

	Scene_Tutorial::Scene_Tutorial()
		: currentPage(0)
	{
	}

	void Scene_Tutorial::Init()
	{
		title = TextBuilder::CreateText("Tutorial");
		background = ObjectBuilder::CreateSquare(Transform(0.0f, 0.0f, 0.0f, 1600.0f, 900.0f), SpriteBuilder::CreateSpriteSquare("Textures/UI/OtherMenus.png"));
		page1 = ObjectBuilder::CreateSquare(Transform(0.0f, 0.0f, 0.0f, 1600.0f, 900.0f), SpriteBuilder::CreateSpriteSquare("Textures/UI/Tutorial/Portrait.png"));
		page2 = ObjectBuilder::CreateSquare(Transform(0.0f, 0.0f, 0.0f, 1600.0f, 900.0f), SpriteBuilder::CreateSpriteSquare("Textures/UI/Tutorial/Page2.png"));
		moveSprite = ObjectBuilder::CreateSquare(Transform(0.0f, 110.0f, 0.0f, 120.0f, 55.0f), SpriteBuilder::CreateSpriteSquare("Textures/UI/Tutorial/Move.png", true, 3, 4, 2.5f, 12));
		jumpSprite = ObjectBuilder::CreateSquare(Transform(132.0f, -4.0f, 0.0f, 90.0f, 100.0f), SpriteBuilder::CreateSpriteSquare("Textures/UI/Tutorial/Jump.png", true, 3, 3, 1.0f, 9));
		dashSprite = ObjectBuilder::CreateSquare(Transform(-290.0f, 100.0f, 0.0f, 100.0f, 100.0f), SpriteBuilder::CreateSpriteSquare("Textures/UI/Tutorial/Dash.png", true, 4, 4, 1.5f, 15));
		grabSprite = ObjectBuilder::CreateSquare(Transform(-145.0f, 10.0f, 0.0f, 150.0f, 74.0f), SpriteBuilder::CreateSpriteSquare("Textures/UI/Tutorial/Grab.png", true, 3, 3, 1.5f, 9));
		grappleSprite = ObjectBuilder::CreateSquare(Transform(265.0f, 90.0f, 0.0f, 150.0f, 124.0f), SpriteBuilder::CreateSpriteSquare("Textures/UI/Tutorial/Grapple.png", true, 4, 4, 1.5f, 15));
		backBtn = ButtonBuilder::CreateButton("", 0.45f, MakeColor(255, 0, 0, 255), Transform(0.f, -325.f, 0.f, 348.f, 128.f),
			"Textures/UI/BackButtonMiddle.png", std::bind(&Scene_Tutorial::TutorialButtonPress, this, std::placeholders::_1), BUTTON_MAINMENU);

		nextBtn = ButtonBuilder::CreateButton("", 0.45f, MakeColor(255, 0, 0, 255), Transform(450.f, -325.f, 0.f, 362.f, 122.f),
			"Textures/UI/Tutorial/NextButtonRight.png", std::bind(&Scene_Tutorial::TutorialButtonPress, this, std::placeholders::_1), BUTTON_NEXT);
		prevBtn = ButtonBuilder::CreateButton("", 0.45f, MakeColor(255, 0, 0, 255), Transform(-450.f, -325.f, 0.f, 362.f, 122.f),
			"Textures/UI/Tutorial/PrevButtonLeft.png", std::bind(&Scene_Tutorial::TutorialButtonPress, this, std::placeholders::_1), BUTTON_PREV);

		currentPage = 1;

		prevBtn->SetActive(false);
		btnList.insert(btnList.end(), nextBtn);
		btnList.insert(btnList.end(), prevBtn);
		btnList.insert(btnList.end(), backBtn);
	}
	
	void Scene_Tutorial::Update()
	{
		if (currentPage == 1)
		{
			moveSprite->Update();
			jumpSprite->Update();
			dashSprite->Update();
			grabSprite->Update();
			grappleSprite->Update();
		}

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

	void Scene_Tutorial::Render()
	{
		background->Render();
		if (currentPage == 1)
		{
			page1->Render();
			// The animations
			moveSprite->Render();
			jumpSprite->Render();
			dashSprite->Render();
			grabSprite->Render();
			grappleSprite->Render();

		}
		else if (currentPage == 2)
		{
			page2->Render();
		}
		//title->Render();
		for (sPtr<Button> btn : btnList)
		{
			btn->Render();
		}
	}
	
	void Scene_Tutorial::Exit()
	{
	}

	void Scene_Tutorial::TutorialButtonPress(BUTTON_PRESS press)
	{
		AudioManager::playAudio(Engine::SFX_BUTTON, false);
		switch (press)
		{
		case Engine::BUTTON_MAINMENU:
		{
			if (Singleton<ProgramEngine>::Instance()->GetNumLoadedScenes() > 1)
				// We reached this menu through the game pause menu.
				Singleton<ProgramEngine>::Instance()->PopScene();
			else
				// reached through the main menu.
				Singleton<ProgramEngine>::Instance()->SwitchToScene(SCENE_MAINMENU);
			break;
		}
		case Engine::BUTTON_NEXT:
		{
			currentPage = 2;
			nextBtn->SetActive(false);
			prevBtn->SetActive(true);
			break;
		}
		case Engine::BUTTON_PREV:
		{
			nextBtn->SetActive(true);
			prevBtn->SetActive(false);
			currentPage = 1;
			break;
		}
		default:
			break;
		}
	}
}
