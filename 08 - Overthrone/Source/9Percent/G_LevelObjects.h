/******************************************************************************/
/*!
\file   G_LevelObjects.h
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Kendrick Sim Hean Guan (100%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\brief
  This is an interface file for a class that stores objects separated into
  layers, for updating/rendering them separately.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#pragma once
#include "pch.h"
#include "E_Object.h"
#include "E_ObjectLayer.h"

namespace Game {
	
	using namespace Engine;

	enum OBJECTS_LAYER
	{
		OBJECTS_LAYER_FOREGROUND = 0,
		OBJECTS_LAYER_BACKGROUND,

		// Number represents parallax percentage
		// Parallax strength is specified at the render call in the scene.
		OBJECTS_LAYER_PARALLAX_25,
		OBJECTS_LAYER_PARALLAX_50,
		OBJECTS_LAYER_PARALLAX_75,

		OBJECTS_LAYER_SKYBOX,

		OBJECTS_LAYER_TOTAL
	};

	class LevelObjectsManager : public IObjectLayerManager<Object, OBJECTS_LAYER, OBJECTS_LAYER_TOTAL>
	{
	protected:

	public:
		// Run a macro on every object within a layer. Macro returns false to remove the object from the layer.
		void RunMacro(OBJECTS_LAYER layer, const std::function<bool(const sPtr<Object>&)>& macro);

		virtual void Update(float dt) override;
		virtual void Update(OBJECTS_LAYER layer, float dt);
		virtual void Render(OBJECTS_LAYER layer) override;

	};

}
