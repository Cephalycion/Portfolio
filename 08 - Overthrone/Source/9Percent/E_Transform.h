/******************************************************************************/
/*!
\file   E_Transform.h
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Kendrick Sim Hean Guan (100%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\brief
  This is an interface file for a class that contains a transform matrix.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#pragma once
#include "pch.h"

namespace Engine { namespace Graphics {

	class Transform
	{
		using UpdateCallbackSig = void(const Transform& thisTransform);

	protected:
		Vec2 position;
		f32 rotation;
		Vec2 scale;

		Mtx33 transform;
		bool isTransformDirty;

		std::function<UpdateCallbackSig> onUpdateCallback;

		virtual void UpdateTransform();

	public:
		Transform(f32 posX = 0.0f, f32 posY = 0.0f, f32 rot = 0.0f, f32 scaleX = 100.0f, f32 scaleY = 100.0f);
		Transform(const Vec2& position, f32 rotation, const Vec2& scale);
		Transform(const Transform& other);
		Transform& operator=(const Transform& other);
		Transform(Transform&& other) noexcept;
		Transform& operator=(Transform&& other) noexcept;

		void SetPosition(const Vec2& newPosition);
		void AddPosition(const Vec2& offset);
		void SetRotation(f32 newRotation);
		void AddRotation(f32 offset, bool clockwise = true);
		void SetScale(const Vec2& newScale);
		void AddScale(const Vec2& offset);

		const Vec2& GetPosition() const;
		f32 GetRotation() const;
		const Vec2& GetScale() const;

		const Mtx33& GetMtx();
		operator const Mtx33&();
		// For compatibility with AEEngine
		f32 (*GetMtxArrPtr())[3];

		// Set a callback whenever the Mtx33 is updated (for updating external variables that are dependent on this transform)
		void SetOnUpdateCallback(const std::function<UpdateCallbackSig>& callback);
		// Force an Mtx33 update (perhaps to fire off the callback)
		void ForceUpdate();

		/* Debug */
		virtual void Debug_TrackVariables(const std::string& namePrefix);
		virtual void Debug_UntrackVariables();

	};

}}
