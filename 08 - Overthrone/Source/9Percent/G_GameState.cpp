/******************************************************************************/
/*!
\file   G_GameState.cpp
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Kendrick Sim Hean Guan (100%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\brief
  This is an interface file for a class that stores various global game
  state variables.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#include "pch.h"
#include "G_GameState.h"

namespace Game {

	GameStateManager::GameStateManager()
		: isLevelClear(false)
		, isFocusBossDying(false)
	{
	}

	bool GameStateManager::GetIsLevelClear() const
	{
		return isLevelClear;
	}
	void GameStateManager::SetIsLevelClear(bool newIsLevelClear)
	{
		isLevelClear = newIsLevelClear;
	}

	bool GameStateManager::GetIsFocusBossDying() const
	{
		return isFocusBossDying;
	}
	void GameStateManager::SetIsFocusBossDying(bool newIsFocusBossDying)
	{
		isFocusBossDying = newIsFocusBossDying;
	}

}
