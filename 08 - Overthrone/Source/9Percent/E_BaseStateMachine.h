/******************************************************************************/
/*!
\file   E_BaseStateMachine.h
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Marc Alviz Evangelista (100%)
\par    email: marcalviz.e\@digipen.edu
\par    DigiPen login: marcalviz.e

\brief
  This is the interface file for all member functions of class EnemyBase.
  This includes utility functions

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#pragma once
#include "pch.h"
#include "E_BaseState.h"
#include <map>
#include "G_Map.h"

namespace Engine
{
	// When making states for each state machine,
	// Put them all in one file
	class BaseStateMachine
	{
	protected:
		BaseState* mCurrentState;
		std::map<int, BaseState*> mStates;
		std::map<BaseState*, int> mStateIndexes;

		bool mFirstRun;
	public:
		BaseStateMachine();
		~BaseStateMachine();

		void Init(BaseState* initialState);
		void Update();
		void ChangeState(int index);
		void InsertState(BaseState* state, int index);
		BaseState* GetState(int index);
		int GetStateIndex();
	};


}

