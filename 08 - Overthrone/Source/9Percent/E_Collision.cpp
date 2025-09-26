/******************************************************************************/
/*!
\file   E_Collision.cpp
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Kendrick Sim Hean Guan (100%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\brief
  This is the implementation file for collision primitives and methods for
  testing for collision.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#include "pch.h"
#include "E_Collision.h"
#include "E_Object.h"
#include <algorithm>

namespace Engine { namespace Physics {

	ColliderBase::ColliderBase(COLLIDER_TYPE type, Transform* transformPtr)
		: type(type)
		, transformPtr(transformPtr)
	{
		transformPtr->SetOnUpdateCallback(nullptr);
	}
	const Transform* ColliderBase::GetTransformPtr() const
	{
		return transformPtr;
	}
	COLLIDER_TYPE ColliderBase::GetType() const
	{
		return type;
	}

	void ColliderBase::UpdateTransformState() const
	{
		// Update the vertices and axes of this collider
		transformPtr->ForceUpdate();
	}

	bool ColliderBase::TestCollision(const ColliderBase& other, CollisionData* outCollisionData) const
	{
		UNREFERENCED_PARAMETER(outCollisionData);
		
		// Update the vertices and axes of this collider
		UpdateTransformState();
		other.UpdateTransformState();
		
		// Delegate to child implementations for the testing of collision;
		return false;
	}


	PointCollider::PointCollider(Transform* transformPtr)
		: ColliderBase(COLLIDER_POINT, transformPtr)
	{
	}

	f32 PointCollider::GetMomentOfInertia() const
	{
		return 0.0f;
	}

	bool PointCollider::TestCollision_OBB(const OBBCollider& other, CollisionData* outCollisionData) const
	{
		outCollisionData->penetrationDepth = FLT_MAX;
		Vec2 posOffset = transformPtr->GetPosition() - other.GetTransformPtr()->GetPosition();

		// Test 1 of the OBB axes
		static auto TestAxis = [&other](const Vec2& pointRelativePos, Vec2 axis, float obbHalflength, CollisionData* outCollisionData) -> bool {
			// Project relative pos onto axis
			float dotVal = Vec2Dot(pointRelativePos, axis);
			if (dotVal < 0.0f)
			{
				// Flip the axis to the correct collision normal direction
				axis = -axis;
				dotVal = -dotVal;
			}

			// Check if we're beyond the bounds of the OBB
			if (dotVal > obbHalflength)
				return false;

			
			// We're colliding.
			float collisionDepth = obbHalflength - dotVal;
			if (collisionDepth < outCollisionData->penetrationDepth)
			{
				outCollisionData->collider = &other;
				outCollisionData->collisionNormal = axis;
				outCollisionData->penetrationDepth = obbHalflength - dotVal;
			}
			return true;
		};

		// Test the 2 axes of the OBB
		if (!(TestAxis(posOffset, other.GetAxis(0), other.GetHalfLengths().x, outCollisionData) &&
			  TestAxis(posOffset, other.GetAxis(1), other.GetHalfLengths().y, outCollisionData)))
			return false;

		// We're colliding
		return true;
	}
	bool PointCollider::TestRaycast(const Ray& ray, RaycastData* outRaycastData) const
	{
		UNREFERENCED_PARAMETER(ray);
		UNREFERENCED_PARAMETER(outRaycastData);
		Error::RaiseErr("PointCollider::TestRaycast", "Undefined!");
		return false;
	}

	bool PointCollider::TestCollision(const ColliderBase& other, CollisionData* outCollisionData) const
	{
		ColliderBase::TestCollision(other, outCollisionData);

		switch (other.GetType())
		{
		case COLLIDER_OBB:
			return TestCollision_OBB(sCast<const OBBCollider&>(other), outCollisionData);
		default:
			Error::RaiseErrFormatted("PointCollider::TestCollision", "Unimplemented type %d", ERROR_ERR, other.GetType());
			return false;
		}
	}


	AABBCollider::AABBCollider(Transform* transformPtr)
		: ColliderBase(COLLIDER_AABB, transformPtr)
		, halfLengths(transformPtr->GetScale() * 0.5f)
	{
	}

	f32 AABBCollider::GetMomentOfInertia() const
	{
		// TODO
		return 1.0f;
	}

	bool AABBCollider::TestCollision_AABB(const AABBCollider& other, CollisionData* outCollisionData) const
	{
		Vec2 halfLengthsTotal = halfLengths + other.halfLengths;
		Vec2 positionDiff = transformPtr->GetPosition() - other.transformPtr->GetPosition();
		Vec2 positivePositionDiff = Vec2Abs(positionDiff);

		// Colliders are only colliding when position is less than sum of half lengths
		Vec2 penetrationDist = halfLengthsTotal - positivePositionDiff;
		if (penetrationDist.x < 0.0f || penetrationDist.y < 0.0f)
			return false;

		// If x axis has smaller penetration than y axis, shortest separation vector is along x axis
		if (penetrationDist.x < penetrationDist.y)
		{
			outCollisionData->collisionNormal = MakeVec2((positionDiff.x >= 0.0f ? 1.0f : -1.0f), 0.0f);
			outCollisionData->penetrationDepth = penetrationDist.x;
		}
		// Else shortest separation vector is along y axis
		else
		{
			outCollisionData->collisionNormal = MakeVec2(0.0f, (positionDiff.y >= 0.0f ? 1.0f : -1.0f));
			outCollisionData->penetrationDepth = penetrationDist.y;
		}
		return true;
	}
	bool AABBCollider::TestCollision(const ColliderBase& other, CollisionData* outCollisionData) const
	{
		ColliderBase::TestCollision(other, outCollisionData);

		switch (other.GetType())
		{
		case COLLIDER_AABB:
			return TestCollision_AABB(sCast<const AABBCollider&>(other), outCollisionData);
		default:
			Error::RaiseErrFormatted("AABBCollider::TestCollision", "Unimplemented type %d", ERROR_ERR, other.GetType());
			return false;
		}
	}


	OBBCollider::OBBCollider(Transform* transformPtr)
		: ColliderBase(COLLIDER_OBB, transformPtr)
		, halfLengths(transformPtr->GetScale() * 0.5f)
		, vertices()
		, axes()
	{
		transformPtr->SetOnUpdateCallback(std::bind(&OBBCollider::UpdateVertices, this, std::placeholders::_1));
		transformPtr->ForceUpdate();
	}

	const Vec2& OBBCollider::GetHalfLengths() const
	{
		return halfLengths;
	}
	const Vec2& OBBCollider::GetAxis(int index) const
	{
		return axes[index];
	}

	f32 OBBCollider::GetMomentOfInertia() const
	{
		f32 smallerSide, greaterSide;
		if (halfLengths.x < halfLengths.y)
		{
			smallerSide = halfLengths.x;
			greaterSide = halfLengths.y;
		}
		else
		{
			smallerSide = halfLengths.y;
			greaterSide = halfLengths.x;
		}
		return (smallerSide * greaterSide * greaterSide) * 0.01f;
	}

	void Debug_DrawVertex(const Vec2& pos, const Vec2& vertex, Color color = MakeColor(0, 255, 0, 255))
	{
		if (!C_debugDoRender)
			return;

		auto obj = ObjectBuilder::CreateSquare();
		obj->SetColorTint(color);
		obj->SetPosition(pos + vertex);
		obj->SetScale(MakeVec2(20.0f, 20.0f));
		obj->Render();
	}
	void Debug_DrawVertices(const Vec2* vertices, int count = 4, Color color = MakeColor(255, 0, 255, 255))
	{
		if (!C_debugDoRender)
			return;

		auto obj = ObjectBuilder::CreateSquare();
		obj->SetColorTint(color);
		
		for (int i = 0; i < count; ++i)
		{
			const Vec2* thisVert = vertices + i;
			const Vec2* otherVert = vertices + (i + 1) % count;
			Vec2 midPos = (*thisVert + *otherVert) * 0.5f;
			Vec2 offset = *otherVert - *thisVert;
			Vec2 scale = MakeVec2(Vec2Length(offset), 10.0f);
			f32 rot = 90.0f - Vec2ToAngle(offset);
		
			obj->SetPosition(midPos);
			obj->SetRotation(rot);
			obj->SetScale(scale);
		
			obj->Render();
		}
	}
	void Debug_DrawAxes(const Vec2& pos, const Vec2* axes)
	{
		if (!C_debugDoRender)
			return;

		auto obj = ObjectBuilder::CreateSquare();
		obj->SetColorTint(MakeColor(255, 0, 255, 255));
		obj->SetScale(MakeVec2(50.0f, 5.0f));

		for (int i = 0; i < 2; ++i)
		{
			obj->SetColorTint(MakeColor(i ? 0 : 255, i ? 255 : 0, 0, 255));
			obj->SetRotation(90.0f - Vec2ToAngle(axes[i]));
			obj->SetPosition(pos + axes[i] * 20.0f);

			obj->Render();
		}
	}

	void OBBCollider::UpdateVertices(const Transform& thisTransform)
	{
		halfLengths = thisTransform.GetScale() * 0.5f;

		// Reset vertices to axis aligned
		vertices[0] = halfLengths; // Top right
		vertices[1] = MakeVec2(-halfLengths.x, halfLengths.y); // Top left
		vertices[2] = -halfLengths; // Bottom left
		vertices[3] = MakeVec2(halfLengths.x, -halfLengths.y); // Bottom right

		// Rotate the vertices
		Mtx33 rotateMatrix;
		AEMtx33RotDeg(&rotateMatrix, thisTransform.GetRotation());
		AEMtx33MultVecArray(vertices, &rotateMatrix, vertices, 4);

		// Translate the vertices
		for (int i = 0; i < 4; ++i)
			vertices[i] += thisTransform.GetPosition();


		// Reset axes
		axes[0] = MakeVec2(1.0f, 0.0f);
		axes[1] = MakeVec2(0.0f, 1.0f);

		// Rotate the axes
		AEMtx33MultVecArray(axes, &rotateMatrix, axes, 2);
	}

	bool OBBCollider::TestCollision_OBB(const OBBCollider& other, CollisionData* outCollisionData) const
	{
		//Debug_DrawVertices(vertices);
		//Debug_DrawVertices(other.vertices);

		/* Use Separating Axis Theorem to determine if there is a collision.
		   Since both colliders are squares, we only need to check 4 axes. */
		std::vector<CollisionData> collisions;

		for (int i = 0; i < 4; ++i)
		{
			const Vec2& axis = (i < 2 ? axes[i] : other.axes[i - 2]);

			float thisMinExtent = 0.0f, thisMaxExtent = 0.0f, otherMinExtent = 0.0f, otherMaxExtent = 0.0f;
			Collision::CalculateSATExtents_Rect(axis, vertices, &thisMinExtent, &thisMaxExtent);
			Collision::CalculateSATExtents_Rect(axis, other.vertices, &otherMinExtent, &otherMaxExtent);
			
			if (thisMinExtent >= otherMaxExtent ||
				thisMaxExtent <= otherMinExtent)
				return false; // Colliders project a separation on this axis. No collision.

			// Store this collision
			float rightExtent = std::abs(otherMaxExtent - thisMinExtent);
			float leftExtent = std::abs(thisMaxExtent - otherMinExtent);
			if (rightExtent < leftExtent)
				collisions.emplace_back(i < 2 ? this : &other, rightExtent, axis);
			else
				collisions.emplace_back(i < 2 ? this : &other, leftExtent, -axis);
		}

		/* Obtain the collision with the smallest penetration */
		auto smallestCollisionIter = std::min_element(collisions.begin(), collisions.end());
		*outCollisionData = *smallestCollisionIter;

		// Store which colliders are the penetrating and the receiving ones.
		const OBBCollider* receivingCollider = sCast<const OBBCollider*>(outCollisionData->collider);
		const OBBCollider* penetratingCollider = (outCollisionData->collider == this ? &other : this);

		/* Get the midpoint of the edge of collision */
		// Rotate the axis aligned half length so that we can project it onto the normal correctly
		Vec2 rotatedHalfLength = receivingCollider->halfLengths;
		Mtx33 rotateMatrix;
		AEMtx33RotDeg(&rotateMatrix, receivingCollider->transformPtr->GetRotation());
		AEMtx33MultVec(&rotatedHalfLength, &rotateMatrix, &rotatedHalfLength);
		
		Vec2 edgePosition{ receivingCollider->transformPtr->GetPosition() };
		Vec2 vecFromPosToEdge{ outCollisionData->collisionNormal * std::abs(Vec2Dot(rotatedHalfLength, outCollisionData->collisionNormal)) };
		// For some reason the signs are flipped depending on which collider is the receiving collider.
		// I also have no idea why, but at least this fixes it.
		edgePosition += (receivingCollider == this ? -vecFromPosToEdge : vecFromPosToEdge);
		//Debug_DrawVertex(Vec2Zero(), edgePosition, MakeColor(255, 0, 0, 255));

		/* Get the vertex with greatest penetration. This is our collision point. */
		const Vec2* penetratingVertices = penetratingCollider->vertices;
		f32 deepestDist = -FLT_MAX;
		bool isEdgeToEdge = false;
		for (int i = 0; i < 4; ++i, ++penetratingVertices)
		{
			f32 penetrationDist = Vec2Dot(*penetratingVertices - edgePosition, outCollisionData->collisionNormal);
			if (receivingCollider != this)
				penetrationDist = -penetrationDist;
			
			if (std::abs(deepestDist - penetrationDist) <= 1.0f)
			{
				// Multiple vertices are penetrating through an equal distance. This should imply edge to edge.
				isEdgeToEdge = true;
				outCollisionData->collisionPoint = (outCollisionData->collisionPoint + *penetratingVertices) * 0.5f;
			}
			else if (penetrationDist > deepestDist)
			{
				isEdgeToEdge = false;
				deepestDist = penetrationDist;
				outCollisionData->collisionPoint = *penetratingVertices;
			}
		}

		// If there is an edge-to-edge collision, check if either centers of mass are within the edge.
		// If yes, the collision is the projection of the center of mass onto the edge. If not, it's one of the collider corners.
		if (isEdgeToEdge)
		{
			Vec2 orthogonalNormal = Vec2Rot90(outCollisionData->collisionNormal);
			const Vec2& penetratingObjPos = penetratingCollider->transformPtr->GetPosition();
			const Vec2& receivingObjPos = receivingCollider->transformPtr->GetPosition();
			penetratingVertices -= 4;
			const Vec2* receivingVertexes = receivingCollider->vertices; // Vertexes of the collider that's being collided

			// Calculate both objects' extents along the orthogonal normal
			float penetratingObjExtent{ Vec2Dot(penetratingObjPos, orthogonalNormal) };
			float receivingObjExtent{ Vec2Dot(receivingObjPos, orthogonalNormal) };
			float penetratingLeftExtent{}, penetratingRightExtent{};
			float receivingLeftExtent{}, receivingRightExtent{};
			Collision::CalculateSATExtents_Rect(orthogonalNormal, penetratingVertices, &penetratingLeftExtent, &penetratingRightExtent);
			Collision::CalculateSATExtents_Rect(orthogonalNormal, receivingVertexes, &receivingLeftExtent, &receivingRightExtent);

			if (receivingLeftExtent <= penetratingObjExtent && penetratingObjExtent <= receivingRightExtent)
				// Penetrating's center of mass is within the receiving's
				edgePosition += (penetratingObjExtent - receivingObjExtent) * orthogonalNormal;
			else if (penetratingLeftExtent <= receivingObjExtent && receivingObjExtent <= penetratingRightExtent)
				// Receiving's center of mass is within the penetrating's
				edgePosition += (receivingObjExtent - penetratingObjExtent) * orthogonalNormal;
			else
			{
				// Both center of mass is outside. Set collision point to one the receiving collider's corner.
				float edgePosExtent{ Vec2Dot(edgePosition, orthogonalNormal) };
				outCollisionData->collisionPoint += orthogonalNormal *
					((penetratingObjExtent < receivingLeftExtent ? receivingLeftExtent : receivingRightExtent) - edgePosExtent);
			}

			//Debug_DrawVertex(Vec2Zero(), outCollisionData->collisionPoint, MakeColor(0, 0, 255, 255));
		}

		//Debug_DrawVertex(Vec2Zero(), outCollisionData->collisionPoint, MakeColor(0, 255, 0, 255));
		//Debug::AddValue("collisionPoint", outCollisionData->collisionPoint);

		return true;
	}

	bool OBBCollider::TestCollision(const ColliderBase& other, CollisionData* outCollisionData) const
	{
		ColliderBase::TestCollision(other, outCollisionData);

		switch (other.GetType())
		{
		case COLLIDER_OBB:
			return TestCollision_OBB(sCast<const OBBCollider&>(other), outCollisionData);
		case COLLIDER_POINT:
			return other.TestCollision(*this, outCollisionData);
		default:
			Error::RaiseErrFormatted("OBBCollider::TestCollision", "Unimplemented type %d", ERROR_ERR, other.GetType());
			return false;
		}
	}

	bool OBBCollider::TestRaycast(const Ray& ray, RaycastData* outRaycastData) const
	{
		Vec2 rayToThis = transformPtr->GetPosition() - ray.originPoint;

		// There is no collision if we're behind the ray.
		if (Vec2Dot(rayToThis, ray.direction) < 0.0f &&						// Angle check (pointing away from us)
			Vec2LengthSquared(rayToThis) > Vec2LengthSquared(halfLengths))	// Distance check (beyond the maximum bounds of this collider)
			return false;

		UpdateTransformState();

		// Find the distances at which the ray intersects the orthogonal planes of the bounding box
		f32 tMin = 0.0f, tMax = FLT_MAX;
		if (!(Collision::CalculateRay_CollisionOnAxis(axes[0], rayToThis, halfLengths.x, ray.direction, &tMin, &tMax) &&
			  Collision::CalculateRay_CollisionOnAxis(axes[1], rayToThis, halfLengths.y, ray.direction, &tMin, &tMax)))
			return false;

		// If the ray exits a far plane on 1 axis before entering the close plane on the other axis, there is no collision
		if (tMax < tMin)
			return false;

		// Collision occured
		if (tMin < outRaycastData->distance)
		{
			outRaycastData->collider = this;
			outRaycastData->distance = tMin;
		}
		return true;
	}


	CollisionData::CollisionData()
		: collider(nullptr)
		, penetrationDepth(0.0f)
		, collisionNormal(Vec2Zero())
		, collisionPoint(Vec2Zero())
	{
	}
	CollisionData::CollisionData(const ColliderBase* collider, float penetrationDepth, const Vec2& collisionNormal)
		: collider(collider)
		, penetrationDepth(penetrationDepth)
		, collisionNormal(collisionNormal)
		, collisionPoint(Vec2Zero())
	{
	}

	bool CollisionData::operator<(const CollisionData& other) const
	{
		return penetrationDepth < other.penetrationDepth;
	}


	Ray::Ray()
		: originPoint(Vec2Zero())
		, direction(Vec2Zero())
	{
	}
	Ray::Ray(const Vec2& originPoint, const Vec2& direction)
		: originPoint(originPoint)
		, direction(direction)
	{
	}

	RaycastData::RaycastData()
		: collider(nullptr)
		, distance(0.0f)
	{
	}
	RaycastData::RaycastData(const ColliderBase* collider, float distance)
		: collider(collider)
		, distance(distance)
	{
	}

	
	void Collision::CalculateSATExtent_Point(const Vec2& axis, const Vec2& pos, float* minExtent, float* maxExtent)
	{
		float dotVal = Vec2Dot(axis, pos);
		if (dotVal < *minExtent)
			*minExtent = dotVal;
		if (dotVal > *maxExtent)
			*maxExtent = dotVal;
	}

	void Collision::CalculateSATExtents_Rect(const Vec2& axis, const Vec2* vertices, float* minExtent, float* maxExtent)
	{
		*minExtent = FLT_MAX;
		*maxExtent = -FLT_MAX;

		for (int i = 0; i < 4; ++i)
			CalculateSATExtent_Point(axis, vertices[i], minExtent, maxExtent);
	}

	bool Collision::CalculateRay_CollisionOnAxis(const Vec2& axis, const Vec2& posOffset, f32 halfLength, const Vec2& rayDir, f32* outTMin, f32* outTMax)
	{
		f32 rayDirProjAxis = Vec2Dot(rayDir, axis);
		f32 offsetProjAxis = Vec2Dot(posOffset, axis);
		
		f32 tMin, tMax;

		// If the ray is parallel to the planes, its only colliding if it goes through the bounding box.
		// Defer to testing the other axis to find the distance.
		if (std::abs(rayDirProjAxis) <= C_epsilon)
			return (-halfLength <= offsetProjAxis && offsetProjAxis <= halfLength);

		// Find the distances to the bounding box's lines/planes from the ray origin
		tMin = (offsetProjAxis - halfLength) / rayDirProjAxis;
		tMax = (offsetProjAxis + halfLength) / rayDirProjAxis;

		if (tMin > tMax)
			std::swap(tMin, tMax);

		if (*outTMin < tMin)
			*outTMin = tMin;
		if (*outTMax > tMax)
			*outTMax = tMax;
		return true;
	}

}}
