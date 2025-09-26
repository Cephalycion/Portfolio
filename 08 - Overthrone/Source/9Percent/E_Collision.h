/******************************************************************************/
/*!
\file   E_Collision.h
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Kendrick Sim Hean Guan (100%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\brief
  This is an interface file for collision primitives and methods for
  testing for collision.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#pragma once
#include "pch.h"
#include "E_Constants.h"
#include "E_Transform.h"

namespace Engine { namespace Physics {

	/* Forward Declares */
	class Collision;
	class ColliderBase;
	class OBBCollider;
	class ABBCollider;
	class PointCollider;
	struct CollisionData;
	struct Ray;
	struct RaycastData;

	/* Using Declares */
	using Graphics::Transform;

	
	enum COLLIDER_TYPE
	{
		COLLIDER_AABB = 0,
		COLLIDER_OBB,
		COLLIDER_POINT,

		COLLIDER_TYPE_TOTAL
	};

	/* Colliders */
	class ColliderBase
	{
	protected:
		COLLIDER_TYPE type;
		Transform* transformPtr;

		// Update the state of the transform object (will also update the collider due to the transform callback)
		void UpdateTransformState() const;

		ColliderBase(COLLIDER_TYPE type, Transform* transformPtr);

	public:
		const Transform* GetTransformPtr() const;
		COLLIDER_TYPE GetType() const;

		// Physics
		virtual f32 GetMomentOfInertia() const = 0;

		virtual bool TestCollision(const ColliderBase& other, CollisionData* outCollisionData) const = 0;
		virtual bool TestRaycast(const Ray& ray, RaycastData* outRaycastData) const = 0;

	};

	class PointCollider : public ColliderBase
	{
	protected:
		// Point vs OBB
		// Note: Collision point is not calculated!
		bool TestCollision_OBB(const OBBCollider& other, CollisionData* outCollisionData) const;
		bool TestRaycast(const Ray& ray, RaycastData* outRaycastData) const;

	public:
		PointCollider(Transform* transformPtr);

		virtual f32 GetMomentOfInertia() const override;

		virtual bool TestCollision(const ColliderBase& other, CollisionData* outCollisionData) const override;
	};

	class AABBCollider : public ColliderBase
	{
	protected:
		Vec2 halfLengths;

		// AABB vs AABB
		bool TestCollision_AABB(const AABBCollider& other, CollisionData* outCollisionData) const;

	public:
		AABBCollider(Transform* transformPtr);

		virtual f32 GetMomentOfInertia() const override;

		virtual bool TestCollision(const ColliderBase& other, CollisionData* outCollisionData) const override;

	};

	class OBBCollider : public ColliderBase
	{
	protected:
		Vec2 halfLengths;
		Vec2 axes[2];
		Vec2 vertices[4];

		void UpdateVertices(const Transform& thisTransform);

		// OBB vs OBB
		bool TestCollision_OBB(const OBBCollider& other, CollisionData* outCollisionData) const;

	public:
		OBBCollider(Transform* transformPtr);

		const Vec2& GetHalfLengths() const;
		const Vec2& GetAxis(int index) const;

		virtual f32 GetMomentOfInertia() const override;

		virtual bool TestCollision(const ColliderBase& other, CollisionData* outCollisionData) const override;
		virtual bool TestRaycast(const Ray& ray, RaycastData* outRaycastData) const override;

	};

	/* Collision Data */
	struct CollisionData
	{
	public:
		const ColliderBase* collider;
		Vec2 collisionPoint;
		Vec2 collisionNormal;
		float penetrationDepth;

		CollisionData();
		CollisionData(const ColliderBase* collider, float penetrationDepth, const Vec2& collisionNormal);

		// For comparison between different possible collision axes, to find the axis with the smallest penetration
		bool operator<(const CollisionData& other) const;
	};

	struct Ray
	{
	public:
		Vec2 originPoint;
		Vec2 direction;

		Ray();
		Ray(const Vec2& originPoint, const Vec2& direction);
	};

	struct RaycastData
	{
	public:
		const ColliderBase* collider;
		float distance;

		RaycastData();
		RaycastData(const ColliderBase* collider, float distance);
	};

	class Collision
	{
	protected:
		Collision() = delete;


	public:
		static void CalculateSATExtent_Point(const Vec2& axis, const Vec2& pos, float* minExtent, float* maxExtent);
		static void CalculateSATExtents_Rect(const Vec2& axis, const Vec2* vertices, float* minExtent, float* maxExtent);

		// False if the ray definitely doesn't collide with the bounding box.
		// True if the ray may still collide.
		static bool CalculateRay_CollisionOnAxis(const Vec2& axis, const Vec2& posOffset, f32 halfLength, const Vec2& rayDir, f32* outTMin, f32* outTMax);

	};

}}
