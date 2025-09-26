/******************************************************************************/
/*!
\file   G_PatrollerAnimTree.cpp
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
  Patroller Animation Tree.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#include "pch.h"
#include "G_PatrollerAnimTree.h"

namespace Game {

//********************************************** IDLE **********************************************//
	PatrollerIdleLeft::PatrollerIdleLeft(EnemyPatroller* obj, BaseStateMachine* fsm)
		: BaseState(fsm)
		, objRef(obj)
		, prevPos(Vec2Zero())
		, currPos(Vec2Zero())
	{
	}
	PatrollerIdleLeft::~PatrollerIdleLeft()
	{
	}
	void PatrollerIdleLeft::Enter()
	{
		objRef->ChangeAnimation(PATROLLER_IDLE_LEFT);
		prevPos = objRef->GetPosition();
		currPos = objRef->GetPosition();
	}
	void PatrollerIdleLeft::Update()
	{
		prevPos = currPos;
		currPos = objRef->GetPosition();
		Vec2 relPos;
		AEVec2Sub(&relPos, &prevPos, &currPos);
		Vec2Abs(&relPos);
		if (relPos.y > C_epsilon)
		{
			fsm->ChangeState(PATROLLER_AIR_LEFT);
		}
		else if (objRef->GetIsExploding())
		{
			fsm->ChangeState(PATROLLER_EXPLODE_LEFT);
		}
		else if (objRef->GetDirection() == ENEMYDIRECTION::EDIR_RIGHT)
		{
			fsm->ChangeState(PATROLLER_IDLE_RIGHT);
		}
		else if (objRef->GetIsMoving())
		{
			fsm->ChangeState(PATROLLER_MOVE_LEFT);
		}

	}
	void PatrollerIdleLeft::Exit()
	{
	}

	PatrollerIdleRight::PatrollerIdleRight(EnemyPatroller* obj, BaseStateMachine* fsm)
		: BaseState(fsm)
		, objRef(obj)
		, prevPos(Vec2Zero())
		, currPos(Vec2Zero())
	{
	}
	PatrollerIdleRight::~PatrollerIdleRight()
	{
	}
	void PatrollerIdleRight::Enter()
	{
		objRef->ChangeAnimation(PATROLLER_IDLE_RIGHT);
		prevPos = objRef->GetPosition();
		currPos = objRef->GetPosition();
	}
	void PatrollerIdleRight::Update()
	{
		prevPos = currPos;
		currPos = objRef->GetPosition();
		Vec2 relPos;
		AEVec2Sub(&relPos, &prevPos, &currPos);
		Vec2Abs(&relPos);
		if (relPos.y > C_epsilon)
		{
			fsm->ChangeState(PATROLLER_AIR_RIGHT);
		}
		else if (objRef->GetIsExploding())
		{
			fsm->ChangeState(PATROLLER_EXPLODE_RIGHT);
		}
		else if (objRef->GetDirection() == ENEMYDIRECTION::EDIR_LEFT)
		{
			fsm->ChangeState(PATROLLER_IDLE_LEFT);
		}
		else if (objRef->GetIsMoving())
		{
			fsm->ChangeState(PATROLLER_MOVE_RIGHT);
		}

	}
	void PatrollerIdleRight::Exit()
	{
	}

//********************************************** MOVE **********************************************//

	PatrollerMoveLeft::PatrollerMoveLeft(EnemyPatroller* obj, BaseStateMachine* fsm)
		: BaseState(fsm)
		, objRef(obj)
		, prevPos(Vec2Zero())
		, currPos(Vec2Zero())
	{
	}
	PatrollerMoveLeft::~PatrollerMoveLeft()
	{
	}
	void PatrollerMoveLeft::Enter()
	{
		objRef->ChangeAnimation(PATROLLER_MOVE_LEFT);
		prevPos = objRef->GetPosition();
		currPos = objRef->GetPosition();
	}
	void PatrollerMoveLeft::Update()
	{
		prevPos = currPos;
		currPos = objRef->GetPosition();
		Vec2 relPos;
		AEVec2Sub(&relPos, &prevPos, &currPos);
		Vec2Abs(&relPos);
		if (relPos.y > C_epsilon)
		{
			fsm->ChangeState(PATROLLER_AIR_LEFT);
		}
		else if (objRef->GetIsExploding())
		{
			fsm->ChangeState(PATROLLER_EXPLODE_LEFT);
		}
		else if (objRef->GetDirection() == ENEMYDIRECTION::EDIR_RIGHT)
		{
			fsm->ChangeState(PATROLLER_MOVE_RIGHT);
		}
		else if (!objRef->GetIsMoving())
		{
			fsm->ChangeState(PATROLLER_IDLE_LEFT);
		}
	}
	void PatrollerMoveLeft::Exit()
	{
	}

	PatrollerMoveRight::PatrollerMoveRight(EnemyPatroller* obj, BaseStateMachine* fsm)
		: BaseState(fsm)
		, objRef(obj)
		, prevPos(Vec2Zero())
		, currPos(Vec2Zero())
	{
	}
	PatrollerMoveRight::~PatrollerMoveRight()
	{
	}
	void PatrollerMoveRight::Enter()
	{
		objRef->ChangeAnimation(PATROLLER_MOVE_RIGHT);
		prevPos = objRef->GetPosition();
		currPos = objRef->GetPosition();
	}
	void PatrollerMoveRight::Update()
	{
		prevPos = currPos;
		currPos = objRef->GetPosition();
		Vec2 relPos;
		AEVec2Sub(&relPos, &prevPos, &currPos);
		Vec2Abs(&relPos);
		if (relPos.y > C_epsilon)
		{
			fsm->ChangeState(PATROLLER_AIR_LEFT);
		}
		else if (objRef->GetIsExploding())
		{
			fsm->ChangeState(PATROLLER_EXPLODE_RIGHT);
		}
		else if (objRef->GetDirection() == ENEMYDIRECTION::EDIR_LEFT)
		{
			fsm->ChangeState(PATROLLER_MOVE_LEFT);
		}
		else if (!objRef->GetIsMoving())
		{
			fsm->ChangeState(PATROLLER_IDLE_RIGHT);
		}
	}
	void PatrollerMoveRight::Exit()
	{
	}

