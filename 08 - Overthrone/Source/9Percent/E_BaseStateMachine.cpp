/******************************************************************************/
/*!
\file   E_BaseStateMachine.cpp
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Marc Alviz Evangelista (100%)
\par    email: marcalviz.e\@digipen.edu
\par    DigiPen login: marcalviz.e

\brief
  This is the implementation file for all member functions of class EnemyBase.
  This includes utility functions

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#include "pch.h"
#include "E_BaseStateMachine.h"

namespace Engine {
	BaseStateMachine::BaseStateMachine()
	{
		mFirstRun = true;
		mCurrentState = nullptr;
	}

	BaseStateMachine::~BaseStateMachine()
	{
		for (auto& pair : mStates) {
			delete pair.second;
		}
	}

	void BaseStateMachine::Init(BaseState* initialState)
	{
		mCurrentState = initialState;
	}

	void BaseStateMachine::Update()
	{
		if (mFirstRun)
		{
			mFirstRun = false;
			mCurrentState->Enter();
		}
		mCurrentState->Update();
	}

	void BaseStateMachine::ChangeState(int index)
	{
		mCurrentState->Exit();
		mCurrentState = mStates[index];
		mCurrentState->Enter();
	}

	void BaseStateMachine::InsertState(BaseState* state, int index)
	{
		mStates.emplace(index, state);
		mStateIndexes.emplace(state, index);

	}

	BaseState* BaseStateMachine::GetState(int index)
	{
		return mStates[index];
	}

	int BaseStateMachine::GetStateIndex()
	{
		return mStateIndexes[mCurrentState];
	}
}