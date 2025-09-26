/******************************************************************************/
/*!
\file   G_EnemyBoss.cpp
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

#include "pch.h"
#include "G_EnemyBoss.h"
#include "G_ParticleSystem.h"
#include "E_Camera.h"
#include "G_GameState.h"

namespace Game
{
	EnemyBoss::EnemyBoss()
		: timer(0.0f)
		, pType(PT_SPIRAL)
		, isDying(false)
		, interval(0.0f)
		, hasAttacked(false)
		, deathPos(Vec2Zero())
		, bossAnimInit(false)
	{
	}

	void EnemyBoss::addProjectile(PREDICTOR_TYPE predType, f32 angle, bool isPositive = true)
	{
		sPtr<PredictorProjectile> p = std::make_shared<PredictorProjectile>();
		p->Init(predType, this->GetPosition(), angle, isPositive);
		projectiles.push_back(p);
	}

	void EnemyBoss::SetBossHasAttacked(bool b)
	{
		hasAttacked = b;
	}

	void EnemyBoss::SetBossAnimInit(bool b)
	{
		bossAnimInit = b;
	}

	void EnemyBoss::InitAnimationTree()
	{
		if (bossAnimInit)
			return;
		bossAnimInit = true;
		bossAnims.emplace(BOSS_IDLE, SpriteBuilder::CreateSpriteSquare("Textures/Boss/BossIdle.png"));
		bossAnims.emplace(BOSS_ATTACK, SpriteBuilder::CreateSpriteSquare("Textures/Boss/BossAttack.png", true, 1, 6, C_bossAttackAnimSpeed, 6));

		bossAnimFSM.InsertState(new BossIdle(this, &bossAnimFSM), BOSS_IDLE);
		bossAnimFSM.InsertState(new BossAttack(this, &bossAnimFSM), BOSS_ATTACK);
		bossAnimFSM.Init(bossAnimFSM.GetState(BOSS_IDLE));
	}

	void EnemyBoss::ChangeAnimation(BOSS_SPRITE newSprite)
	{
		SetSprite(bossAnims[newSprite]);
	}

	bool EnemyBoss::GetHasAttacked()
	{
		return hasAttacked;
	}

	void EnemyBoss::Init()
	{
		interval = 5.0f;
		timer = interval;
		pType = PT_SPIRAL;
		hitPoints = 250.0f;
		hitPointsMax = hitPoints;
		isDying = false;
		// Init animation tree
		hasAttacked = false;
		InitAnimationTree();
		EnemyBase::Init();
	}

	void EnemyBoss::Update()
	{
		/* Hacks: Instakill Boss */
		if (AEInputCheckTriggered(AEVK_L))
			Die();

		bossAnimFSM.Update();
		spritePtr->Update(Engine::C_frametime);
		if (isDying)
		{
			timer -= C_frametime;
			if (timer <= 0.0f)
				EnemyBase::Die();
			else
			{
				int timesShaken = sCast<int>(timer / C_bossDeathShakeInterval);
				SetPosition(deathPos + MakeVec2((timesShaken & 1 ? C_blockLength : -C_blockLength) * 0.12f, 0.0f));
			}
			return;
		}

		if (isInvincible)
		{
			invincibleCountdown -= C_frametime;
			if (invincibleCountdown <= 0.0f)
			{
				isInvincible = false;
			}
		}

		if (timer < interval)
		{
			timer += Engine::C_frametime;
		}
		else
		{
			// Set has attacked to true
			hasAttacked = true;
			// Create a predictor projectile
			timer = 0.0f;
			switch (pType)
			{
			case PT_SPIRAL:
			{
				for (f32 angle = 0.0f; angle <= 360.0f; angle += 90.0f)
				{
					addProjectile(pType, angle);
				}
				pType = PT_SINE;
				break;
			}
			case PT_SINE:
			{
				for (f32 angle = 0.0f; angle <= 360.0f; angle += 18.0f)
				{
					addProjectile(pType, angle);
				}
				pType = PT_SQUARED;
				break;
			}
			case PT_SQUARED:
			{
				for (f32 angle = 0.0f; angle <= 90.0f; angle += 90.0f)
				{
					addProjectile(pType, 0.0f + angle);
					addProjectile(pType, 0.0f + angle, false);
					addProjectile(pType, 180.0f + angle);
					addProjectile(pType, 180.0f + angle, false);
				}
				pType = PT_SPIRAL;
				break;
			}
			default: { break; }
			}
		}

		// Iterate through projectile and check active
		for (auto p = projectiles.begin(); p != projectiles.end(); )
		{
			if (!(*p)->isActive())
			{
				(*p)->Exit();
				(*p).reset();

				// Erase and return next iterator
				p = projectiles.erase(p);
			}
			else 
			{
				(*p)->Update();
				++p; 
			}
		}

		// Update Enemy Base
		EnemyBase::Update();
	}

	void EnemyBoss::Render()
	{
		if (!isAlive) { return; }

		for (const auto& p : projectiles)
		{
			p->Render();
		}
		spritePtr->Render(transform, transparency, colorTint);

		EnemyBase::RenderHealth();
	}

	void EnemyBoss::Die()
	{
		// Iterate through projectile and check active
		for (auto p = projectiles.begin(); p != projectiles.end(); )
		{
			(*p)->setActive(false);
			(*p)->Exit();
			(*p).reset();

			// Erase and return next iterator
			p = projectiles.erase(p);
		}

		//Spawn End Goal
		sPtr<Map> map = Engine::Singleton<Map>::Instance();
		Block* b = map->GetBlockAtIndex(this->GetPosition());
		b->FillBlock(BT_END);

		// Spawn particles
		Singleton<ParticleManager>::Instance()->AddObject(PARTICLE_LAYER_GAME, sPtr<ParticleSpawnerBase>(new ParticleSpawnerDeathShine(GetPosition())));
		Singleton<ParticleManager>::Instance()->AddObject(PARTICLE_LAYER_ABOVE_DYNAMIC, sPtr<ParticleSpawnerBase>(new ParticleSpawnerBossDieExplosion(shared_from_this())));

		// Set unalive and timer until the boss is deleted.
		isDying = true;
		timer = C_bossDeathTime;
		deathPos = GetPosition();
		
		// Update health amount
		UpdateHealthDisplay();

		// Setup boss dead cam manager
		Singleton<EnemyBossDeadCamManager>::Instance()->Init(GetPosition());
	}

	BossIdle::BossIdle(EnemyBoss* bossRef, BaseStateMachine* fsm)
		: BaseState(fsm)
		, ref(bossRef)
	{
	}
	BossIdle::~BossIdle()
	{
	}
	void BossIdle::Enter()
	{
		ref->ChangeAnimation(BOSS_IDLE);
	}
	void BossIdle::Update()
	{
		if (ref->GetHasAttacked())
		{
			fsm->ChangeState(BOSS_ATTACK);
		}
	}
	void BossIdle::Exit()
	{
	}
	BossAttack::BossAttack(EnemyBoss* bossRef, BaseStateMachine* fsm)
		: BaseState(fsm)
		, ref(bossRef)
	{
		timer = C_bossAttackAnimSpeed;
	}
	BossAttack::~BossAttack()
	{
	}
	void BossAttack::Enter()
	{
		ref->ChangeAnimation(BOSS_ATTACK);
		ref->SetBossHasAttacked(false);
		timer = C_bossAttackAnimSpeed;
	}
	void BossAttack::Update()
	{
		timer -= Engine::C_frametime;
		if (timer <= 0.0f)
		{
			fsm->ChangeState(BOSS_IDLE);
		}
	}
	void BossAttack::Exit()
	{
	}

	EnemyBossDeadCamManager::EnemyBossDeadCamManager()
		: startPosition(Vec2Zero())
		, targetPosition(Vec2Zero())
		, timer(0.0f)
	{
	}

	void EnemyBossDeadCamManager::Init(const Vec2& bossDeadPos)
	{
		startPosition = Singleton<Camera>::Instance()->GetPosition();
		targetPosition = bossDeadPos;
		timer = 0.0f;

		Singleton<GameStateManager>::Instance()->SetIsFocusBossDying(true);
	}

	void EnemyBossDeadCamManager::Update(float dt)
	{
		timer += dt;
		if (timer >= C_bossDeathTime + C_bossDeathAfterTime)
		{
			Singleton<GameStateManager>::Instance()->SetIsFocusBossDying(false);
			return;
		}

		float ratio = Util::Clamp(timer / C_bossDeathLerpTime, 0.0f, 1.0f);
		Singleton<Camera>::Instance()->SetPosition(MakeVec2(
			Util::Lerp(startPosition.x, targetPosition.x, Util::LerpRatioEaseInOut(ratio)),
			Util::Lerp(startPosition.y, targetPosition.y, Util::LerpRatioEaseInOut(ratio))
		));
	}

}
