/******************************************************************************/
/*!
\file   G_ButtonBlock.h
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Matthew Chan Shao Jie (100%)
\par    email: m.chan\@digipen.edu
\par    DigiPen login: m.chan

\brief
  This is the interface file for all member functions
  of a class called ButtonBlock.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#pragma once
#include "G_BlockEntity.h"
namespace Game
{
	class ButtonBlock : public BlockEntity
	{
		std::vector<sPtr<BlockEntity>> targetEntities;
	public:
		Block* residentBlock = nullptr;
		bool isPressed = false;
		void Init() override;
		void Update() override;
		void OnActivate() override;
	};
}