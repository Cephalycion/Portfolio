/******************************************************************************/
/*!
\file   E_TurretStateMachine.cpp
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Marc Alviz Evangelista (100%)
\par    email: marcalviz.e\@digipen.edu
\par    DigiPen login: marcalviz.e

\brief
  This is the implementation file for all member functions of the states of the
  Turret behavior State Machine.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#include "pch.h"
#include "E_TurretStateMachine.h"
#include "G_EnemyTurret.h"

namespace Engine {

	using namespace Game;


	TurretIdleState::TurretIdleState(Game::EnemyTurret* obj, BaseStateMachine* fsm)
		: BaseState(fsm)
		, objRef(obj)
	{
		actTimer = 0.5f;
	}

	TurretIdleState::~TurretIdleState()
	{
		fsm = nullptr;
		objRef = nullptr;
		
	}

	void TurretIdleState::Enter()
	{
		objRef->SetActionTimer(actTimer);
	}

	void TurretIdleState::Update()
	{
		if (objRef->UpdateActionTimer(actTimer))
		{
			objRef->DoAction(TURRETSTATES::T_IDLE);
		}

		if (objRef->GetPlayerFoundFlag())
		{
			fsm->ChangeState(TURRETSTATES::T_AGGRO);
		}
	}

	void TurretIdleState::Exit()
	{

	}


	TurretAggroState::TurretAggroState(Game::EnemyTurret* obj, BaseStateMachine* fsm)
		: BaseState(fsm)
		, objRef(obj)
	{
		actTimer = 2.0f;
		tint = C_colorWhite;
	}

	TurretAggroState::~TurretAggroState()
	{
		fsm = nullptr;
		objRef = nullptr;
	}

	void TurretAggroState::Enter()
	{
		objRef->SetActionTimer(actTimer);
		tint = C_colorWhite;
		objRef->SetColorTint(tint);

	}

	void TurretAggroState::Update()
	{
		tint.g -= C_EnemyGreenDrainRate * C_frametime;
		tint.b -= C_EnemyGreenDrainRate * 2 * C_frametime;
		objRef->SetColorTint(tint);
		if (objRef->UpdateActionTimer(actTimer))
		{
			objRef->DoAction(TURRETSTATES::T_AGGRO);
			tint = C_colorWhite;
			objRef->SetColorTint(tint);
		}

		if (!objRef->GetPlayerFoundFlag())
		{
			fsm->ChangeState(TURRETSTATES::T_IDLE);
		}
	}

	void TurretAggroState::Exit()
	{
		tint = C_colorWhite;
		objRef->SetColorTint(tint);

	}
}