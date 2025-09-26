/******************************************************************************/
/*!
\file   G_GrappleHook.h
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Ryan Chan Kuan Fu (80%)
\par    email: c.kuanfuryan\@digipen.edu
\par    DigiPen login: c.kuanfuryan

\author Matthew Chan Shao Jie (20%)
\par    email: m.chan\@digipen.edu
\par    DigiPen login: m.chan

\brief
  Grapple Hook logic and render for the Player. Uses pure math to get the Player
  to swing on a rope. Also pulls dynamic objects to the Player. Also rotates if
  the dynamic object rotates. Just try throwing a block and catching it with the
  grapple, you'll know what I mean.

  Matthew added some code to distinguish between blocks that are grapple-able
  and blocks that are not. As well as the logic to pick up dynamic blocks that
  reach the player.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#pragma once
#include "pch.h"
#include "E_Object.h"
#include "G_Constants.h"
#include "E_LineRenderer.h"

namespace Game
{
	enum GH_STATUS
	{
		GH_FOLLOWING = 0,
		GH_MOVING,
		GH_ATTACHED
	};

	typedef struct GH_ATTACHMENT
	{
		bool isDynamic;
		Engine::PhysicsObject* obj;
		Vec2 prevPos;
		f32 prevRot;
	} GH_ATTACHMENT;

	class GrappleHook
	{
	private:
		GH_STATUS status;					// Status
		GH_ATTACHMENT attached;				// Struct containing information for attaching the hook to a dynamic PhysicsObject
		sPtr<Engine::PhysicsObject> object;	// Object for transform and render
		sPtr<Engine::LineRenderer> line;	// Rope render
		f32 maxExtension;					// Scalar distance
		bool applyingTension;				// If the rope is changing the player movement in any way
		bool collided;						// Has the rope tip collided with anything this frame?
		bool toMovingFlag;

	public:
		GrappleHook() : status(GH_FOLLOWING), attached(), object(nullptr), line(nullptr), maxExtension(0.0f), applyingTension(false), collided(false), toMovingFlag(false) {};
		~GrappleHook();

		void Init(Vec2 playerPos);
		void Update(Vec2& playerVel, Vec2 playerPos, Vec2& playerForce, Vec2 mousePos);
		void Render();
		void OnCollision(Engine::PhysicsObject& thisObj, Engine::PhysicsObject& otherObj, Engine::Physics::CollisionData& collisionData);
		void ResetAttachment();
		void SetToFollowing(Vec2 playerPos);

		GH_STATUS GetStatus() const;
	};
}