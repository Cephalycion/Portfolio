/******************************************************************************/
/*!
\file   G_EnemyPatroller.cpp
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Marc Alviz Evangelista (100%)
\par    email: marcalviz.e\@digipen.edu
\par    DigiPen login: marcalviz.e

\brief
  This is the implemenation file for all member functions of classes called
  EnemyPatroller and PatrollerBuilder. Along with utility functions for EnemyPatroller.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#include "pch.h"
#include "G_EnemyPatroller.h"
#include "G_GameState.h"
#include "E_PatrollerStateMachine.h"
#include "E_Singleton.h"
#include "G_Player.h"
#include "G_Projectile.h"
#include "E_IOManager.h"
#include "G_LevelObjects.h"
#include "G_PatrollerAnimTree.h"

namespace Game {
    EnemyPatroller::EnemyPatroller(const Transform& transform, const sPtr<Sprite>& spritePtr)
        : EnemyBase(true, transform, spritePtr, 500.0f, 0.0f)
        , explosionRadius(150.0f)
        , idlePatrolSwitchTimer(5.0f)
        , initAnim(false)
        , isExploding(false)
        , isMoving(false)
    {
    }

    EnemyPatroller::~EnemyPatroller()
    {
    }
    void EnemyPatroller::Init()
    {
        SetLockRotation(true);
        SetRestitutionCoeff(0.0f);
        fsm.InsertState(new PatrollerIdleState(this, &fsm), PATROLLERSTATES::P_IDLE);
        fsm.InsertState(new PatrollerPatrolState(this, &fsm), PATROLLERSTATES::P_PATROL);
        fsm.InsertState(new PatrollerAggroState(this, &fsm), PATROLLERSTATES::P_AGGRO);
        fsm.InsertState(new PatrollerExplodeState(this, &fsm), PATROLLERSTATES::P_EXPLODING);
        fsm.Init(fsm.GetState(PATROLLERSTATES::P_IDLE));
        ln = std::make_shared<Engine::LineRenderer>();
        ln->Init();
        InitAnimationTree();
        EnemyBase::Init();
    }
    void EnemyPatroller::Update()
    {
        fsm.Update();
        animFsm.Update();
        spritePtr->Update(C_frametime);

        if (isInvincible)
        {
            invincibleCountdown -= C_frametime;
            if (invincibleCountdown <= 0.0f)
            {
                isInvincible = false;
            }
        }
        EnemyBase::Update();
    }
    void EnemyPatroller::Render()
    {
        // Render based on direction
        switch (currDir)
        {
        case ENEMYDIRECTION::EDIR_LEFT:
        {

            break;
        }
        case ENEMYDIRECTION::EDIR_RIGHT:
        {
            break;
        }
        default:
            break;
        }

        switch (fsm.GetStateIndex())
        {
        case PATROLLERSTATES::P_IDLE:
            spritePtr->Render(transform, transparency, colorTint);
            break;
        case PATROLLERSTATES::P_PATROL:
            spritePtr->Render(transform, transparency, colorTint);
            break;
        case PATROLLERSTATES::P_AGGRO:
            spritePtr->Render(transform, transparency, colorTint);
            break;
        case PATROLLERSTATES::P_EXPLODING:
            spritePtr->Render(transform, transparency, colorTint);
            break;
        default:
            break;
        }
        EnemyBase::RenderHealth();
    }
    void EnemyPatroller::DoAction(PATROLLERSTATES state)
    {
        Engine::Physics::PhysicsRaycastData rcd;
        if (Singleton<GameStateManager>::Instance()->GetIsLevelClear())
        {
            Die();
            return;
        }

        // Vector of player -> enemy
        Vec2 EnemytoPlayer = Singleton<Player>::Instance()->GetPosition() - transform.GetPosition();

        switch (state)
        {
        case PATROLLERSTATES::P_IDLE:
        {
            // Check for player
            if (Engine::Physics::PhysicsManager::Raycast(transform.GetPosition(), Vec2Normalize(EnemytoPlayer), { Engine::Physics::MASK_PLAYER }, &rcd))
            {
                // ln->SetLine(transform.GetPosition(), rcd.collisionPoint, 5.0f);
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
        case PATROLLERSTATES::P_PATROL:
        {
            if (Engine::Physics::PhysicsManager::Raycast(transform.GetPosition(), Vec2Normalize(EnemytoPlayer), { Engine::Physics::MASK_PLAYER }, &rcd))
            {
                // Check for player
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
        case PATROLLERSTATES::P_AGGRO:
        {
            // if lose player go back to idle
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

            // Change directions based on player
            if (EnemytoPlayer.x > 0.0f)
            {
                currDir = Game::ENEMYDIRECTION::EDIR_RIGHT;
            }
            else
            {
                currDir = Game::ENEMYDIRECTION::EDIR_LEFT;
            }
            break;
        }
        case PATROLLERSTATES::P_EXPLODING:
        {
            // Create explosion sprite
            sPtr<Projectile> p = ProjectileBuilder::CreateExplosion(
                transform, Vec2Zero(), C_enemyPatrolExplosionTime,
                Engine::SpriteBuilder::CreateSpriteSquare(Engine::IOManager::GetFilePath(Engine::PNG_PARTICLE_EXPLOSION_CREEPER), true, 1, 7, C_enemyPatrolExplosionTime, 7));
            Singleton<ProjectileManager>::Instance()->AddProjectile(p);
            p->SetExplosion(true);
            p->SetExRadius(explosionRadius * 2.5f);
            p->UnsetLayerMask({ Engine::Physics::MASK_ALL });
            p->SetLayerMask({ Engine::Physics::MASK_PLAYER });
         

            // nuke itself
            TakeDamage(hitPoints);
            break;
        }
        default:
            break;
        }
    }


    f32 EnemyPatroller::GetExRadius() const
    {
        return explosionRadius;
    }

    void EnemyPatroller::ChangeAnimation(PATROLLER_SPRITE newSprite)
    {
        SetSprite(patrollerAnims[newSprite]);
    }

    void EnemyPatroller::InitAnimationTree()
    {
        if (initAnim)
        {
            return;
        }
        initAnim = true;

        patrollerAnims.emplace(PATROLLER_IDLE_LEFT,
            SpriteBuilder::CreateSpriteSquare("Textures/Patroller/PatrollerIdleLeft.png", true, 2, 2, 1.0f, 4));
        patrollerAnims.emplace(PATROLLER_IDLE_RIGHT,
            SpriteBuilder::CreateSpriteSquare("Textures/Patroller/PatrollerIdleRight.png", true, 2, 2, 1.0f, 4));

        patrollerAnims.emplace(PATROLLER_MOVE_LEFT,
            SpriteBuilder::CreateSpriteSquare("Textures/Patroller/PatrollerMoveLeft.png", true, 2, 2, 1.0f, 3));
        patrollerAnims.emplace(PATROLLER_MOVE_RIGHT,
            SpriteBuilder::CreateSpriteSquare("Textures/Patroller/PatrollerMoveRight.png", true, 2, 2, 1.0f, 3));

        patrollerAnims.emplace(PATROLLER_AIR_LEFT,
            SpriteBuilder::CreateSpriteSquare("Textures/Patroller/PatrollerMidairLeft.png", true, 2, 2, 1.0f, 3));
        patrollerAnims.emplace(PATROLLER_AIR_RIGHT,
            SpriteBuilder::CreateSpriteSquare("Textures/Patroller/PatrollerMidairRight.png", true, 2, 2, 1.0f, 3));

        patrollerAnims.emplace(PATROLLER_EXPLODE_LEFT,
            SpriteBuilder::CreateSpriteSquare("Textures/Patroller/PatrollerExplodeLeft.png", true, 3, 4, C_patrollerFuseTimer + 0.2f, 9));
        patrollerAnims.emplace(PATROLLER_EXPLODE_RIGHT,
            SpriteBuilder::CreateSpriteSquare("Textures/Patroller/PatrollerExplodeRight.png", true, 3, 4, C_patrollerFuseTimer + 0.2f, 9));

        animFsm.InsertState(new PatrollerIdleLeft(this, &animFsm), PATROLLER_IDLE_LEFT);
        animFsm.InsertState(new PatrollerIdleRight(this, &animFsm), PATROLLER_IDLE_RIGHT);

        animFsm.InsertState(new PatrollerMoveLeft(this, &animFsm), PATROLLER_MOVE_LEFT);
        animFsm.InsertState(new PatrollerMoveRight(this, &animFsm), PATROLLER_MOVE_RIGHT);

        animFsm.InsertState(new PatrollerAirLeft(this, &animFsm), PATROLLER_AIR_LEFT);
        animFsm.InsertState(new PatrollerAirRight(this, &animFsm), PATROLLER_AIR_RIGHT);

        animFsm.InsertState(new PatrollerExplodeLeft(this, &animFsm), PATROLLER_EXPLODE_LEFT);
        animFsm.InsertState(new PatrollerExplodeRight(this, &animFsm), PATROLLER_EXPLODE_RIGHT);
        animFsm.Init(animFsm.GetState(PATROLLER_IDLE_LEFT));
    }

    bool EnemyPatroller::GetIsMoving() const
    {
        return isMoving;
    }

    void EnemyPatroller::SetIsMoving(bool b)
    {
        isMoving = b;
    }

    bool EnemyPatroller::GetIsExploding() const
    {
        return isExploding;
    }

    void EnemyPatroller::SetIsExploding(bool b)
    {
        isExploding = b;
    }

    sPtr<EnemyPatroller> PatrollerBuilder::CreatePatroller(const Transform& transform, const sPtr<Sprite> spritePtr)
    {
        return sPtr<EnemyPatroller>(new EnemyPatroller(
            transform,
            spritePtr
        ));
    }
}
