/******************************************************************************/
/*!
\file   G_PredictorProjectile.cpp
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Ryan Chan Kuan Fu (100%)
\par    email: c.kuanfuryan\@digipen.edu
\par    DigiPen login: c.kuanfuryan

\brief
  This bad boy can render any differentiable equation. Sine wave? Spiral? No
  problem. Just make sure you know your math! Used by EnemyBoss.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#include "pch.h"
#include "G_PredictorProjectile.h"
#include "E_Sprite.h"
#include "E_IOManager.h"
#include "G_Physics.h"

namespace Game
{
    Vec2 PredictorProjectile::PositionAtTimestep(f32 timestep)
    {
        switch (pType)
        {
        case PT_SPIRAL:
        {
            return MakeVec2(C_predictorProjectileSpiralR * cos(timestep) * timestep, 
                C_predictorProjectileSpiralR * sin(timestep) * timestep);
        }
        case PT_SINE:
        {
            return MakeVec2(timestep * C_predictorProjectileSineWidth,
                C_predictorProjectileSineHeight * sCast<f32>(sinf(timestep)));
        }
        case PT_SQUARED:
        {
            return MakeVec2(
                timestep * C_predictorProjectileSquareScale,
                timestep * timestep * C_predictorProjectileSquareScale * 0.25f);
        }
        default: { break; }
        }
        return MakeVec2(0.0f, 0.0f);
    }

    f32 PredictorProjectile::AngleAtTimestep(f32 timestep)
    {
        switch (pType)
        {
        case PT_SPIRAL:
        {
            // Method 1: Differentiation
            f32 dY = sin(timestep) + timestep * cos(timestep);
            f32 dX = cos(timestep) - timestep * sin(timestep);
            return AERadToDeg(sCast<f32>(atan(dY / dX)));
            
            // Method 2: Perpendicular from origin
            /*
            Vec2 v = PositionAtTimestep(timestep);
            Vec2 p{ -v.y, v.x };
            return AERadToDeg(sCast<f32>(atan(p.y / p.x)));
            */
        }
        case PT_SINE:
        {
            f32 g = C_predictorProjectileSineHeight / C_predictorProjectileSineWidth;
            return AERadToDeg(sCast<f32>(atan(cosf(timestep) * g)));
        }
        case PT_SQUARED:
        {
            return AERadToDeg(sCast<f32>(atan(0.5f * timestep)));
        }
        default: { break; }
        }
        return 0.0f;
    }

    void PredictorProjectile::ApplyRotationAboutStartPoint(sPtr<Engine::Object> obj)
    {
        // Get the data from object
        Vec2 currPos = obj->GetPosition();

        // Mtx33 declarations
        Mtx33 translate1{};
        Mtx33 rotate{};
        Mtx33 translate2{};

        // Translate to origin, rotate about attached object and translate back
        AEMtx33Trans(&translate1, -startPos.x, -startPos.y);
        AEMtx33RotDeg(&rotate, pRot);
        AEMtx33Trans(&translate2, startPos.x, startPos.y);

        // Concatenate matrices
        AEMtx33Concat(&translate1, &rotate, &translate1);
        AEMtx33Concat(&translate1, &translate2, &translate1);

        // Set new position by multiplying vector with matrix
        Vec2 newPos{ 0.0f, 0.0f };
        AEMtx33MultVec(&newPos, &translate1, &currPos);

        // Setting values
        obj->SetPosition(newPos);
        obj->SetRotation(obj->GetRotation() + pRot);
    }

    PredictorProjectile::PredictorProjectile() :
        units       {}, 
        active      { true }, 
        shoot       { false }, 
        currentID   { 0 }, 
        pTime       { 0.0f }, 
        startPos    {}, 
        pType       { PT_SPIRAL },
        pRot        { 0.0f },
        positive    { true }
    {
    }

    PredictorProjectile::~PredictorProjectile()
    {
    }

    void PredictorProjectile::Init(PREDICTOR_TYPE type, Vec2 pos, f32 rot, bool isPositive)
    {
        pType = type;
        startPos = pos;
        pRot = rot;
        positive = isPositive;

        // Instantiate predictor lines
        for (s32 i = 0; i < C_predictorProjectileLength; ++i)
        {
            f32 t = sCast<f32>(i) * C_predictorProjectileInterval;

            t = isPositive ? t : -t;

            Vec2 p1 = PositionAtTimestep(t);
            Vec2 p2 = PositionAtTimestep(t + C_predictorProjectileInterval);
            sPtr<AP_UNIT> unit = std::make_shared<AP_UNIT>();
            unit->obj = Engine::ObjectBuilder::CreateGenericSquare(
                Engine::Graphics::Transform(0.0f, 0.0f, 0.0f, Vec2Length(p2 - p1) + C_laserWidth, C_laserWidth),
                Engine::SpriteBuilder::CreateSpriteSquare(),
                0.0f
            );
            units.push_back(unit);

            // Set transform and color
            units[i]->obj->SetPosition(startPos + p1);
            units[i]->obj->SetRotation(AngleAtTimestep(t));
            units[i]->obj->SetColorTint(Engine::MakeColor(255, 0, 0, 255));
            ApplyRotationAboutStartPoint(units[i]->obj);

            t = isPositive ? t : -t;
            units[i]->timeInitial = t;
            units[i]->time = t;
        }
    }

    void PredictorProjectile::Update()
    {
        if (!active) { return; }

        // If haven't shoot projectile, iterate laser line
        if (!shoot)
        {
            f32 transparency = units[currentID]->obj->GetTransparency();
            if (transparency < 0.1f)
            {
                for (int i = 0; i <= currentID; ++i)
                {
                    units[i]->obj->SetTransparency(min(units[i]->obj->GetTransparency() + C_predictorProjectileIterationSpeed * Engine::C_frametime, 1.0f));
                }
            }
            else
            {
                ++currentID;
            }
        }

        // Once the currentID of laser line reaches the last, shoot
        if (currentID == C_predictorProjectileLength && !shoot)
        {
            // Shoot projectile here
            shoot = true;

            // Projectiles instatiated under predictorProjectile DO NOT use the regular projectile manager.
            projectile = std::make_shared<Projectile>();
            projectile->Init();
            Singleton<PhysicsManager>::Instance()->RegisterSpecialStatic(projectile->physicsComp);
            projectile->UnsetLayerMask({ Engine::Physics::MASK_ALL });
            projectile->SetLayerMask({ Engine::Physics::MASK_PROJECTILE, Engine::Physics::MASK_PLAYER });
        }

        // Once the projectile is instantiated, set the position and rotation every frame
        if (shoot)
        {
            pTime += C_predictorProjectileSpeed * C_frametime;

            f32 t = positive ? pTime : -pTime;
            projectile->SetPosition(startPos + PositionAtTimestep(t));
            projectile->SetRotation(AngleAtTimestep(t));
            ApplyRotationAboutStartPoint(projectile);

            // Once the timestep exceeds the last line segment, reset the pointer
            if (pTime >= units[C_predictorProjectileLength - 1]->time)
            {
                projectile->SetTransparency(0.0f);
                projectile.reset();
                active = false;
            }
        }
    }

    void PredictorProjectile::Render()
    {
        if (!active) { return; }

        for (int i = 0; i < C_predictorProjectileLength; ++i)
        {
            units[i]->obj->Render();
        }

        if (shoot) { projectile->Render(); }
    }

    void PredictorProjectile::Exit()
    {
        for (int i = 0; i < C_predictorProjectileLength; ++i)
        {
            units[i]->obj->SetTransparency(0.0f);
            units[i]->obj.reset();
        }
    }

    bool PredictorProjectile::isActive()
    {
        return active;
    }

    void PredictorProjectile::setActive(bool b)
    {
        active = b;
    }
}