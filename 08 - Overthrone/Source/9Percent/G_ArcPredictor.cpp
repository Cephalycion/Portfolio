/******************************************************************************/
/*!
\file   G_ArcPredictor.cpp
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

#include "pch.h"
#include "G_ArcPredictor.h"
#include "E_Sprite.h"
#include "E_IOManager.h"

namespace Game
{
	void ArcPredictor::Init()
	{
		// Transparency decreases with each successive unit
		f32 transparency{ 1.0f };
		f32 transparencyStep{ 1.0f / sCast<f32>(C_arcPredictorLength) };

		for (s32 i = 0; i < C_arcPredictorLength; ++i)
		{
			f32 t = sCast<f32>(i) * C_arcPredictorInterval;
			sPtr<AP_UNIT> unit = std::make_shared<AP_UNIT>();
			unit->obj = Engine::ObjectBuilder::CreateGenericSquare(
				Engine::Graphics::Transform(0.0f, 0.0f, 0.0f, C_arcUnitSize, C_arcUnitSize),
				Engine::SpriteBuilder::CreateSpriteSquare(Engine::IOManager::GetFilePath(Engine::PNG_CIRCLE)),
				transparency
			);
			unit->timeInitial = t;
			unit->time = t;
			units.push_back(unit);
			transparency -= transparencyStep;
		}
	}

	void ArcPredictor::SetPredictor(Vec2 startPos, Vec2 throwVel)
	{
		for (int i = 0; i < C_arcPredictorLength; ++i)
		{
			Vec2 oMove = throwVel + Engine::C_physicsGravity * units[i]->time * 0.5f;
			units[i]->obj->SetPosition(startPos + oMove * units[i]->time);
		}
	}

	void ArcPredictor::Render()
	{
		for (int i = 0; i < C_arcPredictorLength; ++i)
		{
			units[i]->obj->Render();
		}
	}
}