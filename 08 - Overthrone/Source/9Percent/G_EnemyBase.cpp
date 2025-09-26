/******************************************************************************/
/*!
\file   G_EnemyBase.cpp
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
  This is the implementation file for all member functions of class EnemyBase.
  This includes utility functions

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#include "pch.h"
#include "G_EnemyBase.h"
#include "G_Block.h"
#include "G_LevelObjects.h"
#include "E_AudioManager.h"
#include "G_ParticleSystem.h"

Game::EnemyBase::EnemyBase() // base physics object builder in here maybe idk
	: hitPoints(100.0f)
	, hitPointsMax(hitPoints)
	, actionCooldown(4.0f)
	, moveSpeed(100.0f)
	, aggroRange(50.0f)
	, invincibilityTimer(1.0f)
	, invincibleCountdown(invincibilityTimer)
	, currDir(ENEMYDIRECTION::EDIR_LEFT)
	, isInvincible(false)
	, foundPlayer(false)
	, isAlive(true)
	, atTurningPoint(false)
	, initialPos()
{
}

Game::EnemyBase::EnemyBase(bool isDynamic, const Transform& transform, const sPtr<Sprite>& spritePtr, f32 aggroRange, f32 moveSpd)
	: PhysicsObject(true, isDynamic, transform, spritePtr, 1.0f, Color())
	, hitPoints(100.0f)
	, hitPointsMax(hitPoints)
	, actionCooldown(0.5f)
	, moveSpeed(moveSpd)
	, aggroRange(aggroRange)
	, invincibilityTimer(1.0f)
	, invincibleCountdown(invincibilityTimer)
	, currDir(ENEMYDIRECTION::EDIR_LEFT)
	, isInvincible(false)
	, foundPlayer(false)
	, isAlive(true)
	, atTurningPoint(false)
	, initialPos()
{
}


Game::EnemyBase::~EnemyBase()
{
}

void Game::EnemyBase::UpdateHealthDisplay()
{
	healthDisplay->SetHealthDisplay(hitPoints / hitPointsMax, GetPosition() - initialPos, GetPosition(), GetRotation());
}

bool Game::EnemyBase::UpdateActionTimer(f32 cooldown)
{
	actionCooldown -= C_frametime;
	if (actionCooldown < 0.0f)
	{
		actionCooldown = cooldown;
		return true;
	}
	return false;
}

void Game::EnemyBase::Init()
{
	SetOnCollisionCallback(std::bind(&EnemyBase::OnCollision, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	healthDisplay = std::make_shared<HealthDisplay>();
	Vec2 startPos = MakeVec2(GetPosition().x, GetPosition().y) + C_healthPosStart;
	healthDisplay->Init(C_blockLength, C_blockLength, startPos);
	initialPos = GetPosition();
}

void Game::EnemyBase::Update()
{
	UpdateHealthDisplay();
}

void Game::EnemyBase::SetActionTimer(f32 timer)
{
	actionCooldown = timer;
}

bool Game::EnemyBase::GetPlayerFoundFlag()
{
	return foundPlayer;
}

bool Game::EnemyBase::GetAlive()
{
	return isAlive;
}

bool Game::EnemyBase::GetIsInvincible()
{
	return invincibleCountdown < 0.0f;
}

void Game::EnemyBase::TakeDamage(f32 damage)
{
	Engine::AudioManager::playAudio(Engine::SFX_HURT, false);
	healthDisplay->OnChange();
	hitPoints -= damage;
	if (hitPoints <= 0.0f)
	{
		Die();
	}
	else
		Singleton<ParticleManager>::Instance()->AddObject(PARTICLE_LAYER_ABOVE_DYNAMIC, sPtr<ParticleSpawnerBase>(new ParticleSpawnerEnemyHitExplosion(shared_from_this(), false)));
}

void Game::EnemyBase::RenderHealth()
{
	healthDisplay->Render();
}

void Game::EnemyBase::Die()
{
	if (!isAlive)
		return;

	isAlive = false;
	Singleton<ParticleManager>::Instance()->AddObject(PARTICLE_LAYER_ABOVE_DYNAMIC, sPtr<ParticleSpawnerBase>(new ParticleSpawnerEnemyHitExplosion(shared_from_this(), true)));
	Singleton<LevelObjectsManager>::Instance()->RemoveObject(Game::OBJECTS_LAYER_FOREGROUND, shared_from_this());
}

bool Game::EnemyBase::GetTurningPoint()
{
	return atTurningPoint;
}

void Game::EnemyBase::SetTurningPoint(bool b)
{
	atTurningPoint = b;
}

Game::ENEMYDIRECTION Game::EnemyBase::GetDirection() const
{
	return currDir;
}

void Game::EnemyBase::SetDirection(Game::ENEMYDIRECTION dir)
{
	currDir = dir;
}

void Game::EnemyBase::OnCollision(Engine::PhysicsObject& thisObj, Engine::PhysicsObject& otherObj, Engine::Physics::PhysicsCollisionData& collisionData)
{	
	UNREFERENCED_PARAMETER(thisObj);

	// Change directions if going towards an enemy
	sPtr<EnemyBase> en = std::dynamic_pointer_cast<EnemyBase>(otherObj.shared_from_this());
	if (en != nullptr)
	{
		if (currDir == ENEMYDIRECTION::EDIR_LEFT)
		{
			currDir = EDIR_RIGHT;
		}
		else
		{
			currDir = EDIR_LEFT;
		}
	}

	// Bumps into a wall change directions
	sPtr<PhysBlock> b = std::dynamic_pointer_cast<PhysBlock>(otherObj.shared_from_this());
	if (b != nullptr)
	{
		Coordinate objCoord = Engine::Singleton<Map>::Instance()->WorldToMapPos(transform.GetPosition());
		Coordinate bCoord = Engine::Singleton<Map>::Instance()->WorldToMapPos(b->GetPosition());
		bCoord.y -= 1;
		objCoord.y -= 1;
		if (bCoord.y == objCoord.y)
		{
			if (currDir == ENEMYDIRECTION::EDIR_LEFT)
			{
				currDir = EDIR_RIGHT;
			}
			else
			{
				currDir = EDIR_LEFT;
			}
		}
	}

	if (!otherObj.GetIsDynamic()) // Check if dynamic
		return;
	if (Vec2LengthSquared(collisionData.relativeVelocity) < 50.0f * 50.0f) // Check if velocity is of required threshold to be damaged
		return;
	if (isInvincible) // Check if invincible
		return;

	isInvincible = true;
	invincibleCountdown = invincibilityTimer;
	sPtr<PhysBlock> pb = std::dynamic_pointer_cast<PhysBlock>(otherObj.shared_from_this());

	// Try to cast to enemy base
	sPtr<EnemyBase> eb = std::dynamic_pointer_cast<EnemyBase>(otherObj.shared_from_this());
	if (eb) { return; }

	if (pb != nullptr)
	{
		TakeDamage(50.0f);
	}
	else
	{
		TakeDamage(10.0f); // Grapplehook :p
	}
}
