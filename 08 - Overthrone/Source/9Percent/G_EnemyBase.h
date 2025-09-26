/******************************************************************************/
/*!
\file   G_EnemyBase.h
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Marc Alviz Evangelista (90%)
\par    email: marcalviz.e\@digipen.edu
\par    DigiPen login: marcalviz.e

\author Kendrick Sim Hean Guan (5%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\author Ryan Chan Kuan Fu (5%)
\par    email: c.kuanfuryan\@digipen.edu
\par    DigiPen login: c.kuanfuryan

\brief
  This is the interface file for all member functions of class EnemyBase.
  This includes utility functions

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#pragma once
#include "pch.h"
#include "E_Object.h"
#include "E_Camera.h"
#include "G_Map.h"
#include "G_HealthDisplay.h"

namespace Game {
	using namespace Engine;


	enum ENEMYDIRECTION {
		EDIR_LEFT = 0,
		EDIR_RIGHT
	};


	// Other enemies will take from this
	// examples: Enemy_skeleton, Enemy_zombie
	class EnemyBase : public PhysicsObject {
	protected:
		f32 hitPoints;
		f32 hitPointsMax;
		f32 actionCooldown;				// Base range +- a percent for the enemy to do stuff
		f32 aggroRange;					// For aggro-ing to the player, must be within aggro range, facing player and same or lower y
		f32 moveSpeed;					// multiplier for the speed of the enemy
		f32 invincibilityTimer;			
		f32 invincibleCountdown;
		ENEMYDIRECTION currDir;
		bool foundPlayer;
		bool isAlive;
		bool isInvincible;
		bool atTurningPoint;			// For changing directions
		Vec2 initialPos;

		std::vector<sPtr<Sprite>> spriteList; // Store different animations here
		sPtr<HealthDisplay> healthDisplay;	// Hi it's Ryan here just adding some health displays

		void UpdateHealthDisplay();

	public:
		EnemyBase();
		EnemyBase(bool isDynamic, const Transform& transform, const sPtr<Sprite>& spritePtr, f32 aggroRange, f32 moveSpeed);
		~EnemyBase();

		void Init();
		void Update();
		void SetActionTimer(f32 timer);
		bool UpdateActionTimer(f32 timerCooldown);
		bool GetPlayerFoundFlag(); // Returns foundPlayer bool
		bool GetAlive();
		bool GetIsInvincible();
		void TakeDamage(f32 damage);
		void RenderHealth();
		virtual void Die();

		bool GetTurningPoint();
		void SetTurningPoint(bool b);

		ENEMYDIRECTION GetDirection() const;
		void SetDirection(ENEMYDIRECTION dir);

		void OnCollision(Engine::PhysicsObject& thisObj, Engine::PhysicsObject& otherObj, Engine::Physics::PhysicsCollisionData& collisionData);
	};
}
