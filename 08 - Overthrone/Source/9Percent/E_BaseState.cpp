/******************************************************************************/
/*!
\file   E_BaseState.cpp
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Marc Alviz Evangelista (100%)
\par    email: marcalviz.e\@digipen.edu
\par    DigiPen login: marcalviz.e

\brief
  This is the implementation file for all member functions of class BaseState.
  Used to inherit for different states in different state machines.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#include "pch.h"
#include "E_BaseState.h"

namespace Engine {
	BaseState::BaseState(BaseStateMachine* ref)
		: fsm(ref)
		, actTimer(0)
	{
	}

	BaseState::~BaseState()
	{

	}
}