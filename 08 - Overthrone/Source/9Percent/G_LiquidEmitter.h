/******************************************************************************/
/*!
\file   G_LiquidEmitter.h
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
  of a class called LiquidEmitter.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#pragma once
#include "G_BlockEntity.h"
namespace Game
{
	class LiquidEmitter : public BlockEntity
	{
		int curremitCount= 0;
	public:
		BLOCK_TYPE emittedBlock = BT_NONE;
		bool isActive = false;
		int emitCount = -1;

		Block* blockReference = nullptr;

		void Init() override;
		void Update() override;
		void OnActivate() override;
	};
}