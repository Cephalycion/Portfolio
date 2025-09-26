/******************************************************************************/
/*!
\file   E_SceneFactory.cpp
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Kendrick Sim Hean Guan (70%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\author Marc Alviz Evangelista (30%)
\par    email: marcalviz.e\@digipen.edu
\par    DigiPen login: marcalviz.e

\brief
  This is the implementation file for a factory to create scene objects.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#include "pch.h"
#include "E_SceneFactory.h"
#include "G_Scene_Splash.h"
#include "G_Scene_Game.h"
#include "G_Scene_MainMenu.h"
#include "G_Scene_Settings.h"
#include "G_Scene_Credits.h"
#include "G_Scene_Tutorial.h"
#include "G_Scene_Pause.h"
#include "E_Error.h"

namespace Engine { namespace Scene {

	sPtr<Scene_Base> CreateScene(SCENE_IDENTIFIER sceneIdentifier)
	{
		auto scenePtr = sPtr<Scene_Base>();

		switch (sceneIdentifier)
		{
		case SCENE_SPLASH:
			scenePtr.reset(new Game::Scene_Splash());
			break;
		case SCENE_GAME:
			scenePtr.reset(new Game::Scene_Game());
			break;
		case SCENE_MAINMENU:
			scenePtr.reset(new Game::Scene_MainMenu());
			break;
		case SCENE_SETTINGS:
			scenePtr.reset(new Game::Scene_Settings());
			break;
		case SCENE_CREDITS:
			scenePtr.reset(new Game::Scene_Credits());
			break;
		case SCENE_TUTORIAL:
			scenePtr.reset(new Game::Scene_Tutorial());
			scenePtr->SetUpdateBlocking(true);
			break;
		case SCENE_PAUSE:
			scenePtr.reset(new Game::Scene_Pause());
			scenePtr->SetUpdateBlocking(true);
			break;
		default:
			Error::RaiseErrFormatted("CreateScene", "Invalid identifier %d!", ERROR_ERR, sceneIdentifier);
		}

		return scenePtr;
	}
}}