//********************************************** AIR **********************************************//


	PatrollerAirLeft::PatrollerAirLeft(EnemyPatroller* obj, BaseStateMachine* fsm)
		: BaseState(fsm)
		, objRef(obj)
		, prevPos(Vec2Zero())
		, currPos(Vec2Zero())
	{
	}
	PatrollerAirLeft::~PatrollerAirLeft()
	{
	}
	void PatrollerAirLeft::Enter()
	{
		objRef->ChangeAnimation(PATROLLER_AIR_LEFT);
		prevPos = objRef->GetPosition();
		currPos = objRef->GetPosition();
	}
	void PatrollerAirLeft::Update()
	{
		prevPos = currPos;
		currPos = objRef->GetPosition();
		Vec2 relPos;
		AEVec2Sub(&relPos, &prevPos, &currPos);
		Vec2Abs(&relPos);
		if (relPos.y < C_epsilon)
		{
			fsm->ChangeState(PATROLLER_MOVE_LEFT);
		}
		else if (objRef->GetIsExploding())
		{
			fsm->ChangeState(PATROLLER_EXPLODE_LEFT);
		}
		else if (objRef->GetDirection() == ENEMYDIRECTION::EDIR_RIGHT)
		{
			fsm->ChangeState(PATROLLER_AIR_RIGHT);
		}

	}
	void PatrollerAirLeft::Exit()
	{
	}

	PatrollerAirRight::PatrollerAirRight(EnemyPatroller* obj, BaseStateMachine* fsm)
		: BaseState(fsm)
		, objRef(obj)
		, prevPos(Vec2Zero())
		, currPos(Vec2Zero())
	{
	}
	PatrollerAirRight::~PatrollerAirRight()
	{
	}
	void PatrollerAirRight::Enter()
	{
		objRef->ChangeAnimation(PATROLLER_AIR_RIGHT);
		prevPos = objRef->GetPosition();
		currPos = objRef->GetPosition();
	}
	void PatrollerAirRight::Update()
	{
		prevPos = currPos;
		currPos = objRef->GetPosition();
		Vec2 relPos;
		AEVec2Sub(&relPos, &prevPos, &currPos);
		Vec2Abs(&relPos);
		if (relPos.y < C_epsilon)
		{
			fsm->ChangeState(PATROLLER_MOVE_RIGHT);
		}
		else if (objRef->GetIsExploding())
		{
			fsm->ChangeState(PATROLLER_EXPLODE_RIGHT);
		}
		else if (objRef->GetDirection() == ENEMYDIRECTION::EDIR_LEFT)
		{
			fsm->ChangeState(PATROLLER_AIR_LEFT);
		}

	}
	void PatrollerAirRight::Exit()
	{
	}

//********************************************** EXPLODE **********************************************//

	// Theres no getting out of explode :(

	PatrollerExplodeLeft::PatrollerExplodeLeft(EnemyPatroller* obj, BaseStateMachine* fsm)
		: BaseState(fsm)
		, objRef(obj)
	{
	}
	PatrollerExplodeLeft::~PatrollerExplodeLeft()
	{
	}
	void PatrollerExplodeLeft::Enter()
	{
		objRef->ChangeAnimation(PATROLLER_EXPLODE_LEFT);
	}
	void PatrollerExplodeLeft::Update()
	{
	}
	void PatrollerExplodeLeft::Exit()
	{
	}

	PatrollerExplodeRight::PatrollerExplodeRight(EnemyPatroller* obj, BaseStateMachine* fsm)
		: BaseState(fsm)
		, objRef(obj)
	{
	}
	PatrollerExplodeRight::~PatrollerExplodeRight()
	{
	}
	void PatrollerExplodeRight::Enter()
	{
		objRef->ChangeAnimation(PATROLLER_EXPLODE_RIGHT);
	}
	void PatrollerExplodeRight::Update()
	{
	}
	void PatrollerExplodeRight::Exit()
	{
	}
}