/******************************************************************************/
/*!
\file   E_BaseState.h
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Marc Alviz Evangelista (100%)
\par    email: marcalviz.e\@digipen.edu
\par    DigiPen login: marcalviz.e

\brief
  This is the interace file for all member functions of class BaseState.
  This is used as a inheritance for different States used in different state machines.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#pragma once
#include "pch.h"
#include "E_Sprite.h"

namespace Engine {

	class BaseStateMachine;

	class BaseState
	{
	protected:
		 BaseStateMachine* fsm; // reference to fsm
		 f32 actTimer;
	public:
		BaseState(BaseStateMachine* ref);
		virtual ~BaseState();


		virtual void Enter() = 0;		// Set actionTimers for each state, set associated sprite in here
		virtual void Update() = 0;
		virtual void Exit() = 0;
	};

}

