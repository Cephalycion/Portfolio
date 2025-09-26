/******************************************************************************/
/*!
\file   G_TextLayer.h
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Kendrick Sim Hean Guan (100%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\brief
  This is an interface file for a class that stores text objects separated
  into layers, for updating/rendering them separately.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#pragma once
#include "pch.h"
#include "E_ObjectLayer.h"

namespace Game {

	using namespace Engine;

	enum TEXT_LAYER
	{
		TEXT_LAYER_INGAME_OVERLAY = 0,	// Sign text
		TEXT_LAYER_SCREEN_OVERLAY,		// Finish/Dead text

		TEXT_LAYER_TOTAL
	};

	class TextLayerManager : public IObjectLayerManager<IText, TEXT_LAYER, TEXT_LAYER_TOTAL>
	{
	protected:

	public:
		virtual void Update(float dt) override;
		virtual void Render(TEXT_LAYER layer) override;

	};

}
