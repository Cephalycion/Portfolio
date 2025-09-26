/******************************************************************************/
/*!
\file   G_Sign.h
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Kendrick Sim Hean Guan (100%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\brief
  This is an interface file for the tutorial sign in the game.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#pragma once
#include "pch.h"
#include "E_Object.h"

namespace Game
{
	using namespace Engine;

	// Specialize to enable enabling/disabling text rendering
	class BG_Sign_Text : public WrapText
	{
	protected:
		bool isActive; // Whether the player is in range of this sign

	public:
		BG_Sign_Text(const std::string& text);

		void SetIsActive(bool newIsActive);

		virtual void Render() override;
	};

	class BG_Sign : public Object
	{
	protected:
		sPtr<BG_Sign_Text> textObj;

	public:
		BG_Sign(const Vec2& position, const std::string& text);

		virtual void Update() override;
		virtual void Render() override;

	};
}
