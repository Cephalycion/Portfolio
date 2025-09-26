/******************************************************************************/
/*!
\file   E_ParticleSystem.h
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Kendrick Sim Hean Guan (100%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\brief
  This is an interface file for a particle system structure.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#pragma once
#include "pch.h"
#include "E_Object.h"
#include "E_ObjectLayer.h"
#include <random>

namespace Engine { namespace Graphics {

	// Forward declarations
	class ParticleSpawnerBase;

	// Particle spawner layers for rendering order
	enum PARTICLE_LAYER
	{
		PARTICLE_LAYER_GAME = 0,
		PARTICLE_LAYER_ABOVE_MAP,
		PARTICLE_LAYER_ABOVE_DYNAMIC,

		PARTICLE_LAYER_TOTAL
	};


	// Abstract implementation of a particle in the world
	class ParticleBase
	{
	protected:
		const ParticleSpawnerBase* spawner; // Ptr to the owner of this particle. Not smart pointer so that there's no circular referencing.
		float lifespan;		// Lifetime remaining.
		float startFadeThreshold; // Start fading the particle when lifespan falls below this threshold

		ParticleBase(const ParticleSpawnerBase* spawnerOwner);

	public:
		bool IsDead() const;

		// Initialize this particle for reuse
		// (Don't know whether this should be kept.. specialized particles might need more information)
		virtual void Init(const ParticleSpawnerBase* spawnerOwner);
		virtual void Update(float dt) = 0;
		virtual void Render() = 0;

	};

	// A simple particle with velocity that does not collide with the world
	class ParticleSimple : public ParticleBase
	{
	protected:
		sPtr<Object> obj;	// Render this particle as this object. Position/rotation/color are stored here.
		Vec2 velocity;
		Vec2 acceleration;

	public:
		ParticleSimple(const ParticleSpawnerBase* spawnerOwner, const Vec2& velocity, const Vec2& acceleration = Vec2Zero(),
			const Color& colorTint = C_colorWhite);
		virtual ~ParticleSimple();

		virtual void Update(float dt) override;
		virtual void Render() override;

	};

	// A particle that interacts with the physics system
	class ParticlePhysics : public ParticleBase
	{
	protected:
		sPtr<PhysicsObject> obj;

	public:
		ParticlePhysics(const ParticleSpawnerBase* spawnerOwner, const Vec2& velocity);
		virtual ~ParticlePhysics();

		sPtr<PhysicsObject> GetObj();

		virtual void Update(float dt) override;
		virtual void Render() override;

	};

	// Abstract implementation of a particle spawner, a point in the world where particles are spawned from
	class ParticleSpawnerBase
	{
	protected:
		std::vector<sPtr<ParticleBase>> particles;
		Transform transform;
		
		float spawnerLifespan; // Length of time remaining in the life of this spawner.
		float particleLifespan;	// The lifespan of a particle spawned by this spawner.

		float particleSpawnInterval; // How long to wait before spawning another particle.
		float particleSpawnTimeLeft; // How long left until spawning another particle.

		bool isMarkedForDeath; // Whether this spawner should no longer spawn particles.

		// Time to create particle(s). Called by the base spawner implementation when it is time to spawn a new particle.
		// The implementation should create the particle and then call InsertNewParticle().
		// Your class may also call this arbituarily depending on implementation needs.
		virtual void CreateParticle(float simulateTime) = 0;
		// Simulates and inserts a new particle into this spawner. This should only be called by CreateParticle().
		void InsertNewParticle(sPtr<ParticleBase> particle, float simulateTime);

	public:
		ParticleSpawnerBase(float spawnerLifespan, const Vec2& position, float particleLifespan, float particleSpawnInterval);

		const Transform& GetTransform() const;
		float GetParticleLifespan() const;
		bool GetIsDead() const; // Is this spawner dead (marked for death and has no particles)

		// Update, spawn particles.
		virtual void Update(float dt);
		void Render();

	};

	// Handles the object pool of the particles
	class ParticleObjectPool
	{
	protected:
		std::queue<sPtr<Object>> objectPool;
		std::queue<sPtr<PhysicsObject>> physObjectPool;

		// Store a ptr to the default mesh to reset objects returned to us.
		sPtr<Mesh> defaultMeshPtr;

		void ResetNormalObject(sPtr<Object> object);
		void ResetPhysicsObject(sPtr<PhysicsObject> object);

	public:
		ParticleObjectPool();

		// For particle creation usage
		// Gets an object from the object pool
		sPtr<Object> GetNormalObject();
		sPtr<PhysicsObject> GetPhysicsObject();

		// Return an object to the pool for later reuse
		void ReturnNormalObject(sPtr<Object> object);
		void ReturnPhysicsObject(sPtr<PhysicsObject> object);

	};

	// Manages all spawners and particles
	class ParticleManager : public IObjectLayerManager<ParticleSpawnerBase, PARTICLE_LAYER, PARTICLE_LAYER_TOTAL>
	{
	protected:

	public:
		virtual ~ParticleManager();

		virtual void AddObject(PARTICLE_LAYER layer, const sPtr<ParticleSpawnerBase>& obj) override;

		virtual void Update(float dt) override;
		virtual void Render(PARTICLE_LAYER layer);

	};

}}
