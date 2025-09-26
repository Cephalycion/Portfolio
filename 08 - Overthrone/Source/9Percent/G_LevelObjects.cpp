/******************************************************************************/
/*!
\file   G_LevelObjects.cpp
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Kendrick Sim Hean Guan (100%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\brief
  This is the implementation file for a class that stores objects separated into
  layers, for updating/rendering them separately.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#include "pch.h"
#include "G_LevelObjects.h"

namespace Game {

	void LevelObjectsManager::RunMacro(OBJECTS_LAYER layer, const std::function<bool(const sPtr<Object>&)>& macro)
	{
		SafeRunMacro(layer, macro);
	}

	void LevelObjectsManager::Update(float dt)
	{
		for (int i = 0; i < OBJECTS_LAYER_TOTAL; ++i)
			Update(sCast<OBJECTS_LAYER>(i), dt);
	}
	void LevelObjectsManager::Update(OBJECTS_LAYER layer, float dt)
	{
		Flush(layer);
		SafeRunMacro(layer, std::bind([](const sPtr<Object>& obj, float dt) -> bool {
		UNREFERENCED_PARAMETER(dt);
			obj->Update();
			return true;
		}, std::placeholders::_1, dt));
		Flush(layer);
	}

	void LevelObjectsManager::Render(OBJECTS_LAYER layer)
	{
		for (const auto& obj : objects[layer])
			obj->Render();
	}

}
