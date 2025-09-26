/******************************************************************************/
/*!
\file   G_PlayerAnimationTree.h
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Marc Alviz Evangelista (100%)
\par    email: marcalviz.e\@digipen.edu
\par    DigiPen login: marcalviz.e

\brief
  This is the interface file for all member functions of the classes
  of animation states for the Player Animation Tree.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#pragma once
#include "pch.h"
#include "E_BaseStateMachine.h"
#include "E_BaseState.h"
#include "G_Player.h"


namespace Game {
	using namespace Engine;


	class PlayerIdleLeft : public BaseState {
		Player* playerRef;
		Vec2 currVel;
		Vec2 prevVel;
		Vec2 currPos;
		Vec2 prevPos;
	public:
		PlayerIdleLeft(Player* playerRef, BaseStateMachine* fsm);
		~PlayerIdleLeft();

		virtual void Enter() override;
		virtual void Update() override;
		virtual void Exit() override;
	};
	class PlayerIdleRight : public BaseState {
		Player* playerRef;
		Vec2 currVel;
		Vec2 prevVel;
		Vec2 currPos;
		Vec2 prevPos;
	public:
		PlayerIdleRight(Player* playerRef, BaseStateMachine* fsm);
		~PlayerIdleRight();

		virtual void Enter() override;
		virtual void Update() override;
		virtual void Exit() override;
	};

	class PlayerMoveLeft : public BaseState {
		Player* playerRef;
		Vec2 currVel;
		Vec2 prevVel;
		Vec2 currPos;
		Vec2 prevPos;
	public:
		PlayerMoveLeft(Player* playerRef, BaseStateMachine* fsm);
		~PlayerMoveLeft();
		virtual void Enter() override;
		virtual void Update() override;
		virtual void Exit() override;
	};
	class PlayerMoveRight : public BaseState {
		Player* playerRef;
		Vec2 currVel;
		Vec2 prevVel;
		Vec2 currPos;
		Vec2 prevPos;
	public:
		PlayerMoveRight(Player* playerRef, BaseStateMachine* fsm);
		~PlayerMoveRight();
		virtual void Enter() override;
		virtual void Update() override;
		virtual void Exit() override;
	};

	class PlayerJumpLeft : public BaseState {
		Player* playerRef;
		Vec2 currVel;
		Vec2 prevVel;
		Vec2 currPos;
		Vec2 prevPos;
	public:
		PlayerJumpLeft(Player* playerRef, BaseStateMachine* fsm);
		~PlayerJumpLeft();
		virtual void Enter() override;
		virtual void Update() override;
		virtual void Exit() override;
	};
	class PlayerJumpRight : public BaseState {
		Player* playerRef;
		Vec2 currVel;
		Vec2 prevVel;
		Vec2 currPos;
		Vec2 prevPos;
	public:
		PlayerJumpRight(Player* playerRef, BaseStateMachine* fsm);
		~PlayerJumpRight();
		virtual void Enter() override;
		virtual void Update() override;
		virtual void Exit() override;
	};
	class PlayerFallLeft : public BaseState {
		Player* playerRef;
		Vec2 currVel;
		Vec2 prevVel;
		Vec2 currPos;
		Vec2 prevPos;
	public:
		PlayerFallLeft(Player* playerRef, BaseStateMachine* fsm);
		~PlayerFallLeft();
		virtual void Enter() override;
		virtual void Update() override;
		virtual void Exit() override;
	};
	class PlayerFallRight : public BaseState {
		Player* playerRef;
		Vec2 currVel;
		Vec2 prevVel;
		Vec2 currPos;
		Vec2 prevPos;
	public:
		PlayerFallRight(Player* playerRef, BaseStateMachine* fsm);
		~PlayerFallRight();
		virtual void Enter() override;
		virtual void Update() override;
		virtual void Exit() override;
	};
	//***************************************** Grab State *****************************************//
	class PlayerGrabIdleLeft : public BaseState {
		Player* playerRef;
		Vec2 currVel;
		Vec2 prevVel;
		Vec2 currPos;
		Vec2 prevPos;
	public:
		PlayerGrabIdleLeft(Player* playerRef, BaseStateMachine* fsm);
		~PlayerGrabIdleLeft();
		virtual void Enter() override;
		virtual void Update() override;
		virtual void Exit() override;
	};
	class PlayerGrabIdleRight : public BaseState {
		Player* playerRef;
		Vec2 currVel;
		Vec2 prevVel;
		Vec2 currPos;
		Vec2 prevPos;
	public:
		PlayerGrabIdleRight(Player* playerRef, BaseStateMachine* fsm);
		~PlayerGrabIdleRight();
		virtual void Enter() override;
		virtual void Update() override;
		virtual void Exit() override;
	};
	class PlayerGrabMoveLeft : public BaseState {
		Player* playerRef;
	public:
		Vec2 currVel;
		Vec2 prevVel;
		Vec2 currPos;
		Vec2 prevPos;
		PlayerGrabMoveLeft(Player* playerRef, BaseStateMachine* fsm);
		~PlayerGrabMoveLeft();
		virtual void Enter() override;
		virtual void Update() override;
		virtual void Exit() override;
	};
	class PlayerGrabMoveRight : public BaseState {
		Player* playerRef;
	public:
		Vec2 currVel;
		Vec2 prevVel;
		Vec2 currPos;
		Vec2 prevPos;
		PlayerGrabMoveRight(Player* playerRef, BaseStateMachine* fsm);
		~PlayerGrabMoveRight();
		virtual void Enter() override;
		virtual void Update() override;
		virtual void Exit() override;
	};
	class PlayerGrabJumpLeft : public BaseState {
		Player* playerRef;
	public:
		Vec2 currVel;
		Vec2 prevVel;
		Vec2 currPos;
		Vec2 prevPos;
		PlayerGrabJumpLeft(Player* playerRef, BaseStateMachine* fsm);
		~PlayerGrabJumpLeft();
		virtual void Enter() override;
		virtual void Update() override;
		virtual void Exit() override;
	};
	class PlayerGrabJumpRight : public BaseState {
		Player* playerRef;
	public:
		Vec2 currVel;
		Vec2 prevVel;
		Vec2 currPos;
		Vec2 prevPos;
		PlayerGrabJumpRight(Player* playerRef, BaseStateMachine* fsm);
		~PlayerGrabJumpRight();
		virtual void Enter() override;
		virtual void Update() override;
		virtual void Exit() override;
	};
	class PlayerGrabFallLeft : public BaseState {
		Player* playerRef;
	public:
		Vec2 currVel;
		Vec2 prevVel;
		Vec2 currPos;
		Vec2 prevPos;
		PlayerGrabFallLeft(Player* playerRef, BaseStateMachine* fsm);
		~PlayerGrabFallLeft();
		virtual void Enter() override;
		virtual void Update() override;
		virtual void Exit() override;
	};
	class PlayerGrabFallRight : public BaseState {
		Player* playerRef;
	public:
		Vec2 currVel;
		Vec2 prevVel;
		Vec2 currPos;
		Vec2 prevPos;
		PlayerGrabFallRight(Player* playerRef, BaseStateMachine* fsm);
		~PlayerGrabFallRight();
		virtual void Enter() override;
		virtual void Update() override;
		virtual void Exit() override;
	};




}

