/******************************************************************************/
/*!
\file   G_Scene_Credits.h
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Marc Alviz Evangelista (100%)
\par    email: marcalviz.e\@digipen.edu
\par    DigiPen login: marcalviz.e

\brief
  This is the interface file for all member functions of class Scene_Credits.

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
    class Scene_Credits : public Scene_Base
    {
    protected:
        sPtr<Button> backBtn;
        sPtr<Object> background;
        std::vector<sPtr<Button>> btnList;
        sPtr<Button> nextBtn;
        sPtr<Button> prevBtn;
        sPtr<Object> page1;
        sPtr<Object> page2;
        sPtr<Object> page3;

        s8 currPage;
        s8 maxPages;

    public:
        Scene_Credits();

        virtual void Init() override;
        virtual void Update() override;
        virtual void Render() override;
        virtual void Exit() override;

        void CreditsButtonPress(BUTTON_PRESS press);
    };

}

