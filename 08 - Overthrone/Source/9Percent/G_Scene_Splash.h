/******************************************************************************/
/*!
\file   G_Scene_Splash.h
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Kendrick Sim Hean Guan (100%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\brief
  This is an interface file for the splash screen.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#pragma once
#include "pch.h"
#include "E_SceneBase.h"
#include "E_Object.h"

namespace Game {

	using namespace Engine;

	class Scene_Splash : public Scene::Scene_Base
	{
	protected:
		std::vector<sPtr<Object>> logoObjs;
		float logoTimer;

		bool IsSkipKeyPressed() const;

	public:
		Scene_Splash();

		virtual void Init() override;
		virtual void Update() override;
		virtual void Render() override;
		virtual void Exit() override;

	};

	class Object9PercentLogo : public Object
	{
	protected:
		sPtr<Sprite> logoGlitchSprite;
		bool isGlitched;
		int timesLeftGlitch;
		float timer;

	public:
		Object9PercentLogo();

		virtual void Update() override;
		virtual void Render() override;

	};
}
