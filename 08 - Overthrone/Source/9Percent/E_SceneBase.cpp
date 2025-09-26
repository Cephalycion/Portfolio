/******************************************************************************/
/*!
\file   E_SceneBase.cpp
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Kendrick Sim Hean Guan (100%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\brief
  This is the implementation file for the base class of a scene.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#include "pch.h"
#include "E_SceneBase.h"

namespace Engine { namespace Scene {

	Scene_Base::Scene_Base()
		: isUpdateBlocking(false)
	{
	}

	bool Scene_Base::GetUpdateBlocking() const
	{
		return isUpdateBlocking;
	}
	void Scene_Base::SetUpdateBlocking(bool newIsUpdateBlocking)
	{
		isUpdateBlocking = newIsUpdateBlocking;
	}

}}

