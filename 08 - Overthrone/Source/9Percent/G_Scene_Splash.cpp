/******************************************************************************/
/*!
\file   G_Scene_Splash.cpp
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Kendrick Sim Hean Guan (100%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\brief
  This is the implementation file for the splash screen.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#include "pch.h"
#include "G_Scene_Splash.h"
#include "E_Engine.h"
#include "G_Constants.h"

namespace Game {

	using namespace Engine;

	Scene_Splash::Scene_Splash()
		: logoTimer(0.0f)
	{
	}

	bool Scene_Splash::IsSkipKeyPressed() const
	{
		return AEInputCheckTriggered(AEVK_LBUTTON) ||
			   AEInputCheckTriggered(AEVK_RBUTTON) ||
			   AEInputCheckTriggered(AEVK_RETURN) ||
			   AEInputCheckTriggered(AEVK_SPACE) ||
			   AEInputCheckTriggered(AEVK_ESCAPE);
	}

	void Scene_Splash::Init()
	{
		// Digipen Logo
		logoObjs.emplace_back(ObjectBuilder::CreateSquare(
			Transform(0.0f, 0.0f, 0.0f, 1280.0f, 373.5f),
			SpriteBuilder::CreateSpriteSquare(C_filePaths[PNG_SPLASH_DIGIPEN])
		));
		// FMOD Logo
		logoObjs.emplace_back(ObjectBuilder::CreateSquare(
			Transform(0.0f, 30.0f, 0.0f, 728.0f, 192.0f),
			SpriteBuilder::CreateSpriteSquare(C_filePaths[PNG_SPLASH_FMOD])
		));
		// 9Percent Logo
		logoObjs.emplace_back(new Object9PercentLogo());

		logoTimer = C_logoLength;
	}

	void Scene_Splash::Update()
	{
#ifdef _DEBUG
		if (C_skipSplash)
		{
			Singleton<ProgramEngine>::Instance()->SwitchToScene(Scene::SCENE_MAINMENU);
			return;
		}
#endif

		if (logoObjs.empty())
			return;

		for (logoTimer -= C_frametime; logoTimer < 0.0f; logoTimer += C_logoLength)
		{
			logoObjs.erase(logoObjs.begin());
			if (logoObjs.empty())
			{
				Singleton<ProgramEngine>::Instance()->SwitchToScene(Scene::SCENE_MAINMENU);
				return;
			}
		}

		(*logoObjs.begin())->Update();

		// AlphaEngine reports the mouse as triggered in the first frame if the user double clicks the exe,
		// so we only test for clicks after the first frame.
		if (IsSkipKeyPressed() && C_fpsCount > 1)
			logoTimer = min(C_logoFadeLength, min(logoTimer, C_logoLength - logoTimer));

		const sPtr<Object>& logo = *logoObjs.begin();
		if (logoTimer < C_logoFadeLength)
			logo->SetTransparency(logoTimer / C_logoFadeLength);
		else if (logoTimer > C_logoLength - C_logoFadeLength)
			logo->SetTransparency((C_logoLength - logoTimer) / C_logoFadeLength);
		else
			logo->SetTransparency(1.0f);
	}

	void Scene_Splash::Render()
	{
		if (logoObjs.empty())
			return;

		(*logoObjs.begin())->Render();
	}

	void Scene_Splash::Exit()
	{
		logoObjs.clear();
	}


	Object9PercentLogo::Object9PercentLogo()
		: Object(Transform(0.0f, 0.0f, 0.0f, 600.0f, 600.0f), SpriteBuilder::CreateSpriteSquare(C_filePaths[PNG_SPLASH_9PERCENT]), 1.0f, C_colorWhite)
		, logoGlitchSprite(SpriteBuilder::CreateSpriteSquare(C_filePaths[PNG_SPLASH_9PERCENT_GLITCH]))
		, isGlitched(false)
		, timesLeftGlitch(C_logo9PercentTimesGlitch)
		, timer(C_logo9PercentTimeTillGlitch)
	{
	}

	void Object9PercentLogo::Update()
	{
		if (!timesLeftGlitch)
			return;

		timer -= C_frametime;
		if (timer <= 0.0f)
		{
			if (isGlitched)
				--timesLeftGlitch;

			isGlitched = !isGlitched;
			timer = (isGlitched ? C_logo9PercentGlitchLength : C_logo9PercentReglitchTime);
		}
	}

	void Object9PercentLogo::Render()
	{
		if (isGlitched)
			logoGlitchSprite->Render(transform, transparency, colorTint);
		else
			Object::Render();
	}

}
