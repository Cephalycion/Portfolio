/******************************************************************************/
/*!
\file   G_ParticleSystem.h
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Kendrick Sim Hean Guan (100%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\brief
  This is an interface file for all particles spawned within the game.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#pragma once
#include "pch.h"
#include "E_ParticleSystem.h"

namespace Game {

	using namespace Engine;

	// A simple particle with support for texture
	class ParticleSimpleTex : public ParticleSimple
	{
	protected:
		// Hold a reference to the previous sprite object so that we can reset the object to its previous state before
		// returning the object to the pool.
		sPtr<Sprite> prevSprite;

	public:
		ParticleSimpleTex(const ParticleSpawnerBase* spawnerOwner, const Vec2& velocity, const Vec2& acceleration = Vec2Zero(),
			const Color& colorTint = C_colorWhite, const sPtr<Texture>& texture = nullptr, const sPtr<Sprite>& sprite = nullptr);
		virtual ~ParticleSimpleTex();

	};

	// A physics particle with support for texture
	class ParticlePhysTex : public ParticlePhysics
	{
	protected:
		// Hold a reference to the previous sprite object so that we can reset the object to its previous state before
		// returning the object to the pool.
		sPtr<Sprite> prevSprite;

		ParticlePhysTex(const ParticleSpawnerBase* spawnerOwner, const Vec2& velocity, const sPtr<Texture>& texture, const sPtr<Sprite>& sprite = nullptr);
		virtual ~ParticlePhysTex();

	public:

	};

#pragma region Skeleton Classes

	class ParticleRandGen
	{
	protected:
		std::default_random_engine randGenerator;
		std::uniform_real_distribution<f32> lifetimeRandEngine;
		std::uniform_real_distribution<f32> speedRandEngine;
		std::uniform_real_distribution<f32> angleRandEngine;
		
		// Flag for whether there's angle randomization, for optimizing the rotation matrix mult out if possible.
		bool hasAngleVariation;
		Mtx33 rotate;

	public:
		ParticleRandGen(const Vec2& lifetimeMultRange = MakeVec2(1.0f, 1.0f), const Vec2& speedRange = MakeVec2(10.0f, 100.0f), float angleMaxVariation = 0.0f);

		float RandomizeLifetimeMult();
		void RandomizeSpeed(Vec2* direction);
		void VariateAngle(Vec2* velocity);
		Vec2 MakeRandomDir(float baseAngle);

		Vec2 GenerateVelocity(float baseAngle);
		Vec2 GenerateVelocity(const Vec2& baseDirection);
	};

#pragma endregion // Skeleton Classes

#pragma region BlockHit

	class ParticleBlockHit : public ParticlePhysTex
	{
	protected:
		sPtr<Texture> blockTexture;

	public:
		// For blocks without texture
		ParticleBlockHit(const ParticleSpawnerBase* spawnerOwner, const Color& color, const Vec2& velocity, float lifespanMult);
		// For blocks with texture
		ParticleBlockHit(const ParticleSpawnerBase* spawnerOwner, const sPtr<TexOffsetSprite>& sprite, const Vec2& velocity, float lifespanMult);

	};

	class ParticleSpawnerBlockHit : public ParticleSpawnerBase
	{
	protected:
		// Ptr to the sprite to be used by block hit particles. Has a shorter UV length to only render part of the texture
		sPtr<TexOffsetSprite> spritePtr;

		std::default_random_engine randGenerator;
		std::cauchy_distribution<f32> spreadRandEngine;
		std::uniform_real_distribution<f32> speedRandEngine;
		std::uniform_real_distribution<f32> lifetimeRandEngine;
		sPtr<Object> blockObject;

		// The base velocity of a particle spawned by this block hit.
		// The final velocity will be jiggled about this vec.
		Vec2 rawVelocity;

		virtual void CreateParticle(float simulateTime) override;

	public:
		ParticleSpawnerBlockHit(const Vec2& collisionPoint, const Vec2& collisionNormal, const Vec2& relativeVelocity, sPtr<Object> blockObject);

	};

#pragma endregion // BlockHit

#pragma region Smoke

	class ParticleSmoke : public ParticleSimple
	{
	protected:

	public:
		ParticleSmoke(const ParticleSpawnerBase* spawnerOwner, const Vec2& velocity, float lifespanMult);

	};

	class ParticleSpawnerSmoke : public ParticleSpawnerBase
	{
	protected:
		ParticleRandGen randGen;

		virtual void CreateParticle(float simulateTime) override;

	public:
		ParticleSpawnerSmoke(const Vec2& position);

	};

#pragma endregion // Smoke

#pragma region Explosion

	class ParticleEnemyHitExplosion : public ParticlePhysTex
	{
	protected:
		void Init(const Vec2& position, float lifespanMult);

	public:
		ParticleEnemyHitExplosion(const ParticleSpawnerBase* spawnerOwner, const sPtr<Sprite>& sprite, const sPtr<Texture>& texture, const Vec2& velocity, const Vec2& position, float lifespanMult);
		ParticleEnemyHitExplosion(const ParticleSpawnerBase* spawnerOwner, const Color& color, const Vec2& velocity, const Vec2& position, float lifespanMult);
		~ParticleEnemyHitExplosion();

		virtual void Render() override;

	};

	class ParticleSpawnerEnemyHitExplosion : public ParticleSpawnerBase
	{
	protected:
		ParticleRandGen randGen;
		wPtr<Object> objWPtr;

		virtual void CreateParticle(float simulateTime) override;

	public:
		ParticleSpawnerEnemyHitExplosion(const sPtr<Object>& obj, bool isDeath);

	};

#pragma endregion // Death Explosion

#pragma region Death Shine

	class ParticleDeathShine : public ParticleSimpleTex
	{
	protected:

	public:
		ParticleDeathShine(const ParticleSpawnerBase* spawnerOwner, const sPtr<Sprite>& sprite, const Vec2& pos, float angle);

	};

	class ParticleSpawnerDeathShine : public ParticleSpawnerBase
	{
	protected:
		// Sprite rendered for each particle
		sPtr<TexOffsetSprite> particleSprite;
		// Time alive, so that we can update the uv offset of the sprite
		float timeElapsed;

		virtual void CreateParticle(float simulateTime) override;

	public:
		ParticleSpawnerDeathShine(const Vec2& position);

		virtual void Update(float dt) override;

	};

#pragma endregion // Death Shine

#pragma region // Boss Die Explosion

	// Uses ParticleDeathExplosion for the particle class

	class ParticleSpawnerBossDieExplosion : public ParticleSpawnerBase
	{
	protected:
		ParticleRandGen randGen;

		wPtr<Object> bossWPtr;

		virtual void CreateParticle(float simulateTime) override;

	public:
		ParticleSpawnerBossDieExplosion(const sPtr<Object>& boss);

	};

#pragma endregion // Boss Die Explosion

}
