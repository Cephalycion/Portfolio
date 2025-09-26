/******************************************************************************/
/*!
\file   G_EnemyTurret.cpp
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Marc Alviz Evangelista (90%)
\par    email: marcalviz.e\@digipen.edu
\par    DigiPen login: marcalviz.e

\author Ryan Chan Kuan Fu (10%)
\par    email: c.kuanfuryan\@digipen.edu
\par    DigiPen login: c.kuanfuryan

\brief
  This is the interface file for all member functions of the classes EnemyTurret
  and TurretBuilder.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#include "pch.h"
#include "G_EnemyTurret.h"
#include "G_GameState.h"
#include "E_BaseState.h"
#include "G_Player.h"
#include "E_Singleton.h"



namespace Game {
	using namespace Engine;


	EnemyTurret::EnemyTurret()
		: EnemyBase() 
	{
	}

	EnemyTurret::EnemyTurret(const Transform& transform, const sPtr<Sprite>& spritePtr)
		: EnemyBase(false, transform, spritePtr, 750.0f, 0.0f)
	{
	}


	EnemyTurret::~EnemyTurret()
	{
	}

	void EnemyTurret::Init()
	{
		fsm.InsertState(new TurretIdleState(this, &fsm), TURRETSTATES::T_IDLE);
		fsm.InsertState(new TurretAggroState(this, &fsm), TURRETSTATES::T_AGGRO);
		fsm.Init(fsm.GetState(TURRETSTATES::T_IDLE));
		ln = std::make_shared<Engine::LineRenderer>();
		ln->Init();
		ln->SetColor(Engine::C_colorOrange);
		EnemyBase::Init();
	}

	void EnemyTurret::Update()
	{
		fsm.Update();
		spritePtr->Update(C_frametime);

		if (isInvincible)
		{
			invincibleCountdown -= Game::C_frametime;
			if (invincibleCountdown <= 0.0f)
			{
				isInvincible = false;
			}
		}
		EnemyBase::Update();
		if (foundPlayer)
		{
			Vec2 EnemytoPlayer = Singleton<Player>::Instance()->GetPosition() - transform.GetPosition();
			Engine::Physics::PhysicsRaycastData rcd2;
			Engine::Physics::PhysicsManager::Raycast(transform.GetPosition(), Vec2Normalize(EnemytoPlayer), { Engine::Physics::MASK_ENVIRONMENT }, &rcd2);
			ln->SetLine(transform.GetPosition(), rcd2.collisionPoint, C_laserWidth);

		}
	}

	void EnemyTurret::Render()
	{
		switch (fsm.GetStateIndex())
		{
		case TURRETSTATES::T_IDLE:
			spritePtr->Render(transform, transparency, colorTint);
			break;
		case TURRETSTATES::T_AGGRO:
			ln->Render();
			spritePtr->Render(transform, transparency, colorTint);
			break;
		default:
			break;
		}
		EnemyBase::RenderHealth();
	}

	void EnemyTurret::DoAction(TURRETSTATES state)
	{
		Engine::Physics::PhysicsRaycastData rcd;
		if (Singleton<GameStateManager>::Instance()->GetIsLevelClear())
		{
			Die();
			return;
		}
		switch (state)
		{
		case Game::T_IDLE:
		{
			Vec2 EnemytoPlayer = Singleton<Player>::Instance()->GetPosition() - transform.GetPosition();
			if (Engine::Physics::PhysicsManager::Raycast(transform.GetPosition(), Vec2Normalize(EnemytoPlayer), {Engine::Physics::MASK_PLAYER}, &rcd))
			{
				sPtr<Player> p = std::dynamic_pointer_cast<Player>(rcd.hitObj->GetPhysObject()->shared_from_this());
				if (p != nullptr)
				{
					if (rcd.distance < aggroRange)
					{
						foundPlayer = true;
					}
				}
			}
			break;
		}
		case Game::T_AGGRO:
		{
			Vec2 EnemytoPlayer = Singleton<Player>::Instance()->GetPosition() - transform.GetPosition();
			if (Engine::Physics::PhysicsManager::Raycast(transform.GetPosition(), Vec2Normalize(EnemytoPlayer), { Engine::Physics::MASK_PLAYER }, &rcd))
			{
				sPtr<Player> p = std::dynamic_pointer_cast<Player>(rcd.hitObj->GetPhysObject()->shared_from_this());
				if (p != nullptr)
				{
					if (rcd.distance > aggroRange)
					{
						foundPlayer = false; // player outside range
					}
				}
				else
				{
					foundPlayer = false; // Lost Line of sight to player
				}
			}

			// Create a fireball or smth here 
			const Vec2 velocity = Vec2Normalize(Singleton<Player>::Instance()->GetPosition() - transform.GetPosition());
			if (foundPlayer)
			{
				sPtr<Projectile> p = ProjectileBuilder::CreateProjectile(transform, velocity);
				p->SetColorTint(Engine::C_colorWhite);
				Singleton<ProjectileManager>::Instance()->AddProjectile(p);
			}

			break;
		}
		default:
		{
			break;
		}
		}
	}

	sPtr<EnemyTurret> TurretBuilder::CreateTurret(const Transform& transform, const sPtr<Sprite> spritePtr)
	{
		return sPtr<EnemyTurret>(new EnemyTurret(
			transform,
			spritePtr
		));
	}

}
