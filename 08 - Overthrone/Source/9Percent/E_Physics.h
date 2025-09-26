/******************************************************************************/
/*!
\file   E_Physics.h
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Kendrick Sim Hean Guan (100%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\brief
  This is an interface file for the physics system.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#pragma once
#include "pch.h"
#include "E_Collision.h"
#include "E_Constants.h"
#include <bitset>
#include <unordered_set>
#include <queue>

// Forward declarations
namespace Engine {
	class PhysicsObject;

	namespace Physics {
		class PhysicsComponent;
	}
}

namespace Engine { namespace Physics {

	// Collision data with physics
	struct PhysicsCollisionData : public CollisionData
	{
	public:
		Vec2 relativeVelocity; // Relative velocity between this object and the other object

		explicit PhysicsCollisionData(const CollisionData& collisionData, const Vec2& relativeVelocity = Vec2Zero());
	};
	// Raycast data with physics
	struct PhysicsRaycastData
	{
	public:
		sPtr<PhysicsComponent> hitObj;
		float distance;
		Vec2 collisionPoint;

		explicit PhysicsRaycastData(const sPtr<PhysicsComponent>& hitObj = nullptr, float distance = -1.0f);

	};
	
	// Internal events for the implementation
	namespace Events {

		enum COMP_UPDATE_TYPE
		{
			COMP_UPDATE_ADD,		// New component to be added
			COMP_UPDATE_DYNAMIC,	// Component has changed whether they are dynamic or static
			COMP_UPDATE_MASK,		// Component has changed their masks
			COMP_UPDATE_DEAD,		// Object has been deleted and should be removed from the system
		};

		struct CompUpdateEvent
		{
		public:
			wPtr<PhysicsComponent> comp;
			COMP_UPDATE_TYPE type;

			CompUpdateEvent(const wPtr<PhysicsComponent>& comp, COMP_UPDATE_TYPE type);
		};

	}

}}

namespace Engine { namespace Physics {

	enum MASK_LAYER
	{
		MASK_ENVIRONMENT = 0,
		MASK_ENVIRONMENT_DYNAMIC,
		MASK_PLAYER,
		MASK_ENEMY,
		MASK_PROJECTILE,
		
		MASK_TOTAL,
		MASK_ALL = MASK_TOTAL, // For indicating that you want to mask everything
	};

	enum FLAG_TYPE
	{
		FLAG_IS_SIMULATED = 0,
		FLAG_IS_DYNAMIC,
		FLAG_IS_TRIGGER,
		FLAG_GRAVITY_ENABLED,

		FLAG_LOCK_ROTATION,

		FLAG_TOTAL
	};

	using Graphics::Transform;

	class PhysicsComponent : public std::enable_shared_from_this<PhysicsComponent>
	{
	protected:
		bool isRegistered; // Is this component registered in the physics system?

		sPtr<ColliderBase> collider;
		std::bitset<FLAG_TOTAL> flags;
		std::bitset<MASK_TOTAL> collisionLayerMask;

		PhysicsObject* objPtr;
		Transform* transformPtr;
		f32 mass;
		f32 restitutionCoeff; // Coefficient of restitution; Elasticity. 1 -> Fully elastic. 0 -> Not elastic at all.
		f32 frictionCoeff; // Coefficient of friction. 1 -> Equal to applied force; stop on a dime. 0 -> No friction; icy.
		Vec2 velocity;
		f32 angVelocity;

		void UpdateLayerMask(bool setBit, const std::initializer_list<MASK_LAYER>& layers);

	public:
		PhysicsComponent(bool registerToSystem, PhysicsObject* objPtr, Transform* transformPtr, bool isDynamic = false, bool isSimulated = false, bool isTrigger = false);
		PhysicsComponent(const PhysicsComponent&) = delete;
		PhysicsComponent(PhysicsComponent&&) = delete;
		~PhysicsComponent();

		void SetCollider(const sPtr<ColliderBase>& newCollider);
		PhysicsObject* GetPhysObject();
		const PhysicsObject* GetPhysObject() const;

		bool GetIsSimulated() const;
		void SetIsSimulated(bool isSimulated);
		bool GetIsDynamic() const;
		void SetIsDynamic(bool isDynamic);
		bool GetIsTrigger() const;
		void SetIsTrigger(bool isTrigger);
		bool GetGravityEnabled() const;
		void SetGravityEnabled(bool isGravityEnabled);
		bool GetLockRotation() const;
		void SetLockRotation(bool lockRotation);

		void SetLayerMask(std::initializer_list<MASK_LAYER> layers);
		void UnsetLayerMask(std::initializer_list<MASK_LAYER> layers);
		bool TestLayerMask(const PhysicsComponent& other) const; // Test whether another physics component overlaps any of our layer masks
		bool TestLayerMask(const std::bitset<MASK_TOTAL>& layers) const;
		bool TestLayerMask(const std::initializer_list<MASK_LAYER> layers) const;
		bool TestLayerMask(MASK_LAYER layer) const; // Test a single layer. MASK_ALL IS NOT SUPPORTED FOR THIS IMPLEMENTATION!
		// For setting up an external layer mask bitset
		static void UpdateLayerMask(std::bitset<MASK_TOTAL>* masks, bool setBit, const std::initializer_list<MASK_LAYER>& layers);

		const Vec2& GetVelocity() const;
		void SetVelocity(const Vec2& newVelocity);
		void AddVelocity(const Vec2& addVelocity);
		f32 GetAngVelocity() const;
		void SetAngVelocity(f32 newAngVelocity);
		void AddAngVelocity(f32 addAngVelocity);
		f32 GetMass() const;
		f32 GetMassReciprocal() const; // 1.0f / mass
		void SetMass(f32 newMass);
		f32 GetRestitutionCoeff() const;
		void SetRestitutionCoeff(f32 newRestiCoeff);
		f32 GetFrictionCoeff() const;
		void SetFrictionCoeff(f32 newFrictionCoeff);
		
		f32 GetMomentOfInertia() const;
		f32 GetMomentOfInertiaReciprocal() const; // 1.0f / inertia
		Vec2 GetMomentum() const;

		void SetTransform(Transform* newTransformPtr);
		const Transform* GetTransform() const;
		void AlignTransform(f32 rotation);

		// Apply a force at a point relative to this object's position.
		void ApplyForce(const Vec2& force, const Vec2& relativePoint, f32 dt);
		// Apply an impulse at a point relative to this object's position.
		void ApplyImpulse(const Vec2& impulse, const Vec2& relativePoint);

		void Update(float dt);
		bool TestCollision(const PhysicsComponent& other, CollisionData* outCollisionData) const;
		bool TestRaycast(const Ray& ray, RaycastData* outRaycastData) const;

		// Push collider outside of the collision
		void ResolveCollision_PushOut(const Vec2& pushVec);

		void NotifyCollision(const PhysicsComponent& other, PhysicsCollisionData& collisionData);
		void NotifySettle();
		void NotifyUnregister();
	};

	class PhysicsManager
	{
	protected:
		std::unordered_set<sPtr<PhysicsComponent>> staticComps;
		std::unordered_set<sPtr<PhysicsComponent>> dynamicComps;

		// Save all collision events here to notify the objects later,
		// so that we don't crash if they delete themselves while we're looping through object pairs.
		struct CollisionNotifyEvent
		{
			wPtr<PhysicsComponent> colliders[2];
			PhysicsCollisionData data;

			CollisionNotifyEvent(const wPtr<PhysicsComponent>& first, const wPtr<PhysicsComponent>& second, const PhysicsCollisionData& data);
			
			// Check if the components pointed to by this collision notify event are valid and safe to act on
			bool IsValid(int minReferences = 2) const;
		};
		// Save all settle events here for the same reason as above
		struct SettleEvent
		{
			sPtr<PhysicsComponent> collider;
			Vec2 normal;

			SettleEvent(sPtr<PhysicsComponent> collider, const Vec2& normal);
		};

		std::queue<Events::CompUpdateEvent> compChangeQueue;

		void FlushCompChanges();
		virtual void PruneDeadComps();

		// SU_ stands for State Update. These modify the state of the unordered_sets which may invalidate
		// iterators. Exercise caution when calling these functions, ensuring you're not iterating any sets.
		void SU_AddComp(const sPtr<PhysicsComponent>& comp);
		void SU_RemoveComp(const sPtr<PhysicsComponent>& comp);
		void SU_UpdateDynamic(const sPtr<PhysicsComponent>& comp);

		// Check and resolve collision if any
		void CheckCollision(sPtr<PhysicsComponent> obj1, sPtr<PhysicsComponent> obj2, bool isObj2Dynamic,
			CollisionData* collisionData, std::queue<CollisionNotifyEvent>* collisionEvents, std::queue<SettleEvent>* settleEvents);
		// Resolve collision by pushing colliders out of each other and modifying their velocities
		static void ResolveCollision(PhysicsComponent& obj1, PhysicsComponent& obj2, CollisionData& collisionData, bool isObj2Dynamic,
									 Vec2* outRelativeVelocity, std::queue<SettleEvent>* settleEventQueue);
		static void LimitVelocity(PhysicsComponent& obj);
		// Attempt to settle the specified object
		static void SettleObject(PhysicsComponent& obj, const Vec2& collisionNormal, std::queue<SettleEvent>* settleEventQueue);

		// Selects objects and checks collision between them. Default implementation simply
		// checks all dynamic vs dynamic and dynamic vs static.
		// Override this if you are able to implement a faster solution for your specific project.
		virtual void CheckCollisionBetweenComponents(std::queue<CollisionNotifyEvent>* collisionEvents, std::queue<SettleEvent>* settleEvents);

	public:
		static sPtr<PhysicsComponent> MakePhysicsComp(bool registerToSystem, PhysicsObject* objPtr, Transform* transformPtr, bool isDynamic = false, bool isSimulated = false, bool isTrigger = false);

		// Inform the physics manager to register, update or remove a component
		void UpdateComp(const wPtr<PhysicsComponent>& comp, Events::COMP_UPDATE_TYPE updateType);

		// Update the positions, rotations and velocities of dynamic objects
		void UpdatePhysics(float dt);
		// Detect and resolve dynamic object collisions
		void UpdateCollision();
		// Update particle and resolve their collision with the static map world
		virtual void UpdateParticlePhysics(const sPtr<PhysicsComponent>& comp, float dt);

		// Cast a ray through the world
		static bool Raycast(const Vec2& rayOrigin, const Vec2& direction, std::initializer_list<MASK_LAYER> layers, PhysicsRaycastData* outRaycastData);

	};

}}

