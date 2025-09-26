/******************************************************************************/
/*!
\file   G_Sign.cpp
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Kendrick Sim Hean Guan (100%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\brief
  This is the implementation file for the tutorial sign in the game.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#include "pch.h"
#include "G_Sign.h"
#include "G_TextLayer.h"
#include "E_Camera.h"
#include "G_Player.h"

namespace Game {

	using namespace Engine;

	BG_Sign_Text::BG_Sign_Text(const std::string& text)
		: WrapText(text, 800.0f, 0.3f)
		, isActive(false)
	{
		SetVAlign(TEXT_ALIGN_BOTTOM);
		SetHasShadow(true);
	}

	void BG_Sign_Text::SetIsActive(bool newIsActive)
	{
		isActive = newIsActive;
	}

	void BG_Sign_Text::Render()
	{
		if (isActive)
			WrapText::Render();
	}


	BG_Sign::BG_Sign(const Vec2& position, const std::string& text)
		: Object(
			Transform(position, 0.0f, MakeVec2(50.0f, 50.0f)),
			SpriteBuilder::CreateSpriteSquare(C_bTexFilepath + std::string("Sign/Sign.png")),
			1.0f,
			C_colorWhite)
		, textObj(new BG_Sign_Text(text))
	{
		// Register the text to the text layer manager.
		Singleton<TextLayerManager>::Instance()->AddObject(TEXT_LAYER_INGAME_OVERLAY, textObj);
		// Leave removing the sign text to the scene.
	}

	void BG_Sign::Update()
	{
		bool isActive = Vec2DistanceSquared(Singleton<Player>::Instance()->GetPosition(), transform.GetPosition()) < C_signDist * C_signDist;
		textObj->SetIsActive(isActive);
		if (isActive)
			textObj->SetPosition(transform.GetPosition() - Singleton<Camera>::Instance()->GetPosition() + MakeVec2(0.0f, C_blockLength));
	}

	void BG_Sign::Render()
	{
		Object::Render();
		// Let Text Layer Manager handle rendering the sign text so that the text can be rendered above
		// objects that are above this sign.
	}

}
