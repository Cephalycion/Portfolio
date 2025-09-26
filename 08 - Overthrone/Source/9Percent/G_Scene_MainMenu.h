/******************************************************************************/
/*!
\file   G_Scene_MainMenu.h
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Marc Alviz Evangelista (90%)
\par    email: marcalviz.e\@digipen.edu
\par    DigiPen login: marcalviz.e

\author Kendrick Sim Hean Guan (5%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\author Ryan Chan Kuan Fu (5%)
\par    email: c.kuanfuryan\@digipen.edu
\par    DigiPen login: c.kuanfuryan

\brief
  This is the interface file for all member functions of class Scene_MainMenu

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#pragma once
#include "pch.h"
#include "E_SceneBase.h"
#include "E_Button.h"
#include "E_Camera.h"
#include "E_Text.h"
#include "E_Constants.h"
#include "E_Sprite.h"


namespace Game {
	using namespace Engine;
	using namespace Engine::Scene;
	
	class Scene_MainMenu : public Scene_Base
	{
	protected:
		sPtr<Camera> menuCam;
		sPtr<Text> titleText;

		sPtr<Object> background;

		sPtr<Button> playBtn;		// To start the game
		sPtr<Button> settingsBtn;	// Switch scene to go to options/settings page
		sPtr<Button> creditsBtn;	// Switch scene to go to credits page
		sPtr<Button> controlsBtn;	// Switch scene to go to controls/tutorial/how to play page
		sPtr<Button> quitBtn;		// Quits out of the game

		sPtr<Object> confirm;
		sPtr<Button> yes;
		sPtr<Button> no;

		bool atConfirm;

		std::vector<sPtr<Button>> btnList;

	public:
		Scene_MainMenu();

		virtual void Init() override;
		virtual void Update() override;
		virtual void Render() override;
		virtual void Exit() override;

		void MoveToState(BUTTON_PRESS state);
	};
}