/******************************************************************************/
/*!
\file   G_EnemyBoss.h
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Ryan Chan Kuan Fu (55%)
\par    email: c.kuanfuryan\@digipen.edu
\par    DigiPen login: c.kuanfuryan

\author Marc Alviz Evangelista (30%)
\par    email: marcalviz.e\@digipen.edu
\par    DigiPen login: marcalviz.e

\author Kendrick Sim Hean Guan (15%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\brief
  Boss Enemy, my magnum opus. This is the culmination of EnemyBase, projectiles,
  line renderer, predictor projectiles, sprites, etc. Shame you only get to see
  it when you reach level 3.

  Enemy Base, projectiles and sprites are from Marc, while the killed "shaking"
  animation comes from Kendrick.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#pragma once
#include "pch.h"
#include "G_EnemyBase.h"
#include "G_PredictorProjectile.h"
#include "E_BaseStateMachine.h"

namespace Game
{
	enum  BOSS_SPRITE {
		BOSS_IDLE,
		BOSS_ATTACK
	};
	class EnemyBoss : public EnemyBase
	{
	protected:
		f32 timer;
		f32 interval;
		std::vector<sPtr<PredictorProjectile>> projectiles;
		PREDICTOR_TYPE pType;
		bool isDying;
		// The position when death occured, to facilitate offsetting the boss for shaking
		Vec2 deathPos;
		void addProjectile(PREDICTOR_TYPE predType, f32 angle, bool isPositive);

		Engine::BaseStateMachine bossAnimFSM;
		std::map<BOSS_SPRITE, sPtr<Engine::Sprite>> bossAnims;
		bool bossAnimInit;
		bool hasAttacked;
	public:
		EnemyBoss();

		void SetBossHasAttacked(bool b);
		void SetBossAnimInit(bool b);
		void InitAnimationTree();
		void ChangeAnimation(BOSS_SPRITE newSprite);
		bool GetHasAttacked();

		void Init();
		void Update() override;
		void Render() override;
		void Die() override;
	};


	class BossIdle : public BaseState
	{
	private:
		EnemyBoss* ref;
	public:
		BossIdle(EnemyBoss* bossRef, BaseStateMachine* fsm);
		~BossIdle();
		virtual void Enter() override;
		virtual void Update() override;
		virtual void Exit() override;
	};

	class BossAttack : public BaseState
	{
	private:
		EnemyBoss* ref;
		f32 timer;
	public:
		BossAttack(EnemyBoss* bossRef, BaseStateMachine* fsm);
		~BossAttack();
		virtual void Enter() override;
		virtual void Update() override;
		virtual void Exit() override;
	};
	// For managing the camera when the boss dies.
	class EnemyBossDeadCamManager
	{
	protected:
		// Where the camera was when the boss died
		Vec2 startPosition;
		// Where the boss died
		Vec2 targetPosition;

		float timer;

	public:
		EnemyBossDeadCamManager();

		void Init(const Vec2& bossDeadPos);
		void Update(float dt);

	};
}