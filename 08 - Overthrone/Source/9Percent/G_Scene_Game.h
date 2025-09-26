/******************************************************************************/
/*!
\file   G_Scene_Game.h
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Kendrick Sim Hean Guan (40%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\author Ryan Chan Kuan Fu (30%)
\par    email: c.kuanfuryan\@digipen.edu
\par    DigiPen login: c.kuanfuryan

\author Marc Alviz Evangelista (21%)
\par    email: marcalviz.e\@digipen.edu
\par    DigiPen login: marcalviz.e

\author Matthew Chan Shao Jie (9%)
\par    email: m.chan\@digipen.edu
\par    DigiPen login: m.chan

\brief
  This is the interface file for all member functions of a class called
  Scene_Game.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#pragma once
#include "pch.h"
#include "E_SceneBase.h"
#include "E_Constants.h"
#include "E_Object.h"
#include "E_Camera.h"
#include "E_Text.h"
#include "G_Map.h"
#include "E_Button.h"
#include <vector>

#include "G_EnemyTurret.h"

namespace Game {

	using namespace Engine;
	using namespace Engine::Scene;

	class Scene_Game : public Scene_Base
	{
	protected:
		void LoadNextLevel(const sPtr<Map>& mapPtr);

	public:
		virtual void Init() override;
		virtual void Update() override;
		virtual void Render() override;
		virtual void Exit() override;
	};

}
