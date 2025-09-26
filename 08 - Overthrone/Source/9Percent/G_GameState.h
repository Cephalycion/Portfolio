/******************************************************************************/
/*!
\file   G_GameState.h
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

#pragma once

namespace Game {

	class GameStateManager
	{
	protected:
		bool isLevelClear; // Whether the player finished the level.
		bool isFocusBossDying; // Whether the camera is focusing on the boss dying.

	public:
		GameStateManager();

		bool GetIsLevelClear() const;
		void SetIsLevelClear(bool newIsLevelClear);
		bool GetIsFocusBossDying() const;
		void SetIsFocusBossDying(bool newIsFocusBossDying);

	};

}
