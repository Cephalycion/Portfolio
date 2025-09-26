/******************************************************************************/
/*!
\file   G_Projectile.h
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
  This is the interface file for all member functions of class Projectile.
  Including Utility functions.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#pragma once
#include "pch.h"
#include "E_Object.h"

namespace Game
{
	using namespace Engine;

	class Projectile : public PhysicsObject
	{
	private:
		f32 lifeTime;
		f32 projectileSpeed;
		Vec2 vel;
		bool isActive;
		bool isExplosion;
		f32 explosionRad;
	public:
		Projectile();
		Projectile(f32 lifeTime, Transform transform, const Vec2& velocity);
		Projectile(f32 lifeTime, Transform transform, const Vec2& velocity, sPtr<Sprite> sprite);
		Projectile(f32 lifeTime, Transform transform, const Vec2& velocity, sPtr<Sprite> sprite, bool sysRegister);
		~Projectile();
		void Init();
		virtual void Update() override;
		bool GetActive();
		void SetExplosion(bool b);
		void SetExRadius(f32 scale);

		void OnCollision(Engine::PhysicsObject& thisObj, Engine::PhysicsObject& otherObj, Engine::Physics::PhysicsCollisionData& collisionData);
	};

	class ProjectileManager
	{
	private:
		std::vector<sPtr<Projectile>> projectileList;
	public:
		void AddProjectile(sPtr<Projectile> projectile);
		void UpdateList();
		void RenderList();

	};

	class ProjectileBuilder
	{
	private:
		ProjectileBuilder() = delete;
	public:
		static sPtr<Projectile> CreateProjectile(const Transform& transform = Transform(0.0f, 0.0f, 0.0f, 100.0f, 100.0f),
			const Vec2& velocity = Vec2Zero(), f32 lifeTime = 5.0f, sPtr<Sprite> sprite = Engine::SpriteBuilder::CreateSpriteSquare());

		static sPtr<Projectile> CreateExplosion(const Transform& transform = Transform(0.0f, 0.0f, 0.0f, 100.0f, 100.0f),
			const Vec2& velocity = Vec2Zero(), f32 lifeTime = 5.0f, sPtr<Sprite> sprite = Engine::SpriteBuilder::CreateSpriteSquare(),
			bool regToSys = false);
	};
}
