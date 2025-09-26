/******************************************************************************/
/*!
\file   E_Physics.cpp
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Kendrick Sim Hean Guan (100%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\brief
  This is the implementation file for the physics system.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#include "pch.h"
#include "E_Physics.h"
#include "E_Object.h"
#include "E_Transform.h"
#include <limits>

#include "G_Physics.h"
#include "G_Map.h"

namespace Engine { namespace Physics {

	PhysicsComponent::PhysicsComponent(bool registerToSystem, PhysicsObject* objPtr, Transform* transformPtr, bool isDynamic, bool isSimulated, bool isTrigger)
		: isRegistered(registerToSystem)
		, objPtr(objPtr)
		, collider(objPtr->collider)
		, transformPtr(transformPtr)
		, mass(1.0f)
		, restitutionCoeff(0.4f)
		, frictionCoeff(0.4f)
		, velocity(Vec2Zero())
		, angVelocity(0.0f)
	{
		SetIsSimulated(isSimulated);
		SetIsTrigger(isTrigger);
		flags[FLAG_IS_DYNAMIC] = isDynamic;
		SetGravityEnabled(true);
		SetLockRotation(false);

		SetLayerMask({ MASK_ALL });
	}

	PhysicsComponent::~PhysicsComponent()
	{
	}

	void PhysicsComponent::SetCollider(const sPtr<ColliderBase>& newCollider)
	{
		collider = newCollider;
	}

	PhysicsObject* PhysicsComponent::GetPhysObject() 
	{
		return objPtr;
	}
	
	const PhysicsObject* PhysicsComponent::GetPhysObject() const
	{
		return objPtr;
	}

	bool PhysicsComponent::GetIsSimulated() const
	{
		return flags[FLAG_IS_SIMULATED];
	}
	void PhysicsComponent::SetIsSimulated(bool isSimulated)
	{
		flags[FLAG_IS_SIMULATED] = isSimulated;
	}

	bool PhysicsComponent::GetIsDynamic() const
	{
		return flags[FLAG_IS_DYNAMIC];
	}
	void PhysicsComponent::SetIsDynamic(bool isDynamic)
	{
		if (isDynamic != GetIsDynamic())
		{
			flags[FLAG_IS_DYNAMIC] = isDynamic;

			if (!isDynamic)
			{
				// Reset velocities
				velocity = Vec2Zero();
				angVelocity = 0.0f;
			}

			Singleton<C_TYPE_PHYSICSMANAGER>::Instance()->UpdateComp(weak_from_this(), Events::COMP_UPDATE_DYNAMIC);
		}
	}

	bool PhysicsComponent::GetIsTrigger() const
	{
		return flags[FLAG_IS_TRIGGER];
	}
	void PhysicsComponent::SetIsTrigger(bool isTrigger)
	{
		flags[FLAG_IS_TRIGGER] = isTrigger;
	}

	bool PhysicsComponent::GetGravityEnabled() const
	{
		return flags[FLAG_GRAVITY_ENABLED];
	}
	void PhysicsComponent::SetGravityEnabled(bool isGravityEnabled)
	{
		flags[FLAG_GRAVITY_ENABLED] = isGravityEnabled;
	}
	bool PhysicsComponent::GetLockRotation() const
	{
		return flags[FLAG_LOCK_ROTATION];
	}
	void PhysicsComponent::SetLockRotation(bool lockRotation)
	{
		flags[FLAG_LOCK_ROTATION] = lockRotation;
		if (lockRotation)
			angVelocity = 0.0f;
	}

	void PhysicsComponent::UpdateLayerMask(bool setBit, const std::initializer_list<MASK_LAYER>& layers)
	{
		PhysicsComponent::UpdateLayerMask(&collisionLayerMask, setBit, layers);
	}
	void PhysicsComponent::UpdateLayerMask(std::bitset<MASK_TOTAL>* masks, bool setBit, const std::initializer_list<MASK_LAYER>& layers)
	{
		for (const auto& layer : layers)
		{
			switch (layer)
			{
			case MASK_ALL:
				// Set all masks
				if (setBit)
					masks->set();
				else
					masks->reset();
				return;
			default:
				// Set the corresponding bit
				masks->set(layer, setBit);
			}
		}
	}

	void PhysicsComponent::SetLayerMask(std::initializer_list<MASK_LAYER> layers)
	{
		UpdateLayerMask(true, layers);
	}
	void PhysicsComponent::UnsetLayerMask(std::initializer_list<MASK_LAYER> layers)
	{
		UpdateLayerMask(false, layers);
	}

	bool PhysicsComponent::TestLayerMask(const PhysicsComponent& other) const
	{
		return (collisionLayerMask & other.collisionLayerMask).any();
	}
	bool PhysicsComponent::TestLayerMask(const std::bitset<MASK_TOTAL>& layers) const
	{
		return (collisionLayerMask & layers).any();
	}
	bool PhysicsComponent::TestLayerMask(const std::initializer_list<MASK_LAYER> layers) const
	{
		std::bitset<MASK_TOTAL> masks;
		PhysicsComponent::UpdateLayerMask(&masks, true, layers);

		return TestLayerMask(masks);
	}
	bool PhysicsComponent::TestLayerMask(MASK_LAYER layer) const
	{
		return collisionLayerMask.test(layer);
	}

	const Vec2& PhysicsComponent::GetVelocity() const
	{
		return velocity;
	}
	void PhysicsComponent::SetVelocity(const Vec2& newVelocity)
	{
		if (!GetIsDynamic())
			return;
		velocity = newVelocity;
	}
	void PhysicsComponent::AddVelocity(const Vec2& addVelocity)
	{
		velocity += addVelocity;
	}
	f32 PhysicsComponent::GetAngVelocity() const
	{
		return angVelocity;
	}
	void PhysicsComponent::SetAngVelocity(f32 newAngVelocity)
	{
		if (GetLockRotation() || !GetIsDynamic())
			return;
		angVelocity = newAngVelocity;
	}
	void PhysicsComponent::AddAngVelocity(f32 addAngVelocity)
	{
		if (GetLockRotation() || !GetIsDynamic())
			return;
		angVelocity += addAngVelocity;
	}
	f32 PhysicsComponent::GetMass() const
	{
		// If collider is static, calculations should assume it has infinite mass.
		return GetIsDynamic() ? mass : std::numeric_limits<f32>::infinity();
	}
	f32 PhysicsComponent::GetMassReciprocal() const
	{
		// If collider is static, calculations should assume it has infinite mass -> 1/mass == 0
		return GetIsDynamic() ? 1.0f / mass : 0.0f;
	}
	void PhysicsComponent::SetMass(f32 newMass)
	{
		mass = newMass;
	}
	f32 PhysicsComponent::GetRestitutionCoeff() const
	{
		return restitutionCoeff;
	}
	void PhysicsComponent::SetRestitutionCoeff(f32 newRestiCoeff)
	{
		restitutionCoeff = newRestiCoeff;
	}
	f32 PhysicsComponent::GetFrictionCoeff() const
	{
		return frictionCoeff;
	}
	void PhysicsComponent::SetFrictionCoeff(f32 newFrictionCoeff)
	{
		frictionCoeff = newFrictionCoeff;
	}

	f32 PhysicsComponent::GetMomentOfInertia() const
	{
		return collider->GetMomentOfInertia();
	}
	f32 PhysicsComponent::GetMomentOfInertiaReciprocal() const
	{
		return 1.0f / collider->GetMomentOfInertia();
	}
	Vec2 PhysicsComponent::GetMomentum() const
	{
		return velocity * mass;
	}

	void PhysicsComponent::SetTransform(Transform* newTransformPtr)
	{
		transformPtr = newTransformPtr;
	}
	const Transform* PhysicsComponent::GetTransform() const
	{
		return transformPtr;
	}

	void PhysicsComponent::AlignTransform(f32 rotation)
	{
		if (!GetLockRotation())
			transformPtr->SetRotation(rotation);
	}

	void PhysicsComponent::ApplyForce(const Vec2& force, const Vec2& relativePoint, f32 dt)
	{
		ApplyImpulse(force * dt, relativePoint);
	}
	void PhysicsComponent::ApplyImpulse(const Vec2& impulse, const Vec2& relativePoint)
	{
		if (!GetIsDynamic())
			return;

		AddVelocity(impulse * GetMassReciprocal());
		AddAngVelocity(AERadToDeg(Vec2Cross(relativePoint, impulse) * GetMomentOfInertiaReciprocal()));
	}

	void PhysicsComponent::Update(float dt)
	{
		// Apply gravity and update position
		Vec2 finalVelocity = velocity;
		if (GetGravityEnabled())
			finalVelocity += C_physicsGravity * dt;

		transformPtr->AddPosition((velocity + finalVelocity) * 0.5f * dt);
		velocity = finalVelocity;

		// Rotational velocity
		if (!GetLockRotation())
			transformPtr->AddRotation(angVelocity * dt);
	}

	bool PhysicsComponent::TestCollision(const PhysicsComponent& other, CollisionData* outCollisionData) const
	{
#ifdef _DEBUG
		if (!collider.get())
		{
			Error::RaiseErr("PhysicsComponent::TestCollision", "Missing collider!", ERROR_ERR);
			return false;
		}
#endif

		return collider->TestCollision(*other.collider, outCollisionData);
	}

	bool PhysicsComponent::TestRaycast(const Ray& ray, RaycastData* outRaycastData) const
	{
#ifdef _DEBUG
		if (!collider.get())
		{
			Error::RaiseErr("PhysicsComponent::TestRaycast", "Missing collider!", ERROR_ERR);
			return false;
		}
#endif

		return collider->TestRaycast(ray, outRaycastData);
	}

	void PhysicsComponent::ResolveCollision_PushOut(const Vec2& pushVec)
	{
		transformPtr->AddPosition(pushVec);
	}

	void PhysicsComponent::NotifyCollision(const PhysicsComponent& other, PhysicsCollisionData& collisionData)
	{
		objPtr->OnCollision(*other.objPtr, collisionData);
	}
	void PhysicsComponent::NotifySettle()
	{
		objPtr->OnSettle();
	}

	void PhysicsComponent::NotifyUnregister()
	{
		isRegistered = false;
	}

#pragma region PhysicsManager

	PhysicsManager::CollisionNotifyEvent::CollisionNotifyEvent(const wPtr<PhysicsComponent>& first, const wPtr<PhysicsComponent>& second, const PhysicsCollisionData& data)
		: data(data)
	{
		colliders[0] = first;
		colliders[1] = second;
	}

	bool PhysicsManager::CollisionNotifyEvent::IsValid(int minReferences) const
	{
		return colliders[0].use_count() >= minReferences && colliders[1].use_count() >= minReferences;
	}

	PhysicsManager::SettleEvent::SettleEvent(sPtr<PhysicsComponent> collider, const Vec2& normal)
		: collider(collider)
		, normal(normal)
	{
	}

	sPtr<PhysicsComponent> PhysicsManager::MakePhysicsComp(bool registerToSystem, PhysicsObject* objPtr, Transform* transformPtr, bool isDynamic, bool isSimulated, bool isTrigger)
	{
		sPtr<PhysicsComponent> ptr = sPtr<PhysicsComponent>(new PhysicsComponent(registerToSystem, objPtr, transformPtr, isDynamic, isSimulated, isTrigger));
		if (registerToSystem)
			Singleton<C_TYPE_PHYSICSMANAGER>::Instance()->UpdateComp(ptr, Events::COMP_UPDATE_ADD);
		return ptr;
	}

	void PhysicsManager::UpdateComp(const wPtr<PhysicsComponent>& comp, Events::COMP_UPDATE_TYPE updateType)
	{
		compChangeQueue.emplace(comp, updateType);
	}

	void PhysicsManager::FlushCompChanges()
	{
		for (; !compChangeQueue.empty(); compChangeQueue.pop())
		{
			Events::CompUpdateEvent e = compChangeQueue.front();
			if (e.comp.expired())
				continue;

			auto compPtr = e.comp.lock();

			switch (e.type)
			{
			case Events::COMP_UPDATE_ADD:
				SU_AddComp(compPtr);
				break;
			case Events::COMP_UPDATE_DEAD:
				SU_RemoveComp(compPtr);
				break;
			case Events::COMP_UPDATE_DYNAMIC:
				SU_UpdateDynamic(compPtr);
				break;
			}
		}
	}

	void PhysicsManager::PruneDeadComps()
	{
		for (auto iter = staticComps.begin(); iter != staticComps.end(); )
			if (iter->use_count() <= 1)
				iter = staticComps.erase(iter);
			else
				++iter;
		for (auto iter = dynamicComps.begin(); iter != dynamicComps.end(); )
			if (iter->use_count() <= 1)
				iter = dynamicComps.erase(iter);
			else
				++iter;
	}

	void PhysicsManager::SU_AddComp(const sPtr<PhysicsComponent>& comp)
	{
		// Add to the static/dynamic containers
		auto& toList = (comp->GetIsDynamic() ? dynamicComps : staticComps);
		toList.emplace(comp->shared_from_this());
	}

	void PhysicsManager::SU_RemoveComp(const sPtr<PhysicsComponent>& comp)
	{
		comp->NotifyUnregister();

		// Remove from static/dynamic container
		auto& fromList = (comp->GetIsDynamic() ? dynamicComps : staticComps);
		fromList.erase(comp->shared_from_this());
	}

	void PhysicsManager::SU_UpdateDynamic(const sPtr<PhysicsComponent>& comp)
	{
		auto& fromList = (comp->GetIsDynamic() ? staticComps : dynamicComps);
		auto& toList = (comp->GetIsDynamic() ? dynamicComps : staticComps);

		auto iter = fromList.find(comp->shared_from_this());
		if (iter == fromList.end())
			// Not found. This could be because the comp changed twice this frame, from dynamic to static to dynamic.
			// So this comp should be in the correct list already. Shouldn't need to do anything.
			return;

		toList.emplace(*iter);
		fromList.erase(iter);
	}

	void PhysicsManager::UpdatePhysics(float dt)
	{
		FlushCompChanges();

		for (auto thisComp = dynamicComps.begin(), endIter = dynamicComps.end(); thisComp != endIter; ++thisComp)
			if (thisComp->use_count() > 1) // Don't update dead comps. We'll remove them in UpdateCollision().
				(*thisComp)->Update(dt);
	}
	void PhysicsManager::UpdateCollision()
	{
		FlushCompChanges();
		PruneDeadComps();

		// Save collision and settle events to notify the objects later, so that we don't crash while we're iterating
		std::queue<CollisionNotifyEvent> collisionEvents;
		std::queue<SettleEvent> settleEvents;

		CheckCollisionBetweenComponents(&collisionEvents, &settleEvents);

		// TODO: Rewrite this code to prevent the crash if objects are removed within the notification
		for (; !collisionEvents.empty(); collisionEvents.pop())
		{
			CollisionNotifyEvent e = collisionEvents.front();
			if (!e.IsValid())
				continue;

			sPtr<PhysicsComponent> first = e.colliders[0].lock();
			sPtr<PhysicsComponent> second = e.colliders[1].lock();

			first->NotifyCollision(*second, e.data);
			// This notify collision could've deleted one of the colliders. Check validity before notifying the other way around.
			if (!e.IsValid(3))
				continue;
			e.data.collisionNormal = -e.data.collisionNormal;
			second->NotifyCollision(*first, e.data);
		}
		for (; !settleEvents.empty(); settleEvents.pop())
		{
			SettleEvent e = settleEvents.front();
			e.collider->NotifySettle();
		}
	}
	void PhysicsManager::CheckCollisionBetweenComponents(std::queue<CollisionNotifyEvent>* collisionEvents, std::queue<SettleEvent>* settleEvents)
	{
		CollisionData collisionData{};

		// Dynamic vs Dynamic
		for (auto thisComp = dynamicComps.begin(), endIter = dynamicComps.end(); thisComp != endIter; ++thisComp)
			for (auto otherComp = std::next(thisComp); otherComp != endIter; ++otherComp)
				CheckCollision(*thisComp, *otherComp, true, &collisionData, collisionEvents, settleEvents);

		// Dynamic vs Static
		for (auto thisComp = dynamicComps.begin(), endIter = dynamicComps.end(); thisComp != endIter; ++thisComp)
			for (auto otherComp = staticComps.begin(), otherEnd = staticComps.end(); otherComp != otherEnd; ++otherComp)
				CheckCollision(*thisComp, *otherComp, false, &collisionData, collisionEvents, settleEvents);
	}
	void PhysicsManager::CheckCollision(sPtr<PhysicsComponent> obj1, sPtr<PhysicsComponent> obj2, bool isObj2Dynamic,
		CollisionData* collisionData, std::queue<CollisionNotifyEvent>* collisionEvents, std::queue<SettleEvent>* settleEvents)
	{
		if (!obj1->TestLayerMask(*obj2))
			return;

		bool result = obj1->TestCollision(*obj2, collisionData);
		if (result)
		{
			PhysicsCollisionData physCollisionData(*collisionData);
			ResolveCollision(*obj1, *obj2, *collisionData, isObj2Dynamic, &physCollisionData.relativeVelocity, settleEvents);

			collisionEvents->emplace(obj1, obj2, physCollisionData);
		}
	}

	void PhysicsManager::UpdateParticlePhysics(const sPtr<PhysicsComponent>& comp, float dt)
	{
		// Update position
		comp->Update(dt);

		const Vec2& particlePos = comp->GetTransform()->GetPosition();
		// Get the block that the particle is within
		Game::Block* block = Singleton<Game::Map>::Instance()->GetBlockAtIndex(particlePos);
		// If it's not within any block, no collision
		if (!block)
			return;

		// Get the physics object to get the physics component.
		wPtr<const PhysicsObject> blockPhysObjWeak = block->GetPhysBlockWeak();
		if (blockPhysObjWeak.expired())
			// This block has no collider. Possibly air.
			return;
		wPtr<const PhysicsComponent> blockPhysCompWeak = blockPhysObjWeak.lock()->GetPhysicsCompWeak();
		if (blockPhysCompWeak.expired())
		{
			Error::RaiseErr("PhysicsManager::UpdateParticlePhysics", "Block at pos (%f,%f) has PhysicsObject but invalid PhysicsComponent!");
			return;
		}

		// Collision test
		CollisionData collisionData(nullptr, FLT_MAX, Vec2Zero());
		if (!comp->TestCollision(*blockPhysCompWeak.lock(), &collisionData))
			return;

		// Resolve collision
		// Push the particle out
		comp->ResolveCollision_PushOut(collisionData.collisionNormal * collisionData.penetrationDepth);
		// Remove velocity in the opposite direction of the normal
		comp->AddVelocity(Vec2Dot(comp->GetVelocity(), collisionData.collisionNormal) * Vec2Normalize(comp->GetVelocity()));
	}

	bool PhysicsManager::Raycast(const Vec2& rayOrigin, const Vec2& direction, std::initializer_list<MASK_LAYER> layers, PhysicsRaycastData* outRaycastData)
	{
		auto physManager = Singleton<C_TYPE_PHYSICSMANAGER>::Instance();
		physManager->FlushCompChanges();
		physManager->Physics::PhysicsManager::PruneDeadComps();

		bool hasCollision = false;
		Ray ray(rayOrigin, direction);
		outRaycastData->distance = -1.0f;
		RaycastData raycastData(nullptr, FLT_MAX);
		std::bitset<MASK_TOTAL> masks;
		PhysicsComponent::UpdateLayerMask(&masks, true, layers);

		auto RaycastAllInSet = [&](const std::unordered_set<sPtr<PhysicsComponent>>& set) -> void {
			for (auto& comp : set)
				if (!comp->TestLayerMask(masks))
					continue;
				else if (comp->TestRaycast(ray, &raycastData))
				{
					hasCollision = true;
					if (raycastData.distance < outRaycastData->distance || outRaycastData->distance < 0.0f)
					{
						outRaycastData->hitObj = comp;
						outRaycastData->distance = raycastData.distance;
					}
				}
		};

		RaycastAllInSet(physManager->dynamicComps);
		RaycastAllInSet(physManager->staticComps);

		if (hasCollision)
			outRaycastData->collisionPoint = rayOrigin + direction * outRaycastData->distance;
		return hasCollision;
	}

	void PhysicsManager::ResolveCollision(PhysicsComponent& obj1, PhysicsComponent& obj2, CollisionData& collisionData, bool isObj2Dynamic,
		Vec2* outRelativeVelocity, std::queue<SettleEvent>* settleEventQueue)
	{
		// References to shorten names
		const Vec2& normal = collisionData.collisionNormal;
		const Vec2& point = collisionData.collisionPoint;

		// Push colliders out of each other.
		if (isObj2Dynamic)
		{
			// Push both colliders away from each other equally
			collisionData.penetrationDepth *= 0.5f;
			obj1.ResolveCollision_PushOut(normal * collisionData.penetrationDepth);
			obj2.ResolveCollision_PushOut(normal * -collisionData.penetrationDepth);
		}
		else
		{
			// Push dynamic collider away from static
			obj1.ResolveCollision_PushOut(normal * collisionData.penetrationDepth);
		}

		// Get distance from center to collision point.
		Vec2 r1 = point - obj1.GetTransform()->GetPosition();
		Vec2 r2 = point - obj2.GetTransform()->GetPosition();
		// Rotate these distances by 90 degrees to get perpendicular vector
		Vec2 r1Perp = Vec2Rot90(r1);
		Vec2 r2Perp = Vec2Rot90(r2);
		// Calculate angular velocity at the collision point
		Vec2 angLinearVel1 = r1Perp * AEDegToRad(obj1.GetAngVelocity());
		Vec2 angLinearVel2 = r2Perp * AEDegToRad(obj2.GetAngVelocity());
		// Calculate relative velocity between the 2 colliders at the collision point
		Vec2 relativeVelocity = (obj1.GetVelocity() + angLinearVel1) - (obj2.GetVelocity() + angLinearVel2);
		*outRelativeVelocity = relativeVelocity;

		// If colliders are heading away from the collision, we shouldn't need to do anything
		f32 dotVal = Vec2Dot(relativeVelocity, normal);
		if (dotVal >= 0.0f)
			return;

		// Get the component of the perpendicular distances in the direction of the collision normal
		f32 r1PerpDotNorm = Vec2Dot(r1Perp, normal);
		f32 r2PerpDotNorm = Vec2Dot(r2Perp, normal);

		// Calculate the restitution involved in this collision
		f32 avgRestitution = (obj1.GetRestitutionCoeff() + obj2.GetRestitutionCoeff()) * 0.5f;
		// Calculate the velocity change value applied in the normal, ignoring mass/inertia for now
		f32 velChangeByNormal = -Vec2Dot(relativeVelocity * (1.0f + avgRestitution), normal);
		// Calculate the denominator consisting of mass and inertia that will make impulse take mass/inertia into account
		f32 denominator = (obj1.GetMassReciprocal() + obj2.GetMassReciprocal()) + // mass
						  (Util::PowSqr(r1PerpDotNorm) * obj1.GetMomentOfInertiaReciprocal()) + // obj1 inertia
						  (Util::PowSqr(r2PerpDotNorm) * obj2.GetMomentOfInertiaReciprocal()); // obj2 inertia
		// Get the impulse applied onto both objects
		f32 impulseMag = velChangeByNormal / denominator;
		Vec2 impulse = normal * impulseMag;

		// Calculate the friction involved in this collision
		f32 actingFriction = min(obj1.GetFrictionCoeff(), obj2.GetFrictionCoeff());
		// Calculate the direction of friction
		Vec2 frictionDir = Vec2Rot90(normal);
		if (Vec2Dot(relativeVelocity, frictionDir) > 0.0f)
			frictionDir = -frictionDir;
		// Calculate the velocity change value applied by friction, ignoring mass/inertia for now
		f32 velChangeByFriction = -Vec2Dot(relativeVelocity * 0.5f * actingFriction, frictionDir);
		// Get the impulse applied by friction
		f32 fricImpulseMag = velChangeByFriction / denominator;
		impulse += frictionDir * fricImpulseMag;

		// Apply linear and angular velocity changes
		obj1.ApplyImpulse(impulse, r1);
		if (isObj2Dynamic)
			obj2.ApplyImpulse(-impulse, r2);
		LimitVelocity(obj1);
		LimitVelocity(obj2);

		// Align the objects if ang velocity is negligible and they're near parallel
		SettleObject(obj1, collisionData.collisionNormal, settleEventQueue);
		if (isObj2Dynamic)
			SettleObject(obj2, collisionData.collisionNormal, settleEventQueue);
	}

	void PhysicsManager::LimitVelocity(PhysicsComponent& obj)
	{
		// Limit velocity
		if (Vec2LengthSquared(obj.GetVelocity()) > C_physicsMaxVelocity * C_physicsMaxVelocity)
			obj.SetVelocity(Vec2Normalize(obj.GetVelocity()) * C_physicsMaxVelocity);
		// Limit angular velocity
		if (std::fabsf(obj.GetAngVelocity()) > C_physicsMaxAngVelocity)
			obj.SetAngVelocity(C_physicsMaxAngVelocity - std::signbit(obj.GetAngVelocity()) * 2.0f * C_physicsMaxAngVelocity);
	}

	void PhysicsManager::SettleObject(PhysicsComponent& obj, const Vec2& collisionNormal, std::queue<SettleEvent>* settleEventQueue)
	{
		// Don't settle if it's still moving significantly
		if (std::abs(Vec2LengthSquared(obj.GetVelocity())) > C_physicsSettleVelocity ||
			std::abs(obj.GetAngVelocity()) > C_physicsSettleAngVelocity)
			return;

		// Don't settle if it's not nearly aligned with the collision normal
		Vec2 up = Vec2FromAngle(obj.GetTransform()->GetRotation());
		Vec2 right = Vec2Rot90(up);

		f32 upAngDiff = Vec2AngBtwNormalized(up, collisionNormal);
		f32 rightAngDiff = Vec2AngBtwNormalized(right, collisionNormal);

		f32 positiveUpAngDiff = std::abs(upAngDiff), positiveRightAngDiff = std::abs(rightAngDiff);
		if (!(positiveUpAngDiff < C_physicsSettleAngle || positiveRightAngDiff < C_physicsSettleAngle))
			return;

		// Align to normal
		f32 normalAng = Vec2ToAngle(collisionNormal);
		obj.AlignTransform(std::roundf((obj.GetTransform()->GetRotation() - normalAng) / 90.0f) * 90.0f + normalAng);
		obj.SetVelocity(Vec2Zero());
		obj.SetAngVelocity(0.0f);

		settleEventQueue->emplace(obj.shared_from_this(), collisionNormal);
	}

#pragma endregion


	PhysicsCollisionData::PhysicsCollisionData(const CollisionData& collisionData, const Vec2& relativeVelocity)
		: CollisionData(collisionData)
		, relativeVelocity(relativeVelocity)
	{
	}

	PhysicsRaycastData::PhysicsRaycastData(const sPtr<PhysicsComponent>& hitObj, float distance)
		: hitObj(hitObj)
		, distance(distance)
		, collisionPoint(Vec2Zero())
	{
	}

namespace Events {

	CompUpdateEvent::CompUpdateEvent(const wPtr<PhysicsComponent>& comp, COMP_UPDATE_TYPE type)
		: comp(comp)
		, type(type)
	{
	}

}

}}
