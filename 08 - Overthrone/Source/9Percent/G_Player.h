/******************************************************************************/
/*!
\file   G_Player.h
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Matthew Chan Shao Jie (40%)
\par    email: m.chan\@digipen.edu
\par    DigiPen login: m.chan

\author Marc Alviz Evangelista (25%)
\par    email: marcalviz.e\@digipen.edu
\par    DigiPen login: marcalviz.e

\author Ryan Chan Kuan Fu (25%)
\par    email: c.kuanfuryan\@digipen.edu
\par    DigiPen login: c.kuanfuryan

\author Kendrick Sim Hean Guan (10%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\brief
  This is the interface file for all member functions of a class called
  Player, along with different utility functions for players.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#pragma once
#include "pch.h"
#include "E_Object.h"
#include "E_Camera.h"
#include "G_Map.h"
#include "G_GrappleHook.h"
#include "G_ArcPredictor.h"
#include "G_GrabDisplay.h"
#include "G_HealthDisplay.h"
#include "E_BaseStateMachine.h"

#define GRAB_BUTTON AEVK_LBUTTON
#define DASH_BUTTON AEVK_LSHIFT
#define GRAPPLE_BUTTON AEVK_RBUTTON

namespace Game
{
	class Player : public Engine::PhysicsObject {
	public:
		enum PLAYER_STATE
		{
			S_ALIVE,
			S_DEAD,//u r die
			S_COMPLETE,//Lol
		};
		// Hi marc here with enums for player animation tree
		// player sprite will change accordingly to these
		enum PLAYER_SPRITE
		{
			PL_IDLE_LEFT,
			PL_IDLE_RIGHT,
			PL_MOVE_LEFT,			
			PL_MOVE_RIGHT,
			PL_JUMP_LEFT,
			PL_JUMP_RIGHT,
			PL_FALL_LEFT,
			PL_FALL_RIGHT,

			PL_GRAB_IDLE_LEFT,
			PL_GRAB_IDLE_RIGHT,
			PL_GRAB_MOVE_LEFT,
			PL_GRAB_MOVE_RIGHT,
			PL_GRAB_JUMP_LEFT,
			PL_GRAB_JUMP_RIGHT,
			PL_GRAB_FALL_LEFT,
			PL_GRAB_FALL_RIGHT
		};

		Engine::BaseStateMachine animationFSM;
		std::map<PLAYER_SPRITE, sPtr<Engine::Sprite>> playerSpriteAnims;

		bool playerAnimInit;

		sPtr<ArcPredictor> arcPredictor;
		sPtr<GrappleHook> grapple;
		sPtr<Engine::Object> dashIndicator;	// Just a generic texture
		sPtr<GrabDisplay> grabDisplay;
		sPtr<HealthDisplay> healthDisplay;
		Vec2 initialPos;

		// Reset any variables that shouldn't be brought to the new level
		void OnNewLevelLoad();

		void Update() override;
		void Render() override;
		Player();
		virtual ~Player();
		Vec2 pos;
		Vec2 vel;
		Vec2 force;//Adds accel to player every tick, resets to 0 after
		f32 health;
		f32 isInvincible;
		
		sPtr<Engine::Graphics::Camera> camReference;
		s32 mx, my;

		s8 currentState;
		Block* currentBlock;

		s32 xindex;
		s32 yindex;
		float grabRadius = 4.0f;
		Block grabbedBlock;
		Block* highlightedBlock;

		std::string abilityname;

		Object* radiusIndicator;
		float radiusPingTransparency;
		f32 dragCoEff = 0.8f;
		f32 dashDragCoEff = 3.0f;
		f32 dashTime = 1.0f;

		f32 defaultAccel = 90.0f;
		f32 swimAccel = 60.0f;
		f32 airAccel = 30.0f;

		

		sPtr<Engine::Text> abilityText;

		bool isGrounded;
		bool isSwimming;
		int dashAvailable;
		bool chargingDash = false;
		void AddForce(Vec2 dir);//Non-normalised vector
		void AddForce(Vec2 dir, float force);//Normalised vector with force multiplier
		void AddForce(float x, float y, float force);//X and Y component with force multiplier
		
		void TakeDamage(f32 damage);
		void Die();//ur ded
		void Respawn();//ur unded

		void OnCollision(Engine::PhysicsObject& thisObj, Engine::PhysicsObject& otherObj, Engine::Physics::CollisionData& collisionData);
		
		// Functions for the animation tree - Marc
		void InitAnimationTree();
		void ChangeAnimation(PLAYER_SPRITE newSprite);
		void AddToTree(sPtr<Engine::Sprite> sprite, PLAYER_SPRITE key);
	};
}