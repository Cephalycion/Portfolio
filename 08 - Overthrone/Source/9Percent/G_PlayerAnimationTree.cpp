/******************************************************************************/
/*!
\file   G_PlayerAnimationTree.cpp
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Marc Alviz Evangelista (100%)
\par    email: marcalviz.e\@digipen.edu
\par    DigiPen login: marcalviz.e

\brief
  This is the implementation file for all member functions of the classes
  of animation states for the Player Animation Tree.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#include "pch.h"
#include "G_PlayerAnimationTree.h"
#include "E_Constants.h"

Game::PlayerIdleLeft::PlayerIdleLeft(Player* playerRef, BaseStateMachine* fsm)
	: BaseState(fsm)
	, playerRef(playerRef)
	, currVel(Vec2Zero())
	, prevVel(Vec2Zero())
	, currPos(Vec2Zero())
	, prevPos(Vec2Zero())
{
}

Game::PlayerIdleLeft::~PlayerIdleLeft()
{
}

void Game::PlayerIdleLeft::Enter()
{
	playerRef->ChangeAnimation(Player::PLAYER_SPRITE::PL_IDLE_LEFT);
}

void Game::PlayerIdleLeft::Update()
{
	prevPos = currPos;
	currPos = playerRef->GetPosition();
	prevVel = currVel;
	currVel = playerRef->GetVelocity();
	Vec2 relPos = prevPos - currPos;
	Vec2Abs(&relPos);
	Vec2 relVel = prevVel - currVel;
	Vec2Abs(&relVel);

	if (playerRef->grabbedBlock.object != nullptr)
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_GRAB_IDLE_LEFT);
	}

	if (currVel.y > 0.0f)
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_JUMP_LEFT);
	}
	else if (relPos.y > C_epsilon)
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_FALL_LEFT);
	}
	else if (AEInputCheckTriggered(AEVK_D))
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_MOVE_RIGHT);
	}
	else if (AEInputCheckTriggered(AEVK_A))
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_MOVE_LEFT);
	}

}

void Game::PlayerIdleLeft::Exit()
{
}

Game::PlayerIdleRight::PlayerIdleRight(Player* playerRef, BaseStateMachine* fsm)
	: BaseState(fsm)
	, playerRef(playerRef)
	, currVel(Vec2Zero())
	, prevVel(Vec2Zero())
	, currPos(Vec2Zero())
	, prevPos(Vec2Zero())
{
}

Game::PlayerIdleRight::~PlayerIdleRight()
{
}

void Game::PlayerIdleRight::Enter()
{
	playerRef->ChangeAnimation(Player::PLAYER_SPRITE::PL_IDLE_RIGHT);
}

void Game::PlayerIdleRight::Update()
{
	prevPos = currPos;
	currPos = playerRef->GetPosition();
	prevVel = currVel;
	currVel = playerRef->GetVelocity();
	Vec2 relPos = prevPos - currPos;
	Vec2Abs(&relPos);
	Vec2 relVel = prevVel - currVel;
	Vec2Abs(&relVel);

	if (playerRef->grabbedBlock.object != nullptr)
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_GRAB_IDLE_RIGHT);
	}

	if (currVel.y > 0.0f)
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_JUMP_RIGHT);
	}
	else if (relPos.y > C_epsilon)
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_FALL_RIGHT);
	}
	else if (AEInputCheckTriggered(AEVK_D))
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_MOVE_RIGHT);
	}
	else if (AEInputCheckTriggered(AEVK_A))
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_MOVE_LEFT);
	}
}

void Game::PlayerIdleRight::Exit()
{
}

Game::PlayerMoveLeft::PlayerMoveLeft(Player* playerRef, BaseStateMachine* fsm)
	: BaseState(fsm)
	, playerRef(playerRef)
	, currVel(Vec2Zero())
	, prevVel(Vec2Zero())
	, currPos(Vec2Zero())
	, prevPos(Vec2Zero())
{
}

Game::PlayerMoveLeft::~PlayerMoveLeft()
{
}

void Game::PlayerMoveLeft::Enter()
{
	playerRef->ChangeAnimation(Player::PLAYER_SPRITE::PL_MOVE_LEFT);
}

void Game::PlayerMoveLeft::Update()
{
	prevPos = currPos;
	currPos = playerRef->GetPosition();
	prevVel = currVel;
	currVel = playerRef->GetVelocity();
	Vec2 relPos = prevPos - currPos;
	Vec2Abs(&relPos);
	Vec2 relVel = prevVel - currVel;
	Vec2Abs(&relVel);

	if (playerRef->grabbedBlock.object != nullptr)
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_GRAB_MOVE_LEFT);
	}

	if (AEInputCheckTriggered(AEVK_SPACE))
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_JUMP_LEFT);
	}
	else if (AEInputCheckCurr(AEVK_A))
	{

	}
	else if (AEInputCheckCurr(AEVK_D))
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_MOVE_RIGHT);
	}
	else if (relVel.y > 100.f && relPos.y > 1.f)
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_FALL_LEFT);
	}
	else if (relVel.x < C_epsilon + 0.1f)
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_IDLE_LEFT);
	}
}

void Game::PlayerMoveLeft::Exit()
{
}

Game::PlayerMoveRight::PlayerMoveRight(Player* playerRef, BaseStateMachine* fsm)
	: BaseState(fsm)
	, playerRef(playerRef)
	, currVel(Vec2Zero())
	, prevVel(Vec2Zero())
	, currPos(Vec2Zero())
	, prevPos(Vec2Zero())
{
}

Game::PlayerMoveRight::~PlayerMoveRight()
{
}

void Game::PlayerMoveRight::Enter()
{
	playerRef->ChangeAnimation(Player::PLAYER_SPRITE::PL_MOVE_RIGHT);
}

void Game::PlayerMoveRight::Update()
{
	prevPos = currPos;
	currPos = playerRef->GetPosition();
	prevVel = currVel;
	currVel = playerRef->GetVelocity();
	Vec2 relPos = prevPos - currPos;
	Vec2Abs(&relPos);
	Vec2 relVel = prevVel - currVel;
	Vec2Abs(&relVel);

	if (playerRef->grabbedBlock.object != nullptr)
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_GRAB_MOVE_RIGHT);
	}

	if (AEInputCheckTriggered(AEVK_SPACE))
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_JUMP_RIGHT);
	}
	else if (AEInputCheckCurr(AEVK_D))
	{

	}
	else if (AEInputCheckCurr(AEVK_A))
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_MOVE_LEFT);
	}
	else if (relVel.y > 100.f && relPos.y > 1.f)
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_FALL_RIGHT);
	}
	else if (relPos.x < C_epsilon + 0.1f)
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_IDLE_RIGHT);
	}
}

void Game::PlayerMoveRight::Exit()
{
}

Game::PlayerJumpLeft::PlayerJumpLeft(Player* playerRef, BaseStateMachine* fsm)
	: BaseState(fsm)
	, playerRef(playerRef)
	, currVel(Vec2Zero())
	, prevVel(Vec2Zero())
	, currPos(Vec2Zero())
	, prevPos(Vec2Zero())
{
}

Game::PlayerJumpLeft::~PlayerJumpLeft()
{
}

void Game::PlayerJumpLeft::Enter()
{
	playerRef->ChangeAnimation(Player::PLAYER_SPRITE::PL_JUMP_LEFT);
}

void Game::PlayerJumpLeft::Update()
{
	prevPos = currPos;
	currPos = playerRef->GetPosition();
	prevVel = currVel;
	currVel = playerRef->GetVelocity();
	Vec2 relPos = prevPos - currPos;
	Vec2Abs(&relPos);
	Vec2 relVel = prevVel - currVel;
	Vec2Abs(&relVel);

	if (playerRef->grabbedBlock.object != nullptr)
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_GRAB_JUMP_LEFT);
	}

	if (currVel.y < 0.0f)
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_FALL_LEFT);
	}
	if (currVel.x > 0.0f)
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_JUMP_RIGHT);
	}

}

void Game::PlayerJumpLeft::Exit()
{
}

Game::PlayerJumpRight::PlayerJumpRight(Player* playerRef, BaseStateMachine* fsm)
	: BaseState(fsm)
	, playerRef(playerRef)
	, currVel(Vec2Zero())
	, prevVel(Vec2Zero())
	, currPos(Vec2Zero())
	, prevPos(Vec2Zero())
{
}

Game::PlayerJumpRight::~PlayerJumpRight()
{
}

void Game::PlayerJumpRight::Enter()
{
	playerRef->ChangeAnimation(Player::PLAYER_SPRITE::PL_JUMP_RIGHT);
}

void Game::PlayerJumpRight::Update()
{
	prevPos = currPos;
	currPos = playerRef->GetPosition();
	prevVel = currVel;
	currVel = playerRef->GetVelocity();
	Vec2 relPos = prevPos - currPos;
	Vec2Abs(&relPos);
	Vec2 relVel = prevVel - currVel;
	Vec2Abs(&relVel);

	if (playerRef->grabbedBlock.object != nullptr)
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_GRAB_JUMP_RIGHT);
	}

	if (currVel.y < 0.0f)
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_FALL_RIGHT);
	}
	if (currVel.x < 0.0f)
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_JUMP_LEFT);
	}
}

void Game::PlayerJumpRight::Exit()
{
}

Game::PlayerFallLeft::PlayerFallLeft(Player* playerRef, BaseStateMachine* fsm)
	: BaseState(fsm)
	, playerRef(playerRef)
	, currVel(Vec2Zero())
	, prevVel(Vec2Zero())
	, currPos(Vec2Zero())
	, prevPos(Vec2Zero())
{
}

Game::PlayerFallLeft::~PlayerFallLeft()
{
}

void Game::PlayerFallLeft::Enter()
{
	playerRef->ChangeAnimation(Player::PLAYER_SPRITE::PL_FALL_LEFT);
}

void Game::PlayerFallLeft::Update()
{
	prevPos = currPos;
	currPos = playerRef->GetPosition();
	prevVel = currVel;
	currVel = playerRef->GetVelocity();
	Vec2 relPos = prevPos - currPos;
	Vec2Abs(&relPos);
	Vec2 relVel = prevVel - currVel;
	Vec2Abs(&relVel);

	if (playerRef->grabbedBlock.object != nullptr)
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_GRAB_FALL_LEFT);
	}

	if (currVel.y > 0.0f)
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_JUMP_LEFT);
	}
	else if (relPos.y < C_epsilon)
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_IDLE_LEFT);
	}
	if (currVel.x > 0.0f)
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_FALL_RIGHT);
	}
}

void Game::PlayerFallLeft::Exit()
{
}

Game::PlayerFallRight::PlayerFallRight(Player* playerRef, BaseStateMachine* fsm)
	: BaseState(fsm)
	, playerRef(playerRef)
	, currVel(Vec2Zero())
	, prevVel(Vec2Zero())
	, currPos(Vec2Zero())
	, prevPos(Vec2Zero())
{
}

Game::PlayerFallRight::~PlayerFallRight()
{
}

void Game::PlayerFallRight::Enter()
{
	playerRef->ChangeAnimation(Player::PLAYER_SPRITE::PL_FALL_RIGHT);
}

void Game::PlayerFallRight::Update()
{
	prevPos = currPos;
	currPos = playerRef->GetPosition();
	prevVel = currVel;
	currVel = playerRef->GetVelocity();
	Vec2 relPos = prevPos - currPos;
	Vec2Abs(&relPos);
	Vec2 relVel = prevVel - currVel;
	Vec2Abs(&relVel);

	if (playerRef->grabbedBlock.object != nullptr)
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_GRAB_FALL_RIGHT);
	}

	if (currVel.y > 0.0f)
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_JUMP_RIGHT);
	}
	else if (relPos.y < C_epsilon)
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_IDLE_RIGHT);
	}
	if (currVel.x < 0.0f)
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_FALL_LEFT);
	}
}

void Game::PlayerFallRight::Exit()
{
}


Game::PlayerGrabIdleLeft::PlayerGrabIdleLeft(Player* playerRef, BaseStateMachine* fsm)
	: BaseState(fsm)
	, playerRef(playerRef)
	, currVel(Vec2Zero())
	, prevVel(Vec2Zero())
	, currPos(Vec2Zero())
	, prevPos(Vec2Zero())
{

}

Game::PlayerGrabIdleLeft::~PlayerGrabIdleLeft()
{
}

void Game::PlayerGrabIdleLeft::Enter()
{
	playerRef->ChangeAnimation(Player::PLAYER_SPRITE::PL_GRAB_IDLE_RIGHT);

}

void Game::PlayerGrabIdleLeft::Update()
{
	prevPos = currPos;
	currPos = playerRef->GetPosition();
	prevVel = currVel;
	currVel = playerRef->GetVelocity();
	Vec2 relPos = prevPos - currPos;
	Vec2Abs(&relPos);
	Vec2 relVel = prevVel - currVel;
	Vec2Abs(&relVel);

	if (playerRef->grabbedBlock.object == nullptr)
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_IDLE_LEFT);
	}

	if (currVel.y > 0.0f)
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_GRAB_JUMP_LEFT);
	}
	else if (relPos.y > C_epsilon)
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_GRAB_FALL_LEFT);
	}
	else if (currVel.x > 0.0f)
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_GRAB_MOVE_RIGHT);
	}
	else if (relVel.x > C_epsilon + 0.1f)
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_GRAB_MOVE_LEFT);
	}
}

void Game::PlayerGrabIdleLeft::Exit()
{
}


Game::PlayerGrabIdleRight::PlayerGrabIdleRight(Player* playerRef, BaseStateMachine* fsm)
	: BaseState(fsm)
	, playerRef(playerRef)
	, currVel(Vec2Zero())
	, prevVel(Vec2Zero())
	, currPos(Vec2Zero())
	, prevPos(Vec2Zero())
{
}

Game::PlayerGrabIdleRight::~PlayerGrabIdleRight()
{
}

void Game::PlayerGrabIdleRight::Enter()
{
	playerRef->ChangeAnimation(Player::PLAYER_SPRITE::PL_GRAB_IDLE_RIGHT);

}

void Game::PlayerGrabIdleRight::Update()
{
	prevPos = currPos;
	currPos = playerRef->GetPosition();
	prevVel = currVel;
	currVel = playerRef->GetVelocity();
	Vec2 relPos = prevPos - currPos;
	Vec2Abs(&relPos);
	Vec2 relVel = prevVel - currVel;
	Vec2Abs(&relVel);

	if (playerRef->grabbedBlock.object == nullptr)
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_IDLE_RIGHT);
	}

	if (currVel.y > 0.0f)
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_GRAB_JUMP_RIGHT);
	}
	else if (relPos.y > C_epsilon)
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_GRAB_FALL_RIGHT);
	}
	else if (relVel.x > C_epsilon + 0.1f)
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_GRAB_MOVE_RIGHT);
	}
	else if (currVel.x < 0.0f)
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_GRAB_MOVE_LEFT);
	}
}

void Game::PlayerGrabIdleRight::Exit()
{
}

Game::PlayerGrabMoveLeft::PlayerGrabMoveLeft(Player* playerRef, BaseStateMachine* fsm)
	: BaseState(fsm)
	, playerRef(playerRef)
	, currVel(Vec2Zero())
	, prevVel(Vec2Zero())
	, currPos(Vec2Zero())
	, prevPos(Vec2Zero())
{
}

Game::PlayerGrabMoveLeft::~PlayerGrabMoveLeft()
{
}

void Game::PlayerGrabMoveLeft::Enter()
{
	playerRef->ChangeAnimation(Player::PLAYER_SPRITE::PL_GRAB_MOVE_LEFT);

}

void Game::PlayerGrabMoveLeft::Update()
{
	prevPos = currPos;
	currPos = playerRef->GetPosition();
	prevVel = currVel;
	currVel = playerRef->GetVelocity();
	Vec2 relPos = prevPos - currPos;
	Vec2Abs(&relPos);
	Vec2 relVel = prevVel - currVel;
	Vec2Abs(&relVel);

	if (playerRef->grabbedBlock.object == nullptr)
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_MOVE_LEFT);
	}

	if (relVel.y > 100.f && relPos.y > 1.f)
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_GRAB_JUMP_LEFT);
	}
	else if (AEInputCheckCurr(AEVK_A))
	{

	}
	else if (AEInputCheckCurr(AEVK_D))
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_GRAB_MOVE_RIGHT);
	}
	else if (relVel.y > 100.f && relPos.y > 1.f)
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_GRAB_FALL_LEFT);
	}
	else if (relVel.x < C_epsilon + 0.1f)
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_GRAB_IDLE_LEFT);
	}
}

void Game::PlayerGrabMoveLeft::Exit()
{
}

Game::PlayerGrabMoveRight::PlayerGrabMoveRight(Player* playerRef, BaseStateMachine* fsm)
	: BaseState(fsm)
	, playerRef(playerRef)
	, currVel(Vec2Zero())
	, prevVel(Vec2Zero())
	, currPos(Vec2Zero())
	, prevPos(Vec2Zero())
{
}

Game::PlayerGrabMoveRight::~PlayerGrabMoveRight()
{
}

void Game::PlayerGrabMoveRight::Enter()
{
	playerRef->ChangeAnimation(Player::PLAYER_SPRITE::PL_GRAB_MOVE_RIGHT);

}

void Game::PlayerGrabMoveRight::Update()
{
	prevPos = currPos;
	currPos = playerRef->GetPosition();
	prevVel = currVel;
	currVel = playerRef->GetVelocity();
	Vec2 relPos = prevPos - currPos;
	Vec2Abs(&relPos);
	Vec2 relVel = prevVel - currVel;
	Vec2Abs(&relVel);

	if (playerRef->grabbedBlock.object == nullptr)
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_MOVE_RIGHT);
	}

	if (relVel.y > 100.f && relPos.y > 1.f)
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_GRAB_JUMP_RIGHT);
	}
	else if (AEInputCheckCurr(AEVK_D))
	{

	}
	else if (AEInputCheckCurr(AEVK_A))
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_GRAB_MOVE_LEFT);
	}
	else if (relVel.y > 100.f && relPos.y > 1.f)
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_GRAB_FALL_RIGHT);
	}
	else if (relPos.x < C_epsilon + 0.1f)
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_GRAB_IDLE_RIGHT);
	}
}

void Game::PlayerGrabMoveRight::Exit()
{
}

Game::PlayerGrabJumpLeft::PlayerGrabJumpLeft(Player* playerRef, BaseStateMachine* fsm)
	: BaseState(fsm)
	, playerRef(playerRef)
	, currVel(Vec2Zero())
	, prevVel(Vec2Zero())
	, currPos(Vec2Zero())
	, prevPos(Vec2Zero())
{
}

Game::PlayerGrabJumpLeft::~PlayerGrabJumpLeft()
{
}

void Game::PlayerGrabJumpLeft::Enter()
{
	playerRef->ChangeAnimation(Player::PLAYER_SPRITE::PL_GRAB_JUMP_LEFT);

}

void Game::PlayerGrabJumpLeft::Update()
{
	prevPos = currPos;
	currPos = playerRef->GetPosition();
	prevVel = currVel;
	currVel = playerRef->GetVelocity();
	Vec2 relPos = prevPos - currPos;
	Vec2Abs(&relPos);
	Vec2 relVel = prevVel - currVel;
	Vec2Abs(&relVel);

	if (playerRef->grabbedBlock.object == nullptr)
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_JUMP_LEFT);
	}

	if (currVel.y < 0.0f)
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_GRAB_FALL_LEFT);
	}
	if (currVel.x > 0.0f)
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_GRAB_JUMP_RIGHT);
	}

}

void Game::PlayerGrabJumpLeft::Exit()
{
}


Game::PlayerGrabJumpRight::PlayerGrabJumpRight(Player* playerRef, BaseStateMachine* fsm)
	: BaseState(fsm)
	, playerRef(playerRef)
	, currVel(Vec2Zero())
	, prevVel(Vec2Zero())
	, currPos(Vec2Zero())
	, prevPos(Vec2Zero())
{
}

Game::PlayerGrabJumpRight::~PlayerGrabJumpRight()
{
}

void Game::PlayerGrabJumpRight::Enter()
{
	playerRef->ChangeAnimation(Player::PLAYER_SPRITE::PL_GRAB_JUMP_RIGHT);

}

void Game::PlayerGrabJumpRight::Update()
{
	prevPos = currPos;
	currPos = playerRef->GetPosition();
	prevVel = currVel;
	currVel = playerRef->GetVelocity();
	Vec2 relPos = prevPos - currPos;
	Vec2Abs(&relPos);
	Vec2 relVel = prevVel - currVel;
	Vec2Abs(&relVel);

	if (playerRef->grabbedBlock.object == nullptr)
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_JUMP_RIGHT);
	}

	if (currVel.y < 0.0f)
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_GRAB_FALL_RIGHT);
	}
	if (currVel.x < 0.0f)
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_GRAB_JUMP_LEFT);
	}
}

void Game::PlayerGrabJumpRight::Exit()
{
}


Game::PlayerGrabFallLeft::PlayerGrabFallLeft(Player* playerRef, BaseStateMachine* fsm)
	: BaseState(fsm)
	, playerRef(playerRef)
	, currVel(Vec2Zero())
	, prevVel(Vec2Zero())
	, currPos(Vec2Zero())
	, prevPos(Vec2Zero())
{
}

Game::PlayerGrabFallLeft::~PlayerGrabFallLeft()
{
}

void Game::PlayerGrabFallLeft::Enter()
{
	playerRef->ChangeAnimation(Player::PLAYER_SPRITE::PL_GRAB_FALL_LEFT);

}

void Game::PlayerGrabFallLeft::Update()
{
	prevPos = currPos;
	currPos = playerRef->GetPosition();
	prevVel = currVel;
	currVel = playerRef->GetVelocity();
	Vec2 relPos = prevPos - currPos;
	Vec2Abs(&relPos);
	Vec2 relVel = prevVel - currVel;
	Vec2Abs(&relVel);

	if (playerRef->grabbedBlock.object == nullptr)
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_FALL_LEFT);
	}

	if (currVel.y > 0.0f)
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_GRAB_JUMP_LEFT);
	}
	else if (relPos.y < C_epsilon)
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_GRAB_IDLE_LEFT);
	}
	if (currVel.x > 0.0f)
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_GRAB_FALL_RIGHT);
	}
}

void Game::PlayerGrabFallLeft::Exit()
{
}

Game::PlayerGrabFallRight::PlayerGrabFallRight(Player* playerRef, BaseStateMachine* fsm)
	: BaseState(fsm)
	, playerRef(playerRef)
	, currVel(Vec2Zero())
	, prevVel(Vec2Zero())
	, currPos(Vec2Zero())
	, prevPos(Vec2Zero())
{
}

Game::PlayerGrabFallRight::~PlayerGrabFallRight()
{
}

void Game::PlayerGrabFallRight::Enter()
{
	playerRef->ChangeAnimation(Player::PLAYER_SPRITE::PL_GRAB_FALL_RIGHT);
}

void Game::PlayerGrabFallRight::Update()
{
	prevPos = currPos;
	currPos = playerRef->GetPosition();
	prevVel = currVel;
	currVel = playerRef->GetVelocity();
	Vec2 relPos = prevPos - currPos;
	Vec2Abs(&relPos);
	Vec2 relVel = prevVel - currVel;
	Vec2Abs(&relVel);

	if (playerRef->grabbedBlock.object == nullptr)
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_FALL_RIGHT);
	}

	if (currVel.y > 0.0f)
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_GRAB_JUMP_RIGHT);
	}
	else if (relPos.y < C_epsilon)
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_GRAB_IDLE_RIGHT);
	}
	if (currVel.x < 0.0f)
	{
		fsm->ChangeState(Player::PLAYER_SPRITE::PL_GRAB_FALL_LEFT);
	}
}

void Game::PlayerGrabFallRight::Exit()
{
}
