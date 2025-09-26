/******************************************************************************/
/*!
\file   E_SceneBase.h
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Kendrick Sim Hean Guan (100%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\brief
  This is an interface file for the base class of a scene.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#pragma once
#include "pch.h"

namespace Engine { namespace Scene {

	class Scene_Base
	{
	protected:
		bool isUpdateBlocking; // Whether this scene blocks the updating of scenes below it

	public:
		Scene_Base();

		bool GetUpdateBlocking() const;
		void SetUpdateBlocking(bool newIsUpdateBlocking);

		virtual void Init() {};
		virtual void Update() {};
		virtual void Render() {};
		virtual void Exit() {};
	};
}}
