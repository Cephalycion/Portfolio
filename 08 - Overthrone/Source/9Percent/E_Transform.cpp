/******************************************************************************/
/*!
\file   E_Transform.cpp
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Kendrick Sim Hean Guan (100%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\brief
  This is the implementation file for a class that contains a transform matrix.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#include "pch.h"
#include "E_Transform.h"
#include "E_Constants.h"

namespace Engine { namespace Graphics {

	Transform::Transform(f32 posX, f32 posY, f32 rot, f32 scaleX, f32 scaleY)
		: position({ posX, posY })
		, rotation(rot)
		, scale({ scaleX, scaleY })
		, transform() // Don't initialize transform until it's needed
		, isTransformDirty(true)
	{
	}
	Transform::Transform(const Vec2& position, f32 rotation, const Vec2& scale)
		: position(position)
		, rotation(rotation)
		, scale(scale)
		, transform() // Don't initialize transform until it's needed
		, isTransformDirty(true)
	{
	}
	Transform::Transform(const Transform& other)
	{
		*this = other;
	}
	Transform& Transform::operator=(const Transform& other)
	{
		position = other.position;
		rotation = other.rotation;
		scale = other.scale;
		transform = other.transform;
		isTransformDirty = other.isTransformDirty;
		// Only overwrite the callback if the new object has a callback
		if (other.onUpdateCallback)
			onUpdateCallback = other.onUpdateCallback;
		return *this;
	}

	Transform::Transform(Transform&& other) noexcept
	{
		*this = other;
	}
	Transform& Transform::operator=(Transform&& other) noexcept
	{
		position = other.position;
		rotation = other.rotation;
		scale = other.scale;
		transform = other.transform;
		isTransformDirty = other.isTransformDirty;
		onUpdateCallback = other.onUpdateCallback;
		return *this;
	}

	void Transform::UpdateTransform()
	{
		AEMtx33Scale(&transform, scale.x, scale.y);

		Mtx33 nextTransformation;
		AEMtx33RotDeg(&nextTransformation, rotation);
		AEMtx33Concat(&transform, &nextTransformation, &transform);

		AEMtx33TransApply(&transform, &transform, position.x, position.y);

		isTransformDirty = false;
		
		if (onUpdateCallback)
			onUpdateCallback(*this);
	}

	void Transform::SetPosition(const Vec2& newPosition)
	{
		isTransformDirty = true;
		position = newPosition;
	}
	void Transform::AddPosition(const Vec2& offset)
	{
		isTransformDirty = true;
		position += offset;
	}

	void Transform::SetRotation(f32 newRotation)
	{
		isTransformDirty = true;
		rotation = newRotation;
	}
	void Transform::AddRotation(f32 offset, bool clockwise)
	{
		isTransformDirty = true;
		rotation += (clockwise ? offset : -offset);
		rotation -= sCast<int>(rotation / 360.0f) * 360.0f;
	}

	void Transform::SetScale(const Vec2& newScale)
	{
		isTransformDirty = true;
		scale = newScale;
	}
	void Transform::AddScale(const Vec2& offset)
	{
		isTransformDirty = true;
		scale += offset;
	}

	const Vec2& Transform::GetPosition() const
	{
		return position;
	}
	f32 Transform::GetRotation() const
	{
		return rotation;
	}
	const Vec2& Transform::GetScale() const
	{
		return scale;
	}

	const Mtx33& Transform::GetMtx()
	{
		// Brace with if statement to avoid virtual call if possible
		if (isTransformDirty)
			UpdateTransform();
		return transform;
	}

	Transform::operator const Mtx33&()
	{
		return GetMtx();
	}

	f32 (*Transform::GetMtxArrPtr())[3]
	{
		if (isTransformDirty)
			UpdateTransform();
		return transform.m;
	}

	void Transform::SetOnUpdateCallback(const std::function<UpdateCallbackSig>& callback)
	{
		onUpdateCallback = callback;
	}

	void Transform::ForceUpdate()
	{
		if (isTransformDirty)
			UpdateTransform();
	}

	void Transform::Debug_TrackVariables(const std::string& namePrefix)
	{
		Debug::AddTracker(namePrefix + "PosX", &position.x);
		Debug::AddTracker(namePrefix + "PosY", &position.y);
		Debug::AddTracker(namePrefix + "Rot", &rotation);
		Debug::AddTracker(namePrefix + "ScaleX", &scale.x);
		Debug::AddTracker(namePrefix + "ScaleY", &scale.y);
	}
	void Transform::Debug_UntrackVariables()
	{
		Debug::RemoveTracker(&position.x);
		Debug::RemoveTracker(&position.y);
		Debug::RemoveTracker(&rotation);
		Debug::RemoveTracker(&scale.x);
		Debug::RemoveTracker(&scale.y);
	}

}}
