/******************************************************************************/
/*!
\file   G_Projectile.cpp
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Marc Alviz Evangelista (60%)
\par    email: marcalviz.e\@digipen.edu
\par    DigiPen login: marcalviz.e

\author Ryan Chan Kuan Fu (40%)
\par    email: c.kuanfuryan\@digipen.edu
\par    DigiPen login: c.kuanfuryan

\brief
  This is the implementation file for all member functions of class Projectile.
  Including Utility functions.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#include "pch.h"
#include "G_Projectile.h"
#include "E_Collision.h"
#include "E_Singleton.h"
#include "G_Player.h"
#include "G_LevelObjects.h"

namespace Game {
	Projectile::Projectile()
		: PhysicsObject()
		, lifeTime(5.0f)
		, vel(Vec2Zero())
		, isActive(true)
		, projectileSpeed(C_projectileSpeed)
		, isExplosion(false)
		, explosionRad(0.0f)
	{
	}

	Projectile::Projectile(f32 lifetime, Transform transform, const Vec2& velocity)
		: PhysicsObject(true, true, transform, Engine::SpriteBuilder::CreateSpriteSquare(), 1.0f, Color())
		, lifeTime(lifetime)
		, vel(velocity)
		, isActive(true)
		, projectileSpeed(C_projectileSpeed)
		, isExplosion(false)
		, explosionRad(0.0f)
	{
	}

	Projectile::Projectile(f32 lifeTime, Transform transform, const Vec2& velocity, sPtr<Sprite> sprite)
		: PhysicsObject(true, true, transform, sprite, 1.0f, Color())
		, lifeTime(lifeTime)
		, vel(velocity)
		, isActive(true)
		, projectileSpeed(C_projectileSpeed)
		, isExplosion(false)
		, explosionRad(0.0f)
	{
	}

	Projectile::Projectile(f32 lifeTime, Transform transform, const Vec2& velocity, sPtr<Sprite> sprite, bool sysRegister)
		: PhysicsObject(sysRegister, false, transform, sprite, 1.0f, Color())
		, lifeTime(lifeTime)
		, vel(velocity)
		, isActive(true)
		, projectileSpeed(C_projectileSpeed)
		, isExplosion(false)
		, explosionRad(0.0f)
	{
	}

	Projectile::~Projectile()
	{
	}

	void Projectile::Init()
	{
		SetScale(MakeVec2(40.0f, 15.0f));
		SetVelocity(vel * projectileSpeed);
		SetRotation(sCast<f32>(AERadToDeg(atan2f(vel.y, vel.x))));
		SetGravityEnabled(false);
		SetOnCollisionCallback(std::bind(&Projectile::OnCollision, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	}

	void Projectile::Update()
	{
		spritePtr->Update(Engine::C_frametime);
		if (isActive)
		{
			if (isExplosion)
			{
				// Check
				Vec2 DistanceToPlayer = Singleton<Player>::Instance()->GetPosition() - transform.GetPosition();
				f32 length = Vec2Length(DistanceToPlayer);
				if (length < explosionRad)
				{
					Singleton<Player>::Instance()->TakeDamage(40.0f);
				}
				s32 rad = 0;
				for (f32 f = 0.0f; f < explosionRad/C_blockLength; f += 1.0f)
					++rad;

				sPtr<Map> map = Engine::Singleton<Map>::Instance();
				Coordinate pos = map->WorldToMapPos(this->GetPosition());

				for (s32 x = pos.x - rad > 0 ? pos.x - rad : 0; x < pos.x + rad && x < map->width - 1; ++x)
				{
					for (s32 y = pos.y - rad > 0 ? pos.y - rad : 0; y < pos.y + rad && y < map->height - 1; ++y)
					{
						Block* b = map->GetBlockAtIndex(x, y);
						if (b != nullptr)
						{
							switch (b->type)
							{
							case BT_ROCK:
							{
								sPtr<PhysBlock> p = b->object;
								if (p != nullptr)
								{
									Block* pb = nullptr;
									pb = map->GetBlockAtIndex(p->GetPosition());
									switch (pb->type)
									{
									case BT_ROCK:
									{
										if (Vec2DistanceSquared(map->MapToWorldPos(pb->x, pb->y), this->GetPosition()) < explosionRad * explosionRad)
										{
											map->UnsetBlock(pb);
											p->SetIsDynamic(true);
											Engine::Singleton<LevelObjectsManager>::Instance()->AddObject(OBJECTS_LAYER_FOREGROUND, p);

											p->SetVelocity((map->MapToWorldPos(pb->x, pb->y)- this->GetPosition())*-10.0f);
										}}
									break;
									}
								}

							}
							break;
							}
						}
					}
				}


			}
			lifeTime -= C_frametime;
			if (lifeTime < 0.0f)
			{
				isActive = false;
			}
		}
	}

	bool Projectile::GetActive()
	{
		return isActive;
	}

	void Projectile::SetExplosion(bool b)
	{
		isExplosion = b;
	}

	void Projectile::SetExRadius(f32 scale)
	{
		explosionRad = scale / 2;
		SetScale(MakeVec2(scale, scale));
	}

	void Projectile::OnCollision(Engine::PhysicsObject& thisObj, Engine::PhysicsObject& otherObj, Engine::Physics::PhysicsCollisionData& collisionData)
	{
		UNREFERENCED_PARAMETER(thisObj);
		UNREFERENCED_PARAMETER(collisionData);
		bool playerCollide = false;
		sPtr<Player> player = std::dynamic_pointer_cast<Player>(otherObj.shared_from_this());
		if (player != nullptr)
		{
			playerCollide = true;
		}

		if (!isExplosion)
		{
			isActive = false;
		}
		if (playerCollide)
		{
			Singleton<Player>::Instance()->TakeDamage(40.0f);
		}
		else
		{
			//Matt's projectile dislocation
			sPtr<PhysBlock> p = std::dynamic_pointer_cast<PhysBlock>(otherObj.shared_from_this());
			if (p != nullptr)
			{
				if (p->GetIsDynamic())
				{
						p->SetVelocity(p->GetVelocity() +thisObj.GetVelocity());

				}
				else
				{
					sPtr<Map> map = Engine::Singleton<Map>::Instance();
					Block* b = nullptr;
					switch (p->type)
					{
					case BT_ROCK:
					{
						b = map->GetBlockAtIndex(p->GetPosition());
						//b->FillBlock(BT_NONE);
						map->UnsetBlock(b);
						p->SetIsDynamic(true);
						Engine::Singleton<LevelObjectsManager>::Instance()->AddObject(OBJECTS_LAYER_FOREGROUND, p);

						p->SetVelocity(-thisObj.GetVelocity());
					}
					break;
					}
				}
			}
		}
		// Spawn Particles
	}

	void ProjectileManager::AddProjectile(sPtr<Projectile> projectile)
	{
		projectile->Init();
		projectile->UnsetLayerMask({ Engine::Physics::MASK_ALL });
		projectile->SetLayerMask({ Engine::Physics::MASK_PLAYER,Engine::Physics::MASK_ENVIRONMENT });
		projectileList.push_back(projectile);
	}

	void ProjectileManager::UpdateList()
	{
		for (std::vector<sPtr<Projectile>>::iterator startIte = projectileList.begin(); startIte != projectileList.end();)
		{
			(*startIte)->Update();
			if (!(*startIte)->GetActive())
			{
				startIte = projectileList.erase(startIte);
				continue;
			}
			else
			{
				++startIte;
			}
		}
	}

	void ProjectileManager::RenderList()
	{
		for (std::vector<sPtr<Projectile>>::iterator startIte = projectileList.begin(), endIte = projectileList.end(); startIte != endIte; ++startIte)
		{
			if ((*startIte)->GetActive())
			{
				(*startIte)->Render();
			}
		}
	}

	sPtr<Projectile> ProjectileBuilder::CreateProjectile(const Transform& transform, const Vec2& velocity, f32 lifeTime, sPtr<Sprite> sprite)
	{
		return sPtr<Projectile>(new Projectile(
			lifeTime,
			transform,
			velocity,
			sprite
		));
	}
	sPtr<Projectile> ProjectileBuilder::CreateExplosion(const Transform& transform, const Vec2& velocity, f32 lifeTime, sPtr<Sprite> sprite, bool regToSys)
	{
		return sPtr<Projectile>(new Projectile(
			lifeTime,
			transform,
			velocity,
			sprite,
			regToSys
		));
	}
}