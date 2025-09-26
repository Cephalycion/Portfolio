/******************************************************************************/
/*!
\file   E_TurretStateMachine.h
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Marc Alviz Evangelista (100%)
\par    email: marcalviz.e\@digipen.edu
\par    DigiPen login: marcalviz.e

\brief
  This is the interface file for all member functions of the states of the
  Turret behavior State Machine.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#pragma once 
#include "pch.h"
#include "E_BaseStateMachine.h"


// Each Enemy State Machine contains all the states as well
namespace Game {
	class EnemyTurret;
}
namespace Engine {


	class TurretIdleState : public BaseState {
	private:
		Game::EnemyTurret* objRef;
	public:
		TurretIdleState(Game::EnemyTurret* obj, BaseStateMachine* fsm);
		~TurretIdleState();


		virtual void Enter() override;
		virtual void Update() override;
		virtual void Exit() override;
	};

	class TurretAggroState : public BaseState {
	private:
		Game::EnemyTurret* objRef;
		Color tint;
	public:
		TurretAggroState(Game::EnemyTurret* obj, BaseStateMachine* fsm);
		~TurretAggroState();

		virtual void Enter() override;
		virtual void Update() override;
		virtual void Exit() override;
	};

}

