/******************************************************************************/
/*!
\file   G_GrappleHook.cpp
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

#include "pch.h"
#include "G_GrappleHook.h"
#include "G_Block.h"
#include "E_AudioManager.h"
#include "G_Player.h"

namespace Game
{
	void GrappleHook::Init(Vec2 playerPos)
	{
		line = std::make_shared<Engine::LineRenderer>();
		line->Init();
		object = Engine::ObjectBuilder::CreatePhysicsSquare(true, false, Engine::Graphics::Transform(0.0f, 0.0f, 0.0f, C_grappleTipSize, C_grappleTipSize));
		object->SetColorTint(Engine::C_colorRed);
		object->SetPosition(MakeVec2(playerPos.x, playerPos.y + 100.0f));
		status = GH_FOLLOWING;
		maxExtension = 0.0f;
		object->SetOnCollisionCallback(std::bind(&GrappleHook::OnCollision, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
		object->SetGravityEnabled(false);
		object->SetLockRotation(true);
		object->UnsetLayerMask({ Engine::Physics::MASK_ALL });
		object->SetLayerMask({ Engine::Physics::MASK_ENVIRONMENT });
		object->SetRestitutionCoeff(2.0f);
		ResetAttachment();
	}

	void GrappleHook::Update(Vec2& playerVel, Vec2 playerPos, Vec2& playerForce, Vec2 mousePos)
	{
		// LOGIC
		switch (status)
		{
		case GH_FOLLOWING:
		{
			object->SetPosition(playerPos);

			// IF TRIGGERED -> TRANSITION TO MOVING
			if (AEInputCheckTriggered(GRAPPLE_BUTTON))
			{
				object->SetIsDynamic(true);
				Vec2 dir = Vec2Normalize(mousePos - playerPos);
				status = GH_MOVING;
				object->SetVelocity(dir * C_grappleThrowSpeed);
				object->SetLayerMask({ Engine::Physics::MASK_ENVIRONMENT,Engine::Physics::MASK_ENVIRONMENT_DYNAMIC, Engine::Physics::MASK_ENEMY });
				toMovingFlag = true;
			}
			break;
		}
		case GH_MOVING:
		{
			// IF NOT PRESSING -> TRANSITION TO FOLLOWING
			if (!AEInputCheckCurr(GRAPPLE_BUTTON))
			{
				SetToFollowing(playerPos);
				break;
			}

			// IF COLLIDED -> TRANSITION TO ATTACHED
			if (collided)
			{
				// If object collide with something, stop movement
				// and switch state to ATTACHED
				Vec2 rope = object->GetPosition() - playerPos;
				maxExtension = Vec2Length(rope);
				object->SetIsDynamic(false);
				object->SetGravityEnabled(false);
				status = GH_ATTACHED;
				object->UnsetLayerMask({ Engine::Physics::MASK_ALL });
			}
			break;
		}
		case GH_ATTACHED:
		{
			// IF NOT PRESSING OR REACHED HOOK -> TRANSITION TO FOLLOWING
			if (!AEInputCheckCurr(GRAPPLE_BUTTON))
			{
				SetToFollowing(playerPos);
				break;
			}
			attached.isDynamic = attached.obj->physicsComp->GetIsDynamic();

			// GRAPPLE PULL DYNAMIC OBJECT
			Vec2 rope = object->GetPosition() - playerPos;
			Vec2 dir = Vec2Normalize(rope);
			maxExtension = Vec2Length(rope);
			if ((maxExtension <= C_grappleMinExtension))
			{
				PhysBlock* pb = dCast<PhysBlock*>(attached.obj);
				if (attached.isDynamic && pb != nullptr)
				{
					sPtr<Game::Player> p = Engine::Singleton<Game::Player>::Instance();
					if (p->grabbedBlock.object == nullptr)
					{
						p->grabbedBlock.object = std::dynamic_pointer_cast<PhysBlock>(attached.obj->shared_from_this());
						p->grabbedBlock.type = pb->type;
						pb->SetIsDynamic(false);
						pb->UnsetLayerMask({ Engine::Physics::MASK_ALL });
						pb->SetRotation(0.0f);
						Engine::AudioManager::playAudio(Engine::SFX_PICK, false);
						SetToFollowing(playerPos);
					}
				SetToFollowing(playerPos);
				}
				break;
			}

			// HOOK ATTACHMENT
			if (attached.isDynamic && attached.obj)
			{
				// Get the data from attached object
				Vec2 currPos = attached.obj->GetPosition();
				f32 currRot = attached.obj->GetRotation();
				f32 deltaRot = currRot - attached.prevRot;

				// Mtx33 declarations
				Mtx33 translate1{};
				Mtx33 rotate{};
				Mtx33 translate2{};

				// Translate to origin, rotate about attached object and translate back
				AEMtx33Trans(&translate1, -attached.prevPos.x, -attached.prevPos.y);
				AEMtx33RotDeg(&rotate, deltaRot);
				AEMtx33Trans(&translate2, currPos.x, currPos.y);

				// Concatenate matrices
				AEMtx33Concat(&translate1, &rotate, &translate1);
				AEMtx33Concat(&translate1, &translate2, &translate1);

				// Set new position by multiplying vector with matrix
				Vec2 newPos { 0.0f, 0.0f };
				Vec2 objPos = object->GetPosition();
				AEMtx33MultVec(&newPos, &translate1, &objPos);

				// Setting values
				object->SetPosition(newPos);
				object->SetRotation(currRot);
				attached.prevPos = currPos;
				attached.prevRot = currRot;

				// Add velocity to attached object
				attached.obj->ApplyForce(Vec2Normalize(playerPos - attached.obj->GetPosition()) * 10000.0f * attached.obj->GetMass());
				break;
			}

			// GRAPPLE PULL PLAYER
			playerVel += dir * C_grapplePullSpeed * Engine::C_frametime;
			
			// TENSION
			applyingTension = false;
			f32 diff = Vec2Length(rope) - maxExtension;
			Vec2 tensionDir = Vec2Normalize(rope);

			// If the player is further away from the hook than the maximum length of the rope
			if (diff >= 0)
			{
				applyingTension = true;
				Vec2 tensionForce;
				tensionForce = -Vec2Project(playerForce, rope);

				// Add spring-back force
				tensionForce += tensionDir * diff * C_springStiffness;

				/* Method 1: Forces (I don't really understand this code anymore lol)
				// Add centripetal force
				Vec2 perpDir = MakeVec2(-tensionDir.y, tensionDir.x);
				f32 tangentVel = Vec2LengthSquared(Vec2Project(playerVel, perpDir));
				f32 centripetalForce = (tangentVel * playerMass) / Vec2Length(rope);
				tensionForce += (tensionDir * centripetalForce);
				
				// Calculate KE = 0.5 * m * v * v
				f32 kineticEnergy = 0.5f * playerMass * Vec2LengthSquared(playerVel);
				Vec2 velDir = Vec2Normalize(playerVel);
				*/

				/* Method 2: Angle between vectors
				// Angle between 2 vectors = arccosine of (a.b) / (|a||b|)
				if (acos(Vec2Dot(playerVel, rope) / (Vec2Length(playerVel) * Vec2Length(rope))) > (PI / 2)) // If more than 90 degrees
				{
					playerVel -= Vec2Project(playerVel, rope); // Remove the portion of the velocity that is moving away from the rope
				}
				*/

				/* Method 3: Dot product projection */

				// tensionDir = Vec2Normalize(rope);
				f32 result = Vec2Dot(playerVel, tensionDir);
				if (result < 0.0f)
				{
					// Remove the portion of the velocity that is moving away from the rope
					playerVel -= tensionDir * result;
				}

				// Apply tension force
				playerForce += tensionForce;
			}
			break;
		}
		default:
		{
			break;
		}
		}

		// SET COLOR FOR RENDERING
		Engine::Color color;
		switch (status)
		{
		case GH_MOVING:
		{
			color = Engine::C_colorRed;
			break;
		}
		case GH_ATTACHED:
		{
			color = Engine::C_colorGreen;
			break;
		}
		default:
		{
			color = Engine::C_colorWhite;
			break;
		}
		}
		line->SetColor(color);
		line->SetLine(object->GetPosition(), playerPos);
		object->SetColorTint(color);
		collided = false;
	}

	void GrappleHook::Render()
	{
		if (status != GH_FOLLOWING) 
		{
			line->Render(); 
			object->Render();
		}
	}

	void GrappleHook::OnCollision(Engine::PhysicsObject& thisObj,Engine::PhysicsObject& otherObj, Engine::Physics::CollisionData& collisionData)
	{
		UNREFERENCED_PARAMETER(thisObj);
		UNREFERENCED_PARAMETER(collisionData);

		if (status != GH_MOVING) { return; }

		if (toMovingFlag)
		{
			toMovingFlag = false;
			Engine::AudioManager::playAudio(Engine::SFX_HOOK, false);
		}
		
		// Testing the blocktype of collided object
		sPtr<PhysBlock> pb = std::dynamic_pointer_cast<PhysBlock>(otherObj.shared_from_this());
		if (pb != nullptr)
		{
			switch (pb->type)
			{
			case BT_NONGRAPPLE:
			case BT_DOOR:
			case BT_NONE:
				object->SetGravityEnabled(true);
				return;
			}
		}
		else
		{
			object->SetGravityEnabled(true);
			return;
		}

		collided = true;
		attached.obj = &otherObj;
		attached.isDynamic = otherObj.physicsComp->GetIsDynamic();
		attached.prevPos = attached.obj->GetPosition();
		attached.prevRot = attached.obj->GetRotation();
	}

	void GrappleHook::ResetAttachment()
	{
		attached.isDynamic = false;
		attached.obj = nullptr;
		attached.prevPos = MakeVec2(0.0f, 0.0f);
		attached.prevRot = 0.0f;
	}

	void GrappleHook::SetToFollowing(Vec2 playerPos)
	{
		object->SetPosition(playerPos);
		object->SetRotation(0.0f);
		object->SetIsDynamic(false);
		object->SetGravityEnabled(false);
		status = GH_FOLLOWING;
		ResetAttachment();
		object->UnsetLayerMask({ Engine::Physics::MASK_ALL });					

	}

	GH_STATUS GrappleHook::GetStatus() const
	{
		return status;
	}

	GrappleHook::~GrappleHook()
	{
	}
}
