/******************************************************************************/
/*!
\file   G_Physics.h
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Kendrick Sim Hean Guan (100%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\brief
  This is an interface file for a physics manager with optimizations
  that can only work for this game.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#pragma once
#include "pch.h"
#include "E_Physics.h"

namespace Game {

	using namespace Engine::Physics;

	class PhysicsManager : public Engine::Physics::PhysicsManager
	{
	protected:
		// Override the culling of these statics.
		std::vector<wPtr<PhysicsComponent>> specialStatics;

		virtual void PruneDeadComps() override;

		virtual void CheckCollisionBetweenComponents(
			std::queue<CollisionNotifyEvent>* collisionEvents, std::queue<SettleEvent>* settleEvents) override;

	public:
		void RegisterSpecialStatic(const sPtr<PhysicsComponent>& physComp);
		void UnregisterSpecialStatic(const sPtr<PhysicsComponent>& physComp);

		virtual void UpdateParticlePhysics(const sPtr<PhysicsComponent>& comp, float dt) override;

	};

}
