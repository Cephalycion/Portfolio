/******************************************************************************/
/*!
\file   G_TextLayer.cpp
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Kendrick Sim Hean Guan (100%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\brief
  This is the implementation file for a class that stores text objects separated
  into layers, for updating/rendering them separately.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#include "pch.h"
#include "G_TextLayer.h"

namespace Game {
	
	using namespace Engine;

	void TextLayerManager::Update(float dt)
	{
		// Text objects don't have an update function.
		IObjectLayerManager::Update(dt);
	}

	void TextLayerManager::Render(TEXT_LAYER layer)
	{
		SafeRunMacro(layer, [](const sPtr<IText> text) -> bool {
			text->Render();
			return true;
		});
	}

}

