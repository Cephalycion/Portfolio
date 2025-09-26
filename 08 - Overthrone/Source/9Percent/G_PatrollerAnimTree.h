/******************************************************************************/
/*!
\file   G_PatrollerAnimTree.h
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
  Patroller Animation Tree.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#pragma once
#include "pch.h"
#include "E_BaseState.h"
#include "G_EnemyPatroller.h"

namespace Game {

	class PatrollerIdleLeft : public BaseState
	{
	private:
		EnemyPatroller* objRef;
		Vec2 prevPos;
		Vec2 currPos;
	public:
		PatrollerIdleLeft(EnemyPatroller* obj, BaseStateMachine* fsm);
		~PatrollerIdleLeft();

		virtual void Enter() override;
		virtual void Update() override;
		virtual void Exit() override;
	};
	class PatrollerIdleRight : public BaseState
	{
	private:
		EnemyPatroller* objRef;
		Vec2 prevPos;
		Vec2 currPos;
	public:
		PatrollerIdleRight(EnemyPatroller* obj, BaseStateMachine* fsm);
		~PatrollerIdleRight();

		virtual void Enter() override;
		virtual void Update() override;
		virtual void Exit() override;
	};

	class PatrollerMoveLeft : public BaseState
	{
	private:
		EnemyPatroller* objRef;
		Vec2 prevPos;
		Vec2 currPos;
	public:
		PatrollerMoveLeft(EnemyPatroller* obj, BaseStateMachine* fsm);
		~PatrollerMoveLeft();

		virtual void Enter() override;
		virtual void Update() override;
		virtual void Exit() override;
	};
	class PatrollerMoveRight : public BaseState
	{
	private:
		EnemyPatroller* objRef;
		Vec2 prevPos;
		Vec2 currPos;
	public:
		PatrollerMoveRight(EnemyPatroller* obj, BaseStateMachine* fsm);
		~PatrollerMoveRight();

		virtual void Enter() override;
		virtual void Update() override;
		virtual void Exit() override;
	};

	class PatrollerAirLeft : public BaseState
	{
	private:
		EnemyPatroller* objRef;
		Vec2 prevPos;
		Vec2 currPos;
	public:
		PatrollerAirLeft(EnemyPatroller* obj, BaseStateMachine* fsm);
		~PatrollerAirLeft();

		virtual void Enter() override;
		virtual void Update() override;
		virtual void Exit() override;
	};
	class PatrollerAirRight : public BaseState
	{
	private:
		EnemyPatroller* objRef;
		Vec2 prevPos;
		Vec2 currPos;
	public:
		PatrollerAirRight(EnemyPatroller* obj, BaseStateMachine* fsm);
		~PatrollerAirRight();

		virtual void Enter() override;
		virtual void Update() override;
		virtual void Exit() override;
	};

	class PatrollerExplodeLeft : public BaseState
	{
	private:
		EnemyPatroller* objRef;
	public:
		PatrollerExplodeLeft(EnemyPatroller* obj, BaseStateMachine* fsm);
		~PatrollerExplodeLeft();

		virtual void Enter() override;
		virtual void Update() override;
		virtual void Exit() override;
	};
	class PatrollerExplodeRight : public BaseState
	{
	private:
		EnemyPatroller* objRef;
	public:
		PatrollerExplodeRight(EnemyPatroller* obj, BaseStateMachine* fsm);
		~PatrollerExplodeRight();

		virtual void Enter() override;
		virtual void Update() override;
		virtual void Exit() override;
	};

}
