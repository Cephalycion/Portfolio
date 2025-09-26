/******************************************************************************/
/*!
\file   G_ParticleSystem.cpp
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Kendrick Sim Hean Guan (100%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\brief
  This is the implementation file for all particles spawned within the game.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#include "pch.h"
#include "G_Constants.h"
#include "G_ParticleSystem.h"

namespace Game {

	using namespace Engine;

	ParticleSimpleTex::ParticleSimpleTex(const ParticleSpawnerBase* spawnerOwner, const Vec2& velocity, const Vec2& acceleration,
		const Color& color, const sPtr<Texture>& texture, const sPtr<Sprite>& sprite)
		: ParticleSimple(spawnerOwner, velocity, acceleration, color)
	{
		if (sprite)
		{
			prevSprite = obj->GetSprite();
			obj->SetSprite(sprite);
		}
		if (texture)
			obj->SetTexture(texture);
	}
	ParticleSimpleTex::~ParticleSimpleTex()
	{
		if (prevSprite)
			obj->SetSprite(prevSprite);
		obj->SetTexture(nullptr);
	}

	ParticlePhysTex::ParticlePhysTex(const ParticleSpawnerBase* spawnerOwner, const Vec2& velocity, const sPtr<Texture>& texture, const sPtr<Sprite>& sprite)
		: ParticlePhysics(spawnerOwner, velocity)
	{
		if (sprite)
		{
			prevSprite = obj->GetSprite();
			obj->SetSprite(sprite);
		}
		if (texture)
			obj->SetTexture(texture);
	}
	ParticlePhysTex::~ParticlePhysTex()
	{
		if (prevSprite)
			obj->SetSprite(prevSprite);
		obj->SetTexture(nullptr);
	}

#pragma region Skeleton Classes

	ParticleRandGen::ParticleRandGen(const Vec2& lifetimeMultRange, const Vec2& speedRange, float angleMaxVariation)
		: randGenerator(std::random_device()())
		, lifetimeRandEngine(lifetimeMultRange.x, lifetimeMultRange.y)
		, speedRandEngine(speedRange.x, speedRange.y)
		, angleRandEngine(-AEDegToRad(angleMaxVariation), AEDegToRad(angleMaxVariation))
		, hasAngleVariation(angleMaxVariation == 0.0f)
		, rotate({})
	{
	}

	float ParticleRandGen::RandomizeLifetimeMult()
	{
		return lifetimeRandEngine(randGenerator);
	}
	void ParticleRandGen::RandomizeSpeed(Vec2* direction)
	{
		*direction *= speedRandEngine(randGenerator);
	}
	void ParticleRandGen::VariateAngle(Vec2* velocity)
	{
		if (!hasAngleVariation)
			return;
		AEMtx33Rot(&rotate, angleRandEngine(randGenerator));
		*velocity = rotate * *velocity;
	}

	Vec2 ParticleRandGen::MakeRandomDir(float baseAngle)
	{
		baseAngle = AEDegToRad(baseAngle) + angleRandEngine(randGenerator);
		return MakeVec2(cosf(baseAngle), sinf(baseAngle));
	}

	Vec2 ParticleRandGen::GenerateVelocity(float baseAngle)
	{
		return MakeRandomDir(baseAngle) * speedRandEngine(randGenerator);
	}
	Vec2 ParticleRandGen::GenerateVelocity(const Vec2& baseDirection)
	{
		Vec2 velocity = baseDirection * speedRandEngine(randGenerator);
		if (!hasAngleVariation)
			return velocity;
		AEMtx33RotDeg(&rotate, angleRandEngine(randGenerator));
		return rotate * velocity;
	}

#pragma endregion // Skeleton Classes


#pragma region BlockHit

	ParticleBlockHit::ParticleBlockHit(const ParticleSpawnerBase* spawnerOwner, const Color& color, const Vec2& velocity, float lifespanMult)
		: ParticlePhysTex(spawnerOwner, velocity, nullptr)
	{
		lifespan *= lifespanMult;
		startFadeThreshold = C_psBlockHit_fadeThreshold;
		obj->SetScale(MakeVec2(10.0f, 10.0f));
		obj->SetColorTint(color);
	}
	ParticleBlockHit::ParticleBlockHit(const ParticleSpawnerBase* spawnerOwner, const sPtr<TexOffsetSprite>& sprite, const Vec2& velocity, float lifespanMult)
		: ParticlePhysTex(spawnerOwner, velocity, nullptr, sPtr<Sprite>(new TexOffsetSprite(*sprite)))
	{
		lifespan *= lifespanMult;
		startFadeThreshold = C_psBlockHit_fadeThreshold;
		obj->SetScale(MakeVec2(10.0f, 10.0f));

		sPtr<TexOffsetSprite> texOffsetSprite = std::dynamic_pointer_cast<TexOffsetSprite>(obj->GetSprite());
		texOffsetSprite->SetUOffset(AERandFloat());
		texOffsetSprite->SetVOffset(AERandFloat());
	}


	ParticleSpawnerBlockHit::ParticleSpawnerBlockHit(const Vec2& collisionPoint, const Vec2& collisionNormal, const Vec2& relativeVelocity, sPtr<Object> blockObject)
		: ParticleSpawnerBase(-1.0f, collisionPoint, Game::C_psBlockHit_lifespan, -1.0f)
		, randGenerator(std::random_device()())
		, spreadRandEngine(0.0f, Game::C_psBlockHit_spreadFactor) // Second param is scale of randomness (higher -> more packed together)
		, speedRandEngine(-Game::C_psBlockHit_speedFactor, Game::C_psBlockHit_speedFactor)
		, lifetimeRandEngine(Game::C_psBlockHit_minLifetimeFactor, 1.0f)
		, rawVelocity(collisionNormal* Vec2Length(relativeVelocity))
		, blockObject(blockObject)
		, spritePtr(SpriteBuilder::CreateTexOffsetSpriteSquare("", MakeVec2(0.2f, 0.2f)))
	{
		isMarkedForDeath = true;
		if (!blockObject)
		{
			Error::RaiseErr("ParticleSpawnerBlockHit(constructor)", "Block empty");
			return;
		}

		spritePtr->SetTexture(blockObject->GetTexture());

		for (int i = 0; i < Vec2LengthSquared(relativeVelocity) / Game::C_psBlockHit_reqVelSqrPerParticle; ++i)
			CreateParticle(0.0f);
	}

	void ParticleSpawnerBlockHit::CreateParticle(float simulateTime)
	{
		// Randomize the direction of the velocity
		f32 spreadVal = spreadRandEngine(randGenerator) / 5.0f;
		if (spreadVal < 0.0f || spreadVal > 1.0f)
			spreadVal = speedRandEngine(randGenerator) * (1.0f / Game::C_psBlockHit_speedFactor);
		Mtx33 rotate;
		AEMtx33RotDeg(&rotate, spreadVal * Game::C_psBlockHit_spreadMaxAngle);
		Vec2 velocity = rotate * rawVelocity;

		// Slow the speed based on the angle difference from the normal
		f32 speedMult = 1.0f - 0.5f * fabsf(spreadVal);
		// Randomize the speed a bit
		speedMult += speedRandEngine(randGenerator);
		velocity *= speedMult;

		sPtr<ParticleBlockHit> particle{};
		if (blockObject->GetTexture())
			particle.reset(new ParticleBlockHit(this, spritePtr, velocity, lifetimeRandEngine(randGenerator)));
		else
			particle.reset(new ParticleBlockHit(this, blockObject->GetColor(), velocity, lifetimeRandEngine(randGenerator)));
		InsertNewParticle(particle, simulateTime);
	}

#pragma endregion // BlockHit

#pragma region Smoke

	ParticleSmoke::ParticleSmoke(const ParticleSpawnerBase* spawnerOwner, const Vec2& velocity, float lifespanMult)
		: ParticleSimple(spawnerOwner, velocity, velocity * 0.5f + MakeVec2(0.0f, 150.0f), MakeColor(128, 128, 128))
	{
		lifespan *= lifespanMult;
		startFadeThreshold = C_psSmoke_fadeThreshold;

		obj->SetScale(MakeVec2(10.0f, 10.0f));
	}


	ParticleSpawnerSmoke::ParticleSpawnerSmoke(const Vec2& position)
		: ParticleSpawnerBase(-1.0f, position, Game::C_psSmoke_lifespan, -1.0f)
		, randGen(
			MakeVec2(C_psSmoke_minLifetimeFactor * C_psSmoke_lifespan, C_psSmoke_lifespan),
			MakeVec2(C_psSmoke_speedMin, C_psSmoke_speedMax),
			C_psSmoke_spreadMaxAngle
		)
	{
		for (int i = 0; i < Game::C_psSmoke_numParticles; ++i)
			CreateParticle(0.0f);
		isMarkedForDeath = true;
	}

	void ParticleSpawnerSmoke::CreateParticle(float simulateTime)
	{
		sPtr<ParticleSmoke> particle(new ParticleSmoke(this, randGen.GenerateVelocity(90.0f), randGen.RandomizeLifetimeMult()));
		InsertNewParticle(particle, simulateTime);
	}

#pragma endregion // Smoke

#pragma region Death Explosion

	ParticleEnemyHitExplosion::ParticleEnemyHitExplosion(const ParticleSpawnerBase* spawnerOwner, const sPtr<Sprite>& sprite, const sPtr<Texture>& texture, const Vec2& velocity, const Vec2& position, float lifespanMult)
		: ParticlePhysTex(spawnerOwner, velocity, texture, sprite)
	{
		Init(position, lifespanMult);
	}
	ParticleEnemyHitExplosion::ParticleEnemyHitExplosion(const ParticleSpawnerBase* spawnerOwner, const Color& color, const Vec2& velocity, const Vec2& position, float lifespanMult)
		: ParticlePhysTex(spawnerOwner, velocity, nullptr)
	{
		Init(position, lifespanMult);

		obj->SetColorTint(color);
	}
	ParticleEnemyHitExplosion::~ParticleEnemyHitExplosion()
	{
		obj->SetLayerMask({ Physics::MASK_ALL });
	}

	void ParticleEnemyHitExplosion::Render()
	{
		ParticlePhysics::Render();
	}

	void ParticleEnemyHitExplosion::Init(const Vec2& position, float lifespanMult)
	{
		obj->SetPosition(position);
		obj->SetScale(MakeVec2(15.0f, 15.0f));
		obj->UnsetLayerMask({ Physics::MASK_ALL });
		obj->SetLayerMask({ Physics::MASK_ENVIRONMENT });

		lifespan *= lifespanMult;
		startFadeThreshold = C_psEnemyHit_fadeThreshold;
	}


	ParticleSpawnerEnemyHitExplosion::ParticleSpawnerEnemyHitExplosion(const sPtr<Object>& obj, bool isDeath)
		: ParticleSpawnerBase(-1.0f, obj->GetPosition(), C_psEnemyHit_lifespan, -1.0f)
		, randGen(
			MakeVec2(C_psEnemyHit_minLifetimeFactor, 1.0f),
			MakeVec2(C_psEnemyHit_speedMin, C_psEnemyHit_speedMax),
			C_psEnemyHit_spreadMaxAngle
		)
		, objWPtr(obj)
	{
		int numParticles = (isDeath ? C_psEnemyHit_numParticlesDeath : C_psEnemyHit_numParticlesHit);
		for (int i = 0; i < numParticles; ++i)
			CreateParticle(0.0f);
	}

	void ParticleSpawnerEnemyHitExplosion::CreateParticle(float simulateTime)
	{
		if (objWPtr.expired())
			return;

		Vec2 direction = randGen.MakeRandomDir(0.0f);
		Vec2 velocity = randGen.GenerateVelocity(direction);
		Vec2 position = transform.GetPosition() + direction * C_blockLength * 0.2f;

		sPtr<ParticleEnemyHitExplosion> particle{ new ParticleEnemyHitExplosion(
			this, C_colorRed, velocity, position, randGen.RandomizeLifetimeMult()
		) };
		InsertNewParticle(particle, simulateTime);
	}

#pragma endregion // Death Explosion

#pragma region Death Shine

	ParticleDeathShine::ParticleDeathShine(const ParticleSpawnerBase* spawnerOwner, const sPtr<Sprite>& sprite, const Vec2& pos, float angle)
		: ParticleSimpleTex(spawnerOwner, Vec2Zero(), Vec2Zero(), C_colorWhite, nullptr, sprite)
	{
		obj->SetPosition(pos);
		obj->SetRotation(angle);
		obj->SetScale(MakeVec2(sCast<f32>(C_windowWidth), sCast<f32>(C_psDShine_thickness)));
	}


	ParticleSpawnerDeathShine::ParticleSpawnerDeathShine(const Vec2& position)
		: ParticleSpawnerBase(-1.0f, position, C_psDShine_lifespan, -1.0f)
		, particleSprite(SpriteBuilder::CreateTexOffsetSpriteSquare(C_filePaths[PNG_PARTICLE_DEATHSHINE], MakeVec2(C_psDShine_uLength, 1.0f)))
		, timeElapsed(0.0f)
	{
		for (int i = 0; i < C_psDShine_numRays; ++i)
			CreateParticle(0.0f);
		isMarkedForDeath = true;
	}

	void ParticleSpawnerDeathShine::Update(float dt)
	{
		timeElapsed += dt;

		float uOffset{};
		if (timeElapsed < C_psDShine_lifespan * 0.4f)
			uOffset = Util::Lerp(0.0f, C_psDShine_transitionLength, timeElapsed / (C_psDShine_lifespan * 0.4f));
		else if (timeElapsed < C_psDShine_lifespan * 0.7f)
			uOffset = C_psDShine_transitionLength;
		else
			uOffset = C_psDShine_transitionLength + Util::Lerp(0.0f, C_psDShine_transitionLength, (timeElapsed - C_psDShine_lifespan * 0.7f) / (C_psDShine_lifespan * 0.3f));
		particleSprite->SetUOffset(uOffset);

		ParticleSpawnerBase::Update(dt);
	}

	void ParticleSpawnerDeathShine::CreateParticle(float simulateTime)
	{
		float angle = particles.size() * 360.0f / C_psDShine_numRays;
		Vec2 pos = transform.GetPosition() + Vec2FromAngle(-angle) * sCast<f32>(C_windowWidth) * 0.5f;

		sPtr<ParticleDeathShine> particle(new ParticleDeathShine(this, particleSprite, pos, angle - 90.0f));
		InsertNewParticle(particle, simulateTime);
	}

#pragma endregion // Death Shine

#pragma region Boss Die Explosion

	ParticleSpawnerBossDieExplosion::ParticleSpawnerBossDieExplosion(const sPtr<Object>& boss)
		: ParticleSpawnerBase(C_bossDeathTime, boss->GetPosition(), C_psBossExplo_lifespan, C_psBossExplo_spawnRate)
		, randGen(
			MakeVec2(1.0f, 1.0f),
			MakeVec2(C_psBossExplo_speedMin, C_psBossExplo_speedMax),
			C_psBossExplo_spreadMaxAngle
		)
		, bossWPtr(boss)
	{
	}

	void ParticleSpawnerBossDieExplosion::CreateParticle(float simulateTime)
	{
		if (bossWPtr.expired())
			return;

		Vec2 direction = randGen.MakeRandomDir(90.0f);
		Vec2 velocity = randGen.GenerateVelocity(direction);
		Vec2 position = transform.GetPosition() + direction * C_blockLength * 0.2f;

		sPtr<ParticleEnemyHitExplosion> particle{ new ParticleEnemyHitExplosion(
			this, C_colorRed, velocity, position, 1.0f
		) };
		InsertNewParticle(particle, simulateTime);
	}

#pragma endregion // Boss Die Explosion

}
