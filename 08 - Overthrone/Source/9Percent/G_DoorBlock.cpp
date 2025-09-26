/******************************************************************************/
/*!
\file   G_DoorBlock.cpp
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Matthew Chan Shao Jie (100%)
\par    email: m.chan\@digipen.edu
\par    DigiPen login: m.chan

\brief
  This is the implementation file for all member functions
  of a class called DoorBlock.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#pragma once
#include "pch.h"
#include "G_DoorBlock.h"
#include "G_Physics.h"
#include "G_GameState.h"

namespace Game
{
	void DoorBlock::Init()
	{
		Search();
		for (size_t i = 0; i < coveredBlocks.size(); ++i)
		{
			coveredBlocks[i]->type = isOpen ? BT_NONE : BT_DOORCOVERED;
		}
		inited = true;
		Engine::Singleton<Game::PhysicsManager>::Instance()->RegisterSpecialStatic(physicsComp);
	}
	void DoorBlock::Search()
	{
		sPtr<Map> mapReference = Engine::Singleton<Map>::Instance();
		coveredBlocks.clear();
		startpoint = mapReference->MapToWorldPos(x, y);
		s32 xsearch = x;
		s32 ysearch = y;
		coveredBlocks.push_back(mapReference->GetBlockAtIndex(xsearch, ysearch));
		while (ysearch >= 0 && ysearch < mapReference->height && xsearch >= 0 && xsearch < mapReference->width)
		{
			Block* b = mapReference->GetBlockAtIndex(xsearch, ysearch);
			if (!b || b->type != BT_NONE)
			{
				if (x != xsearch || y != ysearch)
				{
					break;
				}
			}
			coveredBlocks.insert(coveredBlocks.begin(), b);
			switch (dir)
			{
			case DIR_DOWN:
				++ysearch;
				break;
			case DIR_UP:
				--ysearch;
				break;
			case DIR_LEFT:
				--xsearch;
				break;
			case DIR_RIGHT:
				++xsearch;
				break;
			}
		}
		midpoint = (mapReference->MapToWorldPos(xsearch, ysearch) + startpoint) / 2.0f;
		startpoint += Vec2Normalize(startpoint - midpoint) * C_blockLengthHalf;
		midpoint += Vec2Normalize(startpoint - midpoint) * C_blockLengthHalf;
		length = Vec2Length(midpoint - startpoint) * 2.0f;

	}
	void DoorBlock::Update()
	{
		if (Vec2LengthSquared(GetVelocity()) > Engine::C_physicsMaxVelocity * Engine::C_physicsMaxVelocity)
			SetVelocity(Vec2Normalize(GetVelocity()) * Engine::C_physicsMaxVelocity);
		if (Engine::Singleton<GameStateManager>::Instance()->GetIsLevelClear())
			return;
		if (!inited)
		{
			Init();
			return;
		}
		bool ismoving = openState > 0.0f && openState < 1.0f;
		if (isOpen)
		{
			openState += Engine::C_frametime * doorSpeed;
		}
		else
		{
			openState -= Engine::C_frametime * doorSpeed;
		}
		if (ismoving)
		{
			size_t tmp = sCast<size_t>(AEClamp(openState * coveredBlocks.size(), 0.0f, sCast<f32>(coveredBlocks.size() - 1)));
			for (size_t i = coveredBlocks.size() - 1; i >= tmp && i < coveredBlocks.size(); --i)
			{
				if(coveredBlocks[i]->type==BT_NONE||coveredBlocks[i]->type==BT_DOORCOVERED)
				coveredBlocks[i]->FillBlock(isOpen ? BT_NONE : BT_DOORCOVERED);
			}
		}
		SetLayerMask({ Engine::Physics::MASK_ENVIRONMENT, Engine::Physics::MASK_ENVIRONMENT_DYNAMIC, Engine::Physics::MASK_PLAYER });
		if(openState>=1.0f)
			UnsetLayerMask({ Engine::Physics::MASK_ALL });
		openState= AEClamp(openState, 0.0f, 1.0f);

		switch (dir)
		{
		case DIR_DOWN:
		case DIR_UP:
 			this->SetScale(MakeVec2(C_blockLength, length * (1.0f - openState)));
			break;
		case DIR_LEFT:
		case DIR_RIGHT:
			this->SetScale(MakeVec2(length * (1.0f - openState), C_blockLength));
			break;
		}
		this->SetPosition(startpoint+(midpoint-startpoint)* (1.0f - openState));

	}
	void DoorBlock::OnActivate()
	{
		isOpen = !isOpen;
		if (!isOpen)
			Search();
	}
}