/******************************************************************************/
/*!
\file   E_PatrollerStateMachine.cpp
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
  Patroller behavior State Machine.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#include "pch.h"
#include "E_PatrollerStateMachine.h"
#include "G_EnemyPatroller.h"
#include "G_Map.h"
#include "E_Singleton.h"
#include "G_Player.h"
#include "G_Constants.h"

namespace Engine {


	// ******************************************** IDLE STATE ******************************************** //
	PatrollerIdleState::PatrollerIdleState(Game::EnemyPatroller* obj, BaseStateMachine* fsm)
		: BaseState(fsm)
		, objRef(obj)
		, switchTimer(5.0f)
	{
		actTimer = AERandFloat();
		if (actTimer > 0.6f)
		{
			actTimer -= 0.25f;
		}
	}

	PatrollerIdleState::~PatrollerIdleState()
	{
	}

	void PatrollerIdleState::Enter()
	{

		// Randomise the timer
		switchTimer = AERandFloat() * 3.0f + 5.0f;
		objRef->SetActionTimer(actTimer);
		objRef->SetIsMoving(false);
	}

	void PatrollerIdleState::Update()
	{
		// Timer to change to Patrol state
		switchTimer -= C_frametime;
		if (switchTimer < 0.0f)
		{
			fsm->ChangeState(Game::PATROLLERSTATES::P_PATROL);
		}

		if (objRef->UpdateActionTimer(actTimer))
		{
			objRef->DoAction(Game::PATROLLERSTATES::P_IDLE);
		}

		// If player found
		if (objRef->GetPlayerFoundFlag())
		{
			fsm->ChangeState(Game::PATROLLERSTATES::P_AGGRO);
		}
	}

	void PatrollerIdleState::Exit()
	{
	}

	// ******************************************** PATROL STATE ******************************************** //
	PatrollerPatrolState::PatrollerPatrolState(Game::EnemyPatroller* obj, BaseStateMachine* fsm)
		: BaseState(fsm)
		, objRef(obj)
		, xVelocity(100.0f)
		, switchTimer(5.0f)
	{
		actTimer = AERandFloat();
		if (actTimer > 0.6f)
		{
			actTimer -= 0.25f;
		}
	}

	PatrollerPatrolState::~PatrollerPatrolState()
	{
	}

	void PatrollerPatrolState::Enter()
	{
		// Randomise the timer
		switchTimer = AERandFloat() * 6.0f + 10.0f;
		objRef->SetActionTimer(actTimer);
		objRef->SetIsMoving(true);
	}


	void PatrollerPatrolState::Update()
	{
		// Timer to change to Idle state
		switchTimer -= C_frametime;
		if (switchTimer < 0.0f)
		{
			fsm->ChangeState(Game::PATROLLERSTATES::P_IDLE);
		}

		if (objRef->UpdateActionTimer(actTimer))
		{
			objRef->DoAction(Game::PATROLLERSTATES::P_PATROL);
		}

		if (objRef->GetDirection() == Game::ENEMYDIRECTION::EDIR_LEFT)
		{
			objRef->SetVelocity(MakeVec2(-xVelocity, objRef->GetVelocity().y));
		}
		else
		{
			objRef->SetVelocity(MakeVec2(xVelocity, objRef->GetVelocity().y));
		}

		// Look ahead and check if need change directions
		if (objRef->GetTurningPoint())
		{
			if (objRef->GetDirection() == Game::ENEMYDIRECTION::EDIR_LEFT)
			{
				objRef->SetDirection(Game::ENEMYDIRECTION::EDIR_RIGHT);
			}
			else
			{
				objRef->SetDirection(Game::ENEMYDIRECTION::EDIR_LEFT);
			}
			objRef->SetTurningPoint(false);
		}

		Game::Coordinate objCoord = Engine::Singleton<Game::Map>::Instance()->WorldToMapPos(MakeVec2(
			objRef->GetPosition().x - Game::C_blockLengthHalf,
			objRef->GetPosition().y + Game::C_blockLengthHalf));
		objCoord.y += 1;
		if (objRef->GetDirection() == Game::ENEMYDIRECTION::EDIR_LEFT)
		{
			objCoord.x -= 1;
		}
		else
		{
			objCoord.x += 1;
		}

		Game::Block* block = Engine::Singleton<Game::Map>::Instance()->GetBlockAtIndex(objCoord);
		if (block)
		{
			Game::BLOCK_TYPE bt = block->type;
			if (bt == Game::BLOCK_TYPE::BT_NONE
				|| bt == Game::BLOCK_TYPE::BT_WATER
				|| bt == Game::BLOCK_TYPE::BT_LAVA)
			{
				objRef->SetTurningPoint(true);
			}
		}


		// Found a player
		if (objRef->GetPlayerFoundFlag())
		{
			fsm->ChangeState(Game::PATROLLERSTATES::P_AGGRO);
		}

		
	}
	void PatrollerPatrolState::Exit()
	{
		objRef->SetVelocity(MakeVec2(-objRef->GetVelocity().x, objRef->GetVelocity().y));
	}

	// ******************************************** AGGRO STATE ******************************************** //
	PatrollerAggroState::PatrollerAggroState(Game::EnemyPatroller* obj, BaseStateMachine* fsm)
		: BaseState(fsm)
		, objRef(obj)
		, xMultiplier(1.5f)
		, xVelocity(100.0f)
		, lostPlayer(5.0f)
	{
		actTimer = AERandFloat();
		if (actTimer > 0.5f)
		{
			actTimer -= 0.35f;
		}
		xVelocity *= xMultiplier;
	}

	PatrollerAggroState::~PatrollerAggroState()
	{
	}

	void PatrollerAggroState::Enter()
	{
		// make the enemy faster ccause running
		objRef->SetActionTimer(actTimer);
		lostPlayer = 5.0f;
		objRef->SetIsMoving(true);
	}

	void PatrollerAggroState::Update()
	{
		// Move in direction of player-> checked inside DoAction
		if (objRef->GetDirection() == Game::ENEMYDIRECTION::EDIR_LEFT)
		{
			objRef->SetVelocity(MakeVec2(-xVelocity, objRef->GetVelocity().y));
		}
		else
		{
			objRef->SetVelocity(MakeVec2(xVelocity, objRef->GetVelocity().y));
		}
		// Check for ledges == Jump to try to reach the player
		Game::Coordinate objCoord = Engine::Singleton<Game::Map>::Instance()->WorldToMapPos(MakeVec2(
			objRef->GetPosition().x - Game::C_blockLengthHalf,
			objRef->GetPosition().y + Game::C_blockLengthHalf));
		Game::Coordinate wallCheck = objCoord;
		objCoord.y += 1;
		bool onGround = false;
		Game::Block* block = Engine::Singleton<Game::Map>::Instance()->GetBlockAtIndex(objCoord);
		if (block)
		{
			Game::BLOCK_TYPE bt = block->type;
			if (bt != Game::BLOCK_TYPE::BT_NONE
				&& bt != Game::BLOCK_TYPE::BT_WATER
				&& bt != Game::BLOCK_TYPE::BT_LAVA)
			{
				onGround = true;
			}
		}

		if (objRef->GetDirection() == Game::ENEMYDIRECTION::EDIR_LEFT)
		{
			objCoord.x -= 1;
			wallCheck.x -= 1;
		}
		else
		{
			objCoord.x += 1;
			wallCheck.x += 1;
		}

		block = Engine::Singleton<Game::Map>::Instance()->GetBlockAtIndex(objCoord);
		if (block)
		{
			Game::BLOCK_TYPE bt = block->type;
			if (bt == Game::BLOCK_TYPE::BT_NONE
				|| bt == Game::BLOCK_TYPE::BT_WATER
				|| bt == Game::BLOCK_TYPE::BT_LAVA)
			{
				// At ledge
				if (onGround)
				{
					objRef->SetVelocity(MakeVec2(objRef->GetVelocity().x * xMultiplier, 500.0f));
					onGround = false;
				}
			}
		}

		block = Engine::Singleton<Game::Map>::Instance()->GetBlockAtIndex(wallCheck);
		if (block)
		{
			Game::BLOCK_TYPE bt = block->type;
			if (bt != Game::BLOCK_TYPE::BT_NONE
				&& bt != Game::BLOCK_TYPE::BT_WATER
				&& bt != Game::BLOCK_TYPE::BT_LAVA)
			{
				if (onGround)
				{
					objRef->SetVelocity(MakeVec2(objRef->GetVelocity().x * xMultiplier, 500.0f));
					onGround = false;
				}
			}
		}



		// check if the player is near enough
		if (objRef->UpdateActionTimer(actTimer))
		{
			objRef->DoAction(Game::PATROLLERSTATES::P_AGGRO);
		}

		Vec2 EnemytoPlayer = Singleton<Game::Player>::Instance()->GetPosition() - objRef->GetPosition();
		f32 playerDistance = Vec2Length(EnemytoPlayer);
		if (playerDistance < objRef->GetExRadius())
		{
			fsm->ChangeState(Game::PATROLLERSTATES::P_EXPLODING);
		}

		if (!objRef->GetPlayerFoundFlag())
		{
			lostPlayer -= f32(Game::C_frametime);
			if (lostPlayer <= 0.0f)
			{
				fsm->ChangeState(Game::PATROLLERSTATES::P_IDLE);
			}
		}
		else
		{
			lostPlayer = 5.0f;
		}
	}

	void PatrollerAggroState::Exit()
	{
		objRef->SetVelocity(Vec2Zero());
	}

	// ******************************************** EXPLODE STATE ******************************************** //
	PatrollerExplodeState::PatrollerExplodeState(Game::EnemyPatroller* obj, BaseStateMachine* fsm)
		: BaseState(fsm)
		, objRef(obj)
	{
		// This actTimer is a countdown to create an explosion
		actTimer = Game::C_patrollerFuseTimer;
	}

	PatrollerExplodeState::~PatrollerExplodeState()
	{
	}

	void PatrollerExplodeState::Enter()
	{
		objRef->SetActionTimer(actTimer);
		objRef->SetVelocity(Vec2Zero());
		objRef->SetIsExploding(true);
	}


	void PatrollerExplodeState::Update()
	{

		if (objRef->UpdateActionTimer(actTimer))
		{
			objRef->DoAction(Game::PATROLLERSTATES::P_EXPLODING);
		}

	}
	void PatrollerExplodeState::Exit()
	{
	}
}
