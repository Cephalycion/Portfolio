/******************************************************************************/
/*!
\file   E_Camera.cpp
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Kendrick Sim Hean Guan (100%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\brief
  This is the implementation file for a camera that has position, rotation
  and zoom, as well as a camera manager.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#include "pch.h"
#include "E_Camera.h"

namespace Engine { namespace Graphics {

	void Transform_Camera::UpdateTransform()
	{
		// Reverse the order of the transformations as we're dealing with the canvas

		AEMtx33Trans(&transform, -position.x, -position.y);

		Mtx33 nextTransformation;
		AEMtx33RotDeg(&nextTransformation, rotation);
		AEMtx33Concat(&transform, &nextTransformation, &transform);

		AEMtx33Scale(&nextTransformation, scale.x, scale.y);
		AEMtx33Concat(&transform, &nextTransformation, &transform);

		isTransformDirty = false;
	}

	Transform_Camera::Transform_Camera(f32 posX, f32 posY, f32 rot, f32 zoom)
		: Transform(posX, posY, rot, zoom, zoom)
	{
	}
	Transform_Camera::Transform_Camera(const Vec2& position, f32 rotation, f32 zoom)
		: Transform(position, rotation, MakeVec2(zoom, zoom))
	{
	}

	void Transform_Camera::Debug_TrackVariables(const std::string& namePrefix)
	{
		Debug::AddTracker(namePrefix + "PosX", &position.x);
		Debug::AddTracker(namePrefix + "PosY", &position.y);
		Debug::AddTracker(namePrefix + "Rot", &rotation);
		Debug::AddTracker(namePrefix + "Zoom", &scale.x);
	}
	void Transform_Camera::Debug_UntrackVariables()
	{
		Debug::RemoveTracker(&position.x);
		Debug::RemoveTracker(&position.y);
		Debug::RemoveTracker(&rotation);
		Debug::RemoveTracker(&scale.x);
	}

	Camera::Camera(f32 posX, f32 posY, f32 rot, f32 zoom)
		: transform(posX, posY, rot, zoom)
	{
	}

	void Camera::SetPosition(const Vec2& newPosition)
	{
		transform.SetPosition(newPosition);
	}
	void Camera::AddPosition(const Vec2& offset)
	{
		transform.AddPosition(offset);
	}

	void Camera::SetRotation(f32 newRotation)
	{
		transform.SetRotation(newRotation);
	}
	void Camera::AddRotation(f32 offset, bool clockwise)
	{
		transform.AddRotation(offset, clockwise);
	}

	void Camera::SetZoom(const f32 newZoom)
	{
		transform.SetScale(MakeVec2(newZoom, newZoom));
	}
	void Camera::AddZoom(const f32 offset)
	{
		transform.AddScale(MakeVec2(offset, offset));
	}

	const Vec2& Camera::GetPosition() const
	{
		return transform.GetPosition();
	}
	f32 Camera::GetRotation() const
	{
		return transform.GetRotation();
	}
	f32 Camera::GetZoom() const
	{
		return transform.GetScale().x;
	}

	Mtx33* Camera::ApplyTransform(Mtx33* transformPtr)
	{
		AEMtx33Concat(transformPtr, cCast<Mtx33*>(&transform.GetMtx()), transformPtr);
		return transformPtr;
	}

	void Camera::Debug_TrackVariables(const std::string& namePrefix)
	{
		transform.Debug_TrackVariables(namePrefix + "Cam");
	}
	void Camera::Debug_UntrackVariables()
	{
		transform.Debug_UntrackVariables();
	}


	CameraManager::CameraManager()
		: activeCam(nullptr)
		, transformBuffer()
	{
		AEMtx33Identity(&transformBuffer);
	}

	void CameraManager::SetActiveCam(const sPtr<Camera>& camera)
	{
		Singleton<CameraManager>::Instance()->activeCam = camera;
	}
	sPtr<const Camera> CameraManager::GetActiveCam() const
	{
		return activeCam;
	}
	void CameraManager::ResetActiveCam()
	{
		Singleton<CameraManager>::Instance()->activeCam.reset();
	}

	Mtx33* CameraManager::ApplyTransform(const Mtx33& transform)
	{
		auto camManager = Singleton<CameraManager>::Instance();
		if (!camManager->activeCam)
			return cCast<Mtx33*>(&transform);

		camManager->transformBuffer = transform;
		return camManager->activeCam->ApplyTransform(&camManager->transformBuffer);
	}

}}
