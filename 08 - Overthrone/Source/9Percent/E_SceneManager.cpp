/******************************************************************************/
/*!
\file   E_SceneManager.cpp
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Kendrick Sim Hean Guan (100%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\brief
  This is the implementation file for a system that manages scenes.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#include "pch.h"
#include "E_SceneManager.h"

namespace Engine { namespace Scene {

	SceneManager::SceneManager()
		: firstUpdatingSceneIter(scenesVec.begin())
	{
	}
	SceneManager::~SceneManager()
	{
	}

	s64 SceneManager::QueuePushScene(SCENE_IDENTIFIER identifier)
	{
		addScenesVec.push_back(identifier);
		return scenesVec.size() - removeScenesVec.size() + addScenesVec.size() - 1;
	}

	void SceneManager::QueuePopScene()
	{
		// Don't pop if there are no loaded scenes
		if (!(scenesVec.size() - removeScenesVec.size()))
			return;

		auto removePtr = scenesVec.end() - 1 - removeScenesVec.size();
		removeScenesVec.emplace_back(*removePtr);
	}

	s64 SceneManager::QueueSwitchToScene(SCENE_IDENTIFIER identifier)
	{
		// Clear all queued scenes
		addScenesVec.clear();
		removeScenesVec.clear();

		// Remove all scenes
		for (auto removeIter = scenesVec.rbegin(); removeIter != scenesVec.rend(); ++removeIter)
			removeScenesVec.emplace_back(*removeIter);

		// Add the new scene
		return QueuePushScene(identifier);
	}

	void SceneManager::FlushChanges()
	{
		// Check if there are changes
		if (addScenesVec.size() + removeScenesVec.size() <= 0)
			return;

		// Flush the changes
		FlushRemoveScenes();
		FlushAddScenes();

		// Find the first scene that's updating (not blocked)
		firstUpdatingSceneIter = scenesVec.cend() - 1;
		while (firstUpdatingSceneIter != scenesVec.cbegin() && !(*firstUpdatingSceneIter)->GetUpdateBlocking())
			--firstUpdatingSceneIter;
	}
	void SceneManager::FlushAddScenes()
	{
		for (auto i = addScenesVec.begin(); i != addScenesVec.end(); ++i)
		{
			auto scene = Scene::CreateScene(*i);
			scenesVec.emplace_back(scene);
			scene->Init();
		}
		addScenesVec.clear();
	}
	void SceneManager::FlushRemoveScenes()
	{
		for (auto i = removeScenesVec.begin(); i != removeScenesVec.end(); ++i)
		{
			auto& scenePtr = *i;
			scenePtr->Exit();
			auto removeIter = std::find(scenesVec.begin(), scenesVec.end(), scenePtr);
			scenesVec.erase(removeIter);
		}
		removeScenesVec.clear();
	}

	std::vector<sPtr<Scene_Base>>::const_iterator SceneManager::GetFirstSceneIter() const
	{
		return scenesVec.cbegin();
	}
	std::vector<sPtr<Scene_Base>>::const_iterator SceneManager::GetFirstActiveSceneIter() const
	{
		return firstUpdatingSceneIter;
	}
	std::vector<sPtr<Scene_Base>>::const_iterator SceneManager::GetEndIter() const
	{
		return scenesVec.cend();
	}

	int SceneManager::GetNumLoadedScenes() const
	{
		return sCast<int>(scenesVec.size());
	}

	void SceneManager::Cleanup()
	{
		for (auto i = scenesVec.begin(); i != scenesVec.end(); ++i)
			(*i)->Exit();
	}
}}
