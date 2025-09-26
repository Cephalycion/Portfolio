/******************************************************************************/
/*!
\file   E_Camera.h
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Kendrick Sim Hean Guan (100%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\brief
  This is an interface file for a camera that has position, rotation
  and zoom, as well as a camera manager.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#pragma once
#include "pch.h"
#include "E_Transform.h"
#include "E_Constants.h"

namespace Engine { namespace Graphics {

	class Transform_Camera : public Transform
	{
	protected:
		virtual void UpdateTransform() override;

	public:
		// Replace constructors with "zoom" versions instead of "scale"
		Transform_Camera(f32 posX, f32 posY, f32 rot, f32 scaleX, f32 scaleY) = delete;
		Transform_Camera(const Vec2& position, f32 rotation, const Vec2& scale) = delete;
		Transform_Camera(f32 posX, f32 posY, f32 rot, f32 zoom);
		Transform_Camera(const Vec2& position, f32 rotation, f32 zoom);

		/* Debug */
		virtual void Debug_TrackVariables(const std::string& namePrefix) override;
		virtual void Debug_UntrackVariables() override;
		
	};

	class Camera
	{
	protected:
		Transform_Camera transform;

	public:
		Camera(f32 posX = 0.0f, f32 posY = 0.0f, f32 rot = 0.0f, f32 zoom = 1.0f);

		void SetPosition(const Vec2& newPosition);
		void AddPosition(const Vec2& offset);
		void SetRotation(f32 newRotation);
		void AddRotation(f32 offset, bool clockwise = true);
		void SetZoom(const f32 newZoom);
		void AddZoom(const f32 offset);

		const Vec2& GetPosition() const;
		f32 GetRotation() const;
		f32 GetZoom() const;

		// Apply camera transformation onto an object transformation
		Mtx33* ApplyTransform(Mtx33* transformPtr);

		/* Debug */
		void Debug_TrackVariables(const std::string& namePrefix);
		void Debug_UntrackVariables();

	};

	// TO BE USED AS SINGLETON.
	class CameraManager
	{
	protected:
		sPtr<Camera> activeCam;

		// Store transformation results in this mtx, to avoid recreating an mtx for each transform application call
		Mtx33 transformBuffer;

	public:
		CameraManager();

		void SetActiveCam(const sPtr<Camera>& camera);
		sPtr<const Camera> GetActiveCam() const;
		void ResetActiveCam();

		Mtx33* ApplyTransform(const Mtx33& transform);

	};

}}
