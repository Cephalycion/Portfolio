/******************************************************************************/
/*!
\file   E_PatrollerStateMachine.h
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
  Patroller behavior State Machine.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#pragma once
#include "pch.h"
#include "E_BaseStateMachine.h"
#include "E_BaseState.h"

namespace Game {
	class EnemyPatroller;
}

namespace Engine {

	class PatrollerIdleState : public BaseState {
	private:
		Game::EnemyPatroller* objRef;
		f32 switchTimer; // Randomised value to change from idle to patrol state and vice versa
	public:
		PatrollerIdleState(Game::EnemyPatroller* obj, BaseStateMachine* fsm);
		~PatrollerIdleState();

		virtual void Enter() override;
		virtual void Update() override;
		virtual void Exit() override;
	};


	// Moves in a direction and changes direction when reaches edge or wall
	class PatrollerPatrolState : public BaseState {
	private:
		Game::EnemyPatroller* objRef;
		f32 switchTimer; // Randomised value to change from idle to patrol state and vice versa
		f32 xVelocity;
	public:
		PatrollerPatrolState(Game::EnemyPatroller* obj, BaseStateMachine* fsm);
		~PatrollerPatrolState();

		virtual void Enter() override;
		virtual void Update() override;
		virtual void Exit() override;

	};

	class PatrollerAggroState : public BaseState {
	private:
		Game::EnemyPatroller* objRef;
		f32 xVelocity;
		f32 xMultiplier;
		f32 lostPlayer;
	public:
		PatrollerAggroState(Game::EnemyPatroller* obj, BaseStateMachine* fsm);
		~PatrollerAggroState();

		virtual void Enter() override;
		virtual void Update() override;
		virtual void Exit() override;
	};

	class PatrollerExplodeState : public BaseState {
	private:
		Game::EnemyPatroller* objRef;
	public:
		PatrollerExplodeState(Game::EnemyPatroller* obj, BaseStateMachine* fsm);
		~PatrollerExplodeState();

		virtual void Enter() override;
		virtual void Update() override;
		virtual void Exit() override;
	};
}

