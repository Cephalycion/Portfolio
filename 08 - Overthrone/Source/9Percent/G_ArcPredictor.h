/******************************************************************************/
/*!
\file   G_ArcPredictor.h
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Ryan Chan Kuan Fu (100%)
\par    email: c.kuanfuryan\@digipen.edu
\par    DigiPen login: c.kuanfuryan

\brief
  Trying to make those dot prediction arcs from Angry Birds. Used by the player
  throwing ability and accurately predicts where the block will go (without
  collisions, of course, that would be crazy.)

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#pragma once
#include "pch.h"
#include "E_Object.h"
#include "G_Constants.h"

namespace Game
{
	typedef struct AP_UNIT
	{
		sPtr<Engine::Object> obj;
		f32 time;
		f32 timeInitial;
		AP_UNIT() : obj{ nullptr }, time{ 0.0f }, timeInitial{ 0.0f } {}
	} AP_UNIT;

	class ArcPredictor
	{
	protected:
		std::vector<sPtr<AP_UNIT>> units;

	public:
		void Init();
		void SetPredictor(Vec2 startPos, Vec2 throwVel);
		void Render();
	};
}