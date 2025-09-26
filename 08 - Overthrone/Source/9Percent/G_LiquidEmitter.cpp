/******************************************************************************/
/*!
\file   G_LiquidEmitter.cpp
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
  of a class called LiquidEmitter.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#pragma once
#include "pch.h"
#include "G_LiquidEmitter.h"
namespace Game
{
	void LiquidEmitter::Init()
	{
		SetPosition(Engine::Singleton<Map>::Instance()->MapToWorldPos(x, y));
		blockReference = Engine::Singleton<Map>::Instance()->GetBlockAtIndex(x, y);
		UnsetLayerMask({ Engine::Physics::MASK_ALL });
		if (isActive)
		{
			blockReference->FillBlock(emittedBlock);
			blockReference->UpdateTexture();
		}

		curremitCount = emitCount;

		inited = true;
	}
	void LiquidEmitter::Update()
	{
		if (!inited)
		{
			Init();
			return;
		}
		if (isActive)
		{
			if (blockReference->type == BT_NONE)
			{				
				if (curremitCount <= 0&&emitCount>0)
				{
					isActive = false;
					return;
				}

				if (emitCount > 0)
					--curremitCount;
				else
					curremitCount = 69;

				blockReference->FillBlock(emittedBlock);
				blockReference->UpdateTexture();

			}
		}
	}

	void LiquidEmitter::OnActivate()
	{
		isActive = true;
	}
}