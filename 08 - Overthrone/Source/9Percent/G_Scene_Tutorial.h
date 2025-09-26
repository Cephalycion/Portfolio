/******************************************************************************/
/*!
\file   G_Scene_Tutorial.h
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Marc Alviz Evangelista (100%)
\par    email: marcalviz.e\@digipen.edu
\par    DigiPen login: marcalviz.e

\brief
  This is the interface file for all member functions of class Scene_Tutorial,
  which is the How To Play screen.

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

    class Scene_Tutorial : public Scene_Base
    {
    protected:
        sPtr<Text> title;
        sPtr<Object> background;
        sPtr<Object> page1;
        sPtr<Object> moveSprite;
        sPtr<Object> jumpSprite;
        sPtr<Object> dashSprite;
        sPtr<Object> grabSprite;
        sPtr<Object> grappleSprite;

        sPtr<Object> page2;

        sPtr<Button> backBtn;
        sPtr<Button> nextBtn;
        sPtr<Button> prevBtn;
        std::vector<sPtr<Button>> btnList;

        s8 currentPage;

    public:
        Scene_Tutorial();

        virtual void Init() override;
        virtual void Update() override;
        virtual void Render() override;
        virtual void Exit() override;

        void TutorialButtonPress(BUTTON_PRESS press);
    };
}

