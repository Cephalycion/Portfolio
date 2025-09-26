/******************************************************************************/
/*!
\file   G_HealthDisplay.h
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Ryan Chan Kuan Fu (100%)
\par    email: c.kuanfuryan\@digipen.edu
\par    DigiPen login: c.kuanfuryan

\brief
  Uses a bunch of line renderers and square objects to render healthbars and
  even includes a flashing animation (using sine wave) that indicates that an
  entity is invincible for a short period after being attacked!

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
	class HealthDisplay
	{
	private:
		sPtr<Engine::LineRenderer> bar;
		sPtr<Engine::LineRenderer> barBackground;
		sPtr<Engine::LineRenderer> barOutline;
		sPtr<Engine::Object> entityFlash;

		bool onChanged;
		f32  timeStep;
		f32  duration;

		Vec2 H_posStart;
		f32  H_length;
		f32  H_width;
		f32  H_outlineThickness;

	public:
		HealthDisplay() : 
			bar					{ nullptr }, 
			barBackground		{ nullptr }, 
			barOutline			{ nullptr }, 
			entityFlash			{ nullptr }, 
			onChanged			{ false },
			timeStep			{0.0f},
			duration			{0.0f},
			H_posStart			{},
			H_length			{0.0f},
			H_width				{0.0f},
			H_outlineThickness	{0.0f} {};
		~HealthDisplay();

		void Init(
			f32 entityWidth = C_blockLength, 
			f32 entityHeight = C_blockLength,
			Vec2 posStart = C_healthPosStart,
			f32 length = C_healthLength,
			f32 width = C_healthWidth,
			f32 outlineThickness = C_healthOutlineThickness);
		void SetHealthDisplay(f32 ratio, Vec2 camPos, Vec2 entityPos, f32 entityRot);
		void OnChange();
		void Render();
	};
}