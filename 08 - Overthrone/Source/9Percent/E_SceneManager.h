/******************************************************************************/
/*!
\file   E_SceneManager.h
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Kendrick Sim Hean Guan (100%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\brief
  This is an interface file for a system that manages scenes.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#pragma once
#include "pch.h"
#include "E_SceneFactory.h"
#include <vector>

namespace Engine { namespace Scene {

	class SceneManager
	{
	protected:
		std::vector<sPtr<Scene_Base>> scenesVec;
		std::vector<SCENE_IDENTIFIER> addScenesVec;
		std::vector<sPtr<Scene_Base>> removeScenesVec;

		std::vector<sPtr<Scene_Base>>::const_iterator firstUpdatingSceneIter;

		void FlushAddScenes();
		void FlushRemoveScenes();

	public:
		SceneManager();
		~SceneManager();

		s64 QueuePushScene(SCENE_IDENTIFIER identifier);
		void QueuePopScene();
		s64 QueueSwitchToScene(SCENE_IDENTIFIER identifier);

		void FlushChanges();

		void Cleanup();

		std::vector<sPtr<Scene_Base>>::const_iterator GetFirstSceneIter() const;
		std::vector<sPtr<Scene_Base>>::const_iterator GetFirstActiveSceneIter() const;
		std::vector<sPtr<Scene_Base>>::const_iterator GetEndIter() const;

		int GetNumLoadedScenes() const;
	};
}}
