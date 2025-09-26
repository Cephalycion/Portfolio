/******************************************************************************/
/*!
\file   E_LineRenderer.h
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Ryan Chan Kuan Fu (100%)
\par    email: c.kuanfuryan\@digipen.edu
\par    DigiPen login: c.kuanfuryan

\brief
  Render a straight line! Colour changing supported. Used in various 
  applications such as turret raycast, predictor projectiles, healthbar, etc.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#pragma once
#include "pch.h"
#include "E_Object.h"

namespace Engine
{
	class LineRenderer
	{
	public:
		sPtr<Object> object;
		void Init();
		void SetLine(
			Vec2 posA, 
			Vec2 posB,
			f32 width = 10.0f);

		void SetColor(Color c);
		void Render();

		LineRenderer() :object(nullptr) {};
		~LineRenderer();
	};
}