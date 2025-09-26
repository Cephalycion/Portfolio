/******************************************************************************/
/*!
\file   G_Scene_Pause.h
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Marc Alviz Evangelista (100%)
\par    email: marcalviz.e\@digipen.edu
\par    DigiPen login: marcalviz.e

\brief
  This is the implementation file for all member functions of class Scene_Pause

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

namespace Game {
    using namespace Engine;
    using namespace Engine::Scene;

    class Scene_Pause : public Scene_Base
    {
    protected:
        sPtr<Text> title;

        sPtr<Button> quitBtn;
        sPtr<Button> unpauseBtn;
        sPtr<Button> helpBtn;
        sPtr<Object> pauseAnim;
        sPtr<Object> menu;

        sPtr<Object> confirm;
        sPtr<Button> yes;
        sPtr<Button> no;

        bool animPlayed;
        bool atConfirm;
        f32 animTimer;
        std::vector<sPtr<Button>> btnList;
    public:
        Scene_Pause();
        virtual void Init() override;
        virtual void Update() override;
        virtual void Render() override;
        virtual void Exit() override;

        void PauseButtonPress(BUTTON_PRESS press);

    };
}

