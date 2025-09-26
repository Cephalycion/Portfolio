/******************************************************************************/
/*!
\file   G_DoorBlock.h
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
  of a class called DoorBlock.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#pragma once
#include "G_BlockEntity.h"
namespace Game
{
	class DoorBlock : public BlockEntity
	{
		Vec2 startpoint = MakeVec2(0.0f, 0.0f);
		Vec2 midpoint = MakeVec2(0.0f,0.0f);
		Vec2 openDir = MakeVec2(0.0f, 0.0f);
		f32 length=0.0f;

		f32 openState=0.0f;
		std::vector<Block*> coveredBlocks;
	public:
		enum DOOR_DIRECTION {
			DIR_DOWN,
			DIR_LEFT,
			DIR_RIGHT,
			DIR_UP,
		};
		DOOR_DIRECTION dir = DIR_DOWN;
		f32 doorSpeed= 1.0f;


		bool isOpen = false;
		void Search();
		void Init() override;
		void Update() override;
		void OnActivate() override;
	};
}