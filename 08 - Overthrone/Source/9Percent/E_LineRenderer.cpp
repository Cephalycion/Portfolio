/******************************************************************************/
/*!
\file   E_LineRenderer.cpp
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

#include "pch.h"
#include "E_LineRenderer.h"

namespace Engine
{
	void LineRenderer::Init()
	{
		object = ObjectBuilder::CreateSquare(Engine::Graphics::Transform(0.0f, 0.0f, 0.0f, 10.0f, 10.0f));
		object->SetColorTint(Engine::C_colorWhite);
	}
	void LineRenderer::SetLine(Vec2 posA, Vec2 posB, f32 width)
	{
		Vec2 vec = posA - posB;
		f32 scale = Vec2Length(vec);
		f32 rot = atan2f(vec.y, vec.x) * 180.0f / PI;
		Vec2 translate = MakeVec2((posA.x + posB.x) * 0.5f, (posA.y + posB.y) * 0.5f);

		object->SetScale(MakeVec2(scale, width));
		object->SetRotation(rot);
		object->SetPosition(translate);
	}

	void LineRenderer::SetColor(Color c)
	{
		object->SetColorTint(c);
	}

	void LineRenderer::Render()
	{
		object->Render();
	}

	LineRenderer::~LineRenderer()
	{
	}
}