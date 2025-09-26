/******************************************************************************/
/*!
\file   E_ParticleSystem.cpp
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Kendrick Sim Hean Guan (100%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\brief
  This is the implementation file for a particle system structure.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#include "pch.h"
#include "E_ParticleSystem.h"

namespace Engine { namespace Graphics {

	ParticleBase::ParticleBase(const ParticleSpawnerBase* spawnerOwner)
		: spawner(spawnerOwner)
		, lifespan(spawnerOwner->GetParticleLifespan())
		, startFadeThreshold(-1.0f)
	{
	}

	bool ParticleBase::IsDead() const
	{
		return lifespan <= 0.0f;
	}

	void ParticleBase::Init(const ParticleSpawnerBase* spawnerOwner)
	{
		spawner = spawnerOwner;
		lifespan = spawnerOwner->GetParticleLifespan();
	}

	void ParticleBase::Update(float dt)
	{
		lifespan -= dt;
	}


	ParticleSimple::ParticleSimple(const ParticleSpawnerBase* spawnerOwner, const Vec2& velocity, const Vec2& acceleration,
		const Color& colorTint)
		: ParticleBase(spawnerOwner)
		, obj(Singleton<ParticleObjectPool>::Instance()->GetNormalObject())
		, velocity(velocity)
		, acceleration(acceleration)
	{
		obj->SetTransform(spawnerOwner->GetTransform());
		obj->SetColorTint(colorTint);
	}

	ParticleSimple::~ParticleSimple()
	{
		if (Singleton<ParticleObjectPool>::IsInitialized())
			Singleton<ParticleObjectPool>::Instance()->ReturnNormalObject(obj);
	}

	void ParticleSimple::Update(float dt)
	{
		ParticleBase::Update(dt);
		if (lifespan < startFadeThreshold)
			obj->SetTransparency(lifespan / startFadeThreshold);

		// dx = vt + .5at^2
		obj->AddPosition(velocity * dt + acceleration * dt * dt * 0.5f);

		// v = v + at
		velocity += acceleration * dt;
	}

	void ParticleSimple::Render()
	{
		obj->Render();
	}


	ParticlePhysics::ParticlePhysics(const ParticleSpawnerBase* spawnerOwner, const Vec2& velocity)
		: ParticleBase(spawnerOwner)
		, obj(Singleton<ParticleObjectPool>::Instance()->GetPhysicsObject())
	{
		obj->SetPosition(spawner->GetTransform().GetPosition());
		obj->SetVelocity(velocity);
	}
	ParticlePhysics::~ParticlePhysics()
	{
		if (Singleton<ParticleObjectPool>::IsInitialized())
			Singleton<ParticleObjectPool>::Instance()->ReturnPhysicsObject(obj);
	}

	sPtr<PhysicsObject> ParticlePhysics::GetObj()
	{
		return obj;
	}

	void ParticlePhysics::Update(float dt)
	{
		ParticleBase::Update(dt);
		if (lifespan < startFadeThreshold)
			obj->SetTransparency(lifespan / startFadeThreshold);

		sPtr<Physics::PhysicsComponent> comp(obj->GetPhysicsCompWeak().lock());
		if (comp)
			Singleton<C_TYPE_PHYSICSMANAGER>::Instance()->UpdateParticlePhysics(comp, dt);
		else
			Error::RaiseErr("ParticlePhysics", "Failed to retrieve physics component!");
	}

	void ParticlePhysics::Render()
	{
		obj->Render();
	}


	ParticleSpawnerBase::ParticleSpawnerBase(float spawnerLifespan, const Vec2& position, float particleLifespan, float particleSpawnInterval)
		: transform(position, 0.0f, MakeVec2(100.0f, 100.0f))
		, spawnerLifespan(spawnerLifespan)
		, particleLifespan(particleLifespan)
		, particleSpawnInterval(particleSpawnInterval)
		, particleSpawnTimeLeft(0.0f)
		, isMarkedForDeath(spawnerLifespan <= 0.0f)
	{
	}

	const Transform& ParticleSpawnerBase::GetTransform() const
	{
		return transform;
	}
	float ParticleSpawnerBase::GetParticleLifespan() const
	{
		return particleLifespan;
	}
	bool ParticleSpawnerBase::GetIsDead() const
	{
		return isMarkedForDeath && particles.empty();
	}

	void ParticleSpawnerBase::Update(float dt)
	{
		if (GetIsDead())
			return;

#ifdef _DEBUG
		if (particleSpawnInterval < 0.0f && particles.empty())
			Error::RaiseErr("ParticleSpawnerBase::Update", "A one-shot spawner has 0 particles! Did you forget to set isMarkedForDeath?", ERROR_WARN);
#endif

		/* Update existing particles */
		for (auto iter = particles.begin(); iter != particles.end(); )
		{
			(*iter)->Update(dt);
			// Only delete the particle if it's dead.
			if ((*iter)->IsDead())
				iter = particles.erase(iter);
			else
				++iter;
		}

		/* Spawn new particles */
		if (isMarkedForDeath || // Don't spawn more particles if this spawner is dead
			particleSpawnInterval < 0.0f) // Automatic spawning of particles is disabled for this spawner implementation
			return;

		particleSpawnTimeLeft -= dt;
		// Clamp particle spawn time to the spawner's remaining lifespan
		if (spawnerLifespan > 0.0f)
			particleSpawnTimeLeft = Util::Clamp(particleSpawnTimeLeft, -spawnerLifespan, FLT_MAX);

		// Skip spawning particles that, when simulated with the time they should've been alive,
		// are dead by the end of this frame.
		if (-particleSpawnTimeLeft > particleLifespan)
			particleSpawnTimeLeft += particleLifespan * (sCast<s32>((-particleSpawnTimeLeft - particleLifespan) / particleLifespan) + 1);

		// Spawn the particles
		for (; particleSpawnTimeLeft < 0.0f; particleSpawnTimeLeft += particleSpawnInterval)
		{
			CreateParticle(-particleSpawnTimeLeft);

			// Don't spawn more particles if the spawner just marked itself for death.
			if (isMarkedForDeath)
				break;
		}

		// Decrease spawner lifespan
		spawnerLifespan -= dt;
		if (spawnerLifespan < 0.0f)
			isMarkedForDeath = true;
	}

	void ParticleSpawnerBase::Render()
	{
		if (GetIsDead())
			return;

		for (const auto& particle : particles)
			particle->Render();
	}

	void ParticleSpawnerBase::InsertNewParticle(sPtr<ParticleBase> particle, float simulateTime)
	{
		particle->Update(simulateTime);
		particles.emplace_back(particle);
	}


	ParticleObjectPool::ParticleObjectPool()
		: defaultMeshPtr(AssetManager::GetMeshSquare())
	{
		for (int i = 0; i < C_particleReservedCount; ++i)
			objectPool.emplace(ObjectBuilder::CreateSquare());
		for (int i = 0; i < C_particlePhysReservedCount; ++i)
		{
			auto physObj = ObjectBuilder::CreatePhysicsSquare(false, true);
			physObj->SetCollider<Physics::PointCollider>();
			physObjectPool.emplace(physObj);
		}
	}

	void ParticleObjectPool::ResetNormalObject(sPtr<Object> object)
	{
		object->SetTransform(Transform());
		object->SetColorTint(Color());
		object->SetTransparency(1.0f);
		object->SetMesh(defaultMeshPtr);
		// Add more attributes if needed
	}
	void ParticleObjectPool::ResetPhysicsObject(sPtr<PhysicsObject> object)
	{
		ResetNormalObject(std::dynamic_pointer_cast<Object>(object));

		object->SetVelocity(Vec2Zero());
		object->SetAngVelocity(0.0f);
		object->SetIsDynamic(true);
		object->SetGravityEnabled(true);
		object->SetLockRotation(false);
		object->SetLayerMask({ Physics::MASK_ALL });
		object->SetMass(1.0f);
		object->SetRestitutionCoeff(0.4f);
		object->SetFrictionCoeff(0.4f);
		object->SetOnCollisionCallback(nullptr);
		object->SetOnSettleCallback(nullptr);
	}

	sPtr<Object> ParticleObjectPool::GetNormalObject()
	{
		if (!objectPool.empty())
		{
			sPtr<Object> obj = objectPool.front();
			objectPool.pop();
			ResetNormalObject(obj);
			return obj;
		}
		else
			return ObjectBuilder::CreateSquare();
	}
	sPtr<PhysicsObject> ParticleObjectPool::GetPhysicsObject()
	{
		if (!physObjectPool.empty())
		{
			sPtr<PhysicsObject> obj = physObjectPool.front();
			physObjectPool.pop();
			ResetPhysicsObject(obj);
			return obj;
		}
		else
		{
			auto physObj = ObjectBuilder::CreatePhysicsSquare(false, true);
			physObj->SetCollider<Physics::PointCollider>();
			return physObj;
		}
	}

	void ParticleObjectPool::ReturnNormalObject(sPtr<Object> object)
	{
		objectPool.emplace(object);
	}
	void ParticleObjectPool::ReturnPhysicsObject(sPtr<PhysicsObject> object)
	{
		physObjectPool.emplace(object);
	}


	ParticleManager::~ParticleManager()
	{
		Singleton<ParticleObjectPool>::Destroy();
	}

	void ParticleManager::AddObject(PARTICLE_LAYER layer, const sPtr<ParticleSpawnerBase>& obj)
	{
		IObjectLayerManager::AddObject(layer, obj);
		// Flush changes into the objects vector so we can get the total number of objects.
		Flush(layer);
		// Remove the oldest spawner if we've exceeded the max number of spawners allowed.
		if (objects[layer].size() > C_particleSpawnerMaxCountPerLayer)
			objects[layer].erase(objects[layer].begin());
	}

	void ParticleManager::Update(float dt)
	{
		SafeRunMacro(std::bind([](const sPtr<ParticleSpawnerBase>& spawner, float dt) -> bool {
			spawner->Update(dt);
			return !spawner->GetIsDead();
		}, std::placeholders::_1, dt));

		IObjectLayerManager::Update(dt);
	}

	void ParticleManager::Render(PARTICLE_LAYER layer)
	{
		for (const auto& spawner : objects[layer])
			spawner->Render();
	}

}}
