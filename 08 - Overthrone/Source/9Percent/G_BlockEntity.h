/******************************************************************************/
/*!
\file   G_BlockEntity.h
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
  of a class called BlockEntity.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#pragma once
#include "G_Block.h"
#include "E_Object.h"
#include "G_Map.h"
#include <string>
namespace Game
{
	class BlockEntity : public Engine::PhysicsObject
	{
	public:
		bool inited = false;
		s32 x=0, y=0;
		std::string ID="";
		virtual void Init() = 0;
		virtual void OnActivate() = 0;
	};
}