/******************************************************************************/
/*!
\file   G_Physics.cpp
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Kendrick Sim Hean Guan (100%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\brief
  This is the implementation file for a physics manager with optimizations
  that can only work for this game.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#include "pch.h"
#include "G_Physics.h"
#include "G_Map.h"

namespace Game {

	using namespace Engine;
	using namespace Engine::Physics;

	void Game::PhysicsManager::PruneDeadComps()
	{
		Engine::Physics::PhysicsManager::PruneDeadComps();

		for (auto iter = specialStatics.begin(); iter != specialStatics.end(); )
			if (iter->expired())
				iter = specialStatics.erase(iter);
			else
				++iter;
	}

	void Game::PhysicsManager::CheckCollisionBetweenComponents(
		std::queue<CollisionNotifyEvent>* collisionEvents, std::queue<SettleEvent>* settleEvents)
	{
		CollisionData collisionData{};

		// Dynamic vs Dynamic
		for (auto thisComp = dynamicComps.begin(), endIter = dynamicComps.end(); thisComp != endIter; ++thisComp)
			for (auto otherComp = std::next(thisComp); otherComp != endIter; ++otherComp)
				CheckCollision(*thisComp, *otherComp, true, &collisionData, collisionEvents, settleEvents);

		// Dynamic vs Special Static
		for (auto& dynComp : dynamicComps)
			for (auto& specialStaticComp : specialStatics)
				CheckCollision(dynComp, specialStaticComp.lock(), false, &collisionData, collisionEvents, settleEvents);

		// Dynamic vs Static
		for (auto& dynComp : dynamicComps)
		{
			// Convert to map pos
			Coordinate dynPos = Singleton<Map>::Instance()->WorldToMapPos(dynComp->GetTransform()->GetPosition());

			// For each block in a 3x3 around this dynamic comp
			for (int x = dynPos.x - 1; x <= dynPos.x + 1; ++x)
				for (int y = dynPos.y - 1; y <= dynPos.y + 1; ++y)
				{
					// Get the block at the index
					Block* block = Singleton<Map>::Instance()->GetBlockAtIndex(x, y);
					if (!(block && block->object))
						continue;

					// Make sure it has a physics comp
					auto wPtrStaticComp = block->object->GetPhysicsCompWeak();
					if (wPtrStaticComp.expired())
						continue;

					// Check collision with it
					CheckCollision(dynComp, wPtrStaticComp.lock(), false, &collisionData, collisionEvents, settleEvents);
				}
		}
			
	}

	void Game::PhysicsManager::RegisterSpecialStatic(const sPtr<Engine::Physics::PhysicsComponent>& physComp)
	{
		specialStatics.emplace_back(physComp);
	}
	void PhysicsManager::UnregisterSpecialStatic(const sPtr<PhysicsComponent>& physComp)
	{
		// Manual search as weak_ptr doesn't implement operator==
		for (auto iter = specialStatics.begin(), end = specialStatics.end(); iter != end; ++iter)
			if (iter->lock() == physComp)
			{
				specialStatics.erase(iter);
				return;
			}
		Error::RaiseErr("Game::PhysicsManager::UnregisterSpecialStatic", "Failed to find physics component!");
	}

	void Game::PhysicsManager::UpdateParticlePhysics(const sPtr<PhysicsComponent>& comp, float dt)
	{
		Engine::Physics::PhysicsManager::UpdateParticlePhysics(comp, dt);

		// Check if the particle is still inside a block. If it is, it's squished between 2 blocks.
		// So to fix that, push it up 1 block.

		// Get the block and check if it's solid
		const Vec2& particlePos = comp->GetTransform()->GetPosition();
		Game::Block* block = Singleton<Map>::Instance()->GetBlockAtIndex(particlePos);
		if (!block || block->GetPhysBlockWeak().expired())
			return;

		// Push particle up
		Coordinate newCoord = Singleton<Map>::Instance()->WorldToMapPos(particlePos);
		--newCoord.y;
		Vec2 newPos = Singleton<Map>::Instance()->MapToWorldPos(newCoord);
		newPos.x = particlePos.x; // We don't want to snap the particle's x to the center of the block.
		newPos.y -= C_blockLengthHalf; // MapToWorldPos gives us the center of the block. We want to end up at the bottom.
		comp->ResolveCollision_PushOut(newPos - particlePos);
	}

}
