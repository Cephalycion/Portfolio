/******************************************************************************/
/*!
\file   G_GrabDisplay.cpp
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

#include "pch.h"
#include "G_GrabDisplay.h"
#include "E_IOManager.h"

namespace Game
{
	GrabDisplay::~GrabDisplay()
	{
	}

	void GrabDisplay::Init()
	{
		grabPoint = Engine::ObjectBuilder::CreateGenericSquare(
			Engine::Graphics::Transform(0.0f, 0.0f, 0.0f, C_grabSize, C_grabSize),
			Engine::SpriteBuilder::CreateSpriteSquare(Engine::IOManager::GetFilePath(Engine::PNG_CIRCLE))
		);
		grabLine = std::make_shared<Engine::LineRenderer>();
		grabLine->Init();
	}

	void GrabDisplay::SetDisplay(Vec2 posA, Vec2 posB, f32 width)
	{
		grabLine->SetLine(posA, posB, width);
		grabPoint->SetPosition(posB);
	}

	void GrabDisplay::SetColor(Engine::Color c)
	{
		grabLine->SetColor(c);
		grabPoint->SetColorTint(c);
	}

	void GrabDisplay::Render()
	{
		if (renderThisFrame)
		{
			grabPoint->Render();
			grabLine->Render();
		}
	}
	void GrabDisplay::RenderThisFrame(bool r)
	{
		renderThisFrame = r;
	}
}