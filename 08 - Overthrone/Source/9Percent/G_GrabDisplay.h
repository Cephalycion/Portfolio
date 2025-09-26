/******************************************************************************/
/*!
\file   G_GrabDisplay.h
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Ryan Chan Kuan Fu (100%)
\par    email: c.kuanfuryan\@digipen.edu
\par    DigiPen login: c.kuanfuryan

\brief
  Renders a line and a ball from one position to another. Used in Player grab
  ability to show the raycast determining which block to pick up. Supports
  colour changing.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#pragma once
#include "pch.h"
#include "E_Object.h"
#include "G_Constants.h"
#include "E_LineRenderer.h"

namespace Game
{
	class GrabDisplay
	{
	private:
		sPtr<Engine::Object> grabPoint;
		sPtr<Engine::LineRenderer> grabLine;
		bool renderThisFrame;
	public:
		GrabDisplay() : grabPoint{ nullptr }, grabLine{ nullptr }, renderThisFrame{ false } {};
		~GrabDisplay();

		void Init();
		void SetDisplay(
			Vec2 posA,
			Vec2 posB,
			f32 width = 10.0f);
		void SetColor(Engine::Color c);
		void Render();

		void RenderThisFrame(bool r);
	};
}