/******************************************************************************/
/*!
\file   G_ButtonBlock.cpp
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
  of a class called ButtonBlock.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#include "pch.h"
#include "G_ButtonBlock.h"
#include "G_LevelObjects.h"

namespace Game
{
	void ButtonBlock::Init()
	{
		UnsetLayerMask({ Engine::Physics::MASK_ALL });
		SetPosition(Engine::Singleton<Map>::Instance()->MapToWorldPos(x, y)+MakeVec2(0.0f,-C_blockLength*0.4f));
		SetColorTint(Engine::MakeColor(225, 64, 64, 255));
		inited = true;
	}
	void ButtonBlock::Update()
	{
		if (!inited)
			Init();
		if (targetEntities.size()<=0)
		{
			Engine::Singleton<LevelObjectsManager>::Instance()->RunMacro(OBJECTS_LAYER_FOREGROUND, [&](const sPtr<Engine::Object>& go) -> bool {
				sPtr<BlockEntity> be = std::dynamic_pointer_cast<BlockEntity>(go);
				if (be != nullptr)
				{
					if ((be)->ID == ID && !(be->x == x && be->y == y))
					{
						targetEntities.push_back(be);
					}
				}
				return true;
			});
		}

		if (residentBlock->type != BT_NONE)
		{
			if (!isPressed)
				OnActivate();
		}
		else
		{
			if (isPressed)
				OnActivate();
		}
#ifdef _DEBUG
		if (AEInputCheckTriggered(AEVK_O))
		{
			for (sPtr<BlockEntity> b : targetEntities)
			{
				b->OnActivate();
			}
		}
#endif
	}
	void ButtonBlock::OnActivate()
	{
		isPressed = !isPressed;
		if (isPressed)
		{
			SetPosition(Engine::Singleton<Map>::Instance()->MapToWorldPos(x, y) + MakeVec2(0.0f, -C_blockLength * 0.6f));
			SetColorTint(Engine::MakeColor(64, 180, 64, 255));
		}
		else
		{
			SetPosition(Engine::Singleton<Map>::Instance()->MapToWorldPos(x, y) + MakeVec2(0.0f, -C_blockLength * 0.4f));
			SetColorTint(Engine::MakeColor(225, 64,64, 255));
		}
		for (sPtr<BlockEntity> b : targetEntities)
		{
			b->OnActivate();
		}
	}
}
