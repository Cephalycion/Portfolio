/******************************************************************************/
/*!
\file   E_SceneFactory.h
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
  This is an interface file for a factory to create scene objects.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#pragma once
#include "pch.h"
#include "E_SceneBase.h"
#include "E_Constants.h"

namespace Engine { namespace Scene {

	enum SCENE_IDENTIFIER
	{
		SCENE_SPLASH = 0,
		
		SCENE_MAINMENU,
		SCENE_SETTINGS,
		SCENE_CREDITS,
		SCENE_TUTORIAL,
		SCENE_PAUSE,
		SCENE_GAME,

		SCENE_NUMSCENES
	};

	sPtr<Scene_Base> CreateScene(SCENE_IDENTIFIER sceneIdentifier);
}}
