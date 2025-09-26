/******************************************************************************/
/*!
\file   G_HealthDisplay.cpp
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

#include "pch.h"
#include "G_HealthDisplay.h"

namespace Game
{
	HealthDisplay::~HealthDisplay()
	{
	}

	void HealthDisplay::Init(
		f32 entityWidth, 
		f32 entityHeight,
		Vec2 posStart,
		f32 length,
		f32 width,
		f32 outlineThickness)
	{
		H_posStart = posStart;
		H_length = length;
		H_width = width;
		H_outlineThickness = outlineThickness;

		// Health bar
		bar = std::make_shared<Engine::LineRenderer>();
		bar->Init();
		bar->SetColor(Engine::C_colorGreen);
		bar->SetLine(H_posStart, MakeVec2(H_posStart.x + H_length, H_posStart.y), H_width);

		// Background
		barBackground = std::make_shared<Engine::LineRenderer>();
		barBackground->Init();
		barBackground->SetColor(Engine::C_colorBlack);
		barBackground->SetLine(H_posStart, MakeVec2(H_posStart.x + H_length, H_posStart.y), H_width);

		// Outline
		barOutline = std::make_shared<Engine::LineRenderer>();
		barOutline->Init();
		barOutline->SetColor(Engine::MakeColor(255, 255, 255, 128));
		barOutline->SetLine(
			MakeVec2(H_posStart.x - H_outlineThickness, H_posStart.y), 
			MakeVec2(H_posStart.x + H_length + H_outlineThickness, H_posStart.y), 
			H_width + H_outlineThickness * 2.0f);

		entityFlash = Engine::ObjectBuilder::CreateGenericSquare(Engine::Graphics::Transform(0.0f, 0.0f, 0.0f, entityWidth, entityHeight));
	}

	void HealthDisplay::SetHealthDisplay(f32 ratio, Vec2 camPos, Vec2 entityPos, f32 entityRot)
	{
		// Set positions of components
		bar->SetLine(
			H_posStart + camPos, 
			MakeVec2(H_posStart.x + ratio * H_length, H_posStart.y) + camPos, 
			H_width);

		barBackground->SetLine(
			H_posStart + camPos, 
			MakeVec2(H_posStart.x + H_length, H_posStart.y) + camPos, 
			H_width);

		barOutline->SetLine(
			MakeVec2(H_posStart.x - H_outlineThickness, H_posStart.y) + camPos,
			MakeVec2(H_posStart.x + H_length + H_outlineThickness, H_posStart.y) + camPos,
			H_width + H_outlineThickness * 2.0f);

		entityFlash->SetPosition(entityPos);
		entityFlash->SetRotation(entityRot);

		// If on changed, start flash "animation"
		if (onChanged)
		{
			onChanged = false;
			timeStep = 0.0f;
			duration = C_invincibilityTimer;
		}

		// Flash animation
		if (duration > 0.0f)
		{
			duration -= Engine::C_frametime;
			timeStep += Engine::C_frametime * C_healthFlashSpeed;
			entityFlash->SetTransparency(sinf(sCast<float>(timeStep)));
			barOutline->object->SetTransparency(max(sinf(sCast<float>(timeStep)), 0.5f));
		}
		else
		{
			entityFlash->SetTransparency(0.0f);
			barOutline->object->SetTransparency(0.5f);
		}
	}

	void HealthDisplay::OnChange()
	{
		onChanged = true;
	}

	void HealthDisplay::Render()
	{
		barOutline->Render();
		barBackground->Render();
		bar->Render();
		entityFlash->Render();
	}
}