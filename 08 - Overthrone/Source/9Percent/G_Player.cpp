/******************************************************************************/
/*!
\file   G_Player.cpp
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Matthew Chan Shao Jie (40%)
\par    email: m.chan\@digipen.edu
\par    DigiPen login: m.chan

\author Marc Alviz Evangelista (25%)
\par    email: marcalviz.e\@digipen.edu
\par    DigiPen login: marcalviz.e

\author Ryan Chan Kuan Fu (25%)
\par    email: c.kuanfuryan\@digipen.edu
\par    DigiPen login: c.kuanfuryan

\author Kendrick Sim Hean Guan (10%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\brief
  This is the implementation file for all member functions of a class called 
  Player, along with different utility functions for players.
  This file also includes the Nuke function.

All content � 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#include "pch.h"
#include "E_Asset.h"
#include "E_Physics.h"
#include "G_Player.h"
#include "E_AudioManager.h"
#include "E_IOManager.h"
#include "G_GameState.h"
#include "G_LevelObjects.h"
#include "G_DoorBlock.h"
#include "G_ButtonBlock.h"
#include "G_PlayerAnimationTree.h"
#include "G_TextLayer.h"

namespace Game
{
	void Nuke(sPtr<Map> mapReference)
	{
		// Nuke door blocks, remove button blocks
		Singleton<LevelObjectsManager>::Instance()->RunMacro(OBJECTS_LAYER_FOREGROUND, [](const sPtr<Object>& obj) -> bool {
			sPtr<DoorBlock> doorPtr = std::dynamic_pointer_cast<DoorBlock>(obj);
			if (doorPtr)
			{
				if (doorPtr->isOpen)
					return false;
				else
				{
					doorPtr->SetIsDynamic(true);
					doorPtr->SetVelocity(Vec2Zero());
					doorPtr->SetAngVelocity(0.0f);
					Singleton<Game::PhysicsManager>::Instance()->UnregisterSpecialStatic(doorPtr->GetPhysicsCompWeak().lock());
					return true;
				}
			}

			sPtr<ButtonBlock> buttonPtr = std::dynamic_pointer_cast<ButtonBlock>(obj);
			if (buttonPtr)
				return false;

			return true;
		});

		const Vec2& camPos = Singleton<Camera>::Instance()->GetPosition();
		Coordinate camCoord = { sCast<s32>(camPos.x / C_blockLength), sCast<s32>(-camPos.y / C_blockLength) };
		// Get the number of blocks along the length of the screen
		int numBlocksX = sCast<s32>(std::round(sCast<f32>(C_windowWidth) / C_blockLength));
		int numBlocksY = sCast<s32>(std::round(C_windowHeight / C_blockLength));

		for (int x = camCoord.x; x < camCoord.x + numBlocksX; ++x)
		{
			for (int y = camCoord.y; y < camCoord.y + numBlocksY; ++y)
			{
				Block* b = mapReference->GetBlockAtIndex(sCast<s32>(x), sCast<s32>(y));
				if (b != nullptr)
				{
					if (b->object != nullptr)
					{
						b->object->SetIsDynamic(true);
						b->object->type = b->type;
						b->type = BT_NONE;
						Engine::Singleton<LevelObjectsManager>::Instance()->AddObject(OBJECTS_LAYER_FOREGROUND, b->object);
						b->object->SetVelocity(MakeVec2(AERandFloat() * 500.0f - 250.0f, AERandFloat() * 500.0f - 250.0f));
						b->object->UnsetLayerMask({ Engine::Physics::MASK_ALL });
						b->object = nullptr;
					}
				}
			}
		}
	}

	void Player::TakeDamage(f32 damage)
	{
		// If player is not invincible, decrease health
		if (isInvincible <= 0.0f)
		{
			f32 val = health - damage;
			/* Hacks: Invincible */
			if (!C_hackInvincible)
				health = val > 0.0f ? val : 0.0f;
			isInvincible = C_invincibilityTimer;

			std::cout << "Health is: " << health << '\n';
			healthDisplay->OnChange();

			// If health is zero, die.
  			if (health <= 0.0f)
			{
				Die();
			}
			Engine::AudioManager::playAudio(Engine::SFX_PLAYERHURT, false);
		}
	}
	
	void Player::Die()
	{
		// Can't die if we've completed the level.
		if (currentState == S_COMPLETE)
			return;

		if (currentState != S_DEAD)
		{
			currentState = S_DEAD;
			SetLockRotation(false);
			SetAngVelocity(GetAngVelocity() + 10.0f);
			SetFrictionCoeff(0.5f);
			SetRestitutionCoeff(0.5f);
			abilityname = "";

			Engine::Singleton<TextLayerManager>::Instance()->AddObject(TEXT_LAYER_SCREEN_OVERLAY, TextBuilder::CreateText("You Died!", 1.0f, MakeVec2(0.0f, 200.0f), C_colorWhite, true));
			Engine::Singleton<TextLayerManager>::Instance()->AddObject(TEXT_LAYER_SCREEN_OVERLAY, TextBuilder::CreateText("Press Left Mouse to respawn", 0.3f, MakeVec2(0.0f, 50.0f), C_colorWhite, true));
		}
		if (abilityname.length() >0)
			abilityname += " ";
		if (abilityname.length() < 29)
			abilityname += "ow";

		if (grabbedBlock.object)
		{
			grabbedBlock.object->SetIsDynamic(true);
			Engine::Singleton<LevelObjectsManager>::Instance()->AddObject(OBJECTS_LAYER_FOREGROUND, grabbedBlock.object);
			grabbedBlock.object->UnsetLayerMask({ Engine::Physics::MASK_ALL });
			grabbedBlock.object->SetLayerMask({ Engine::Physics::MASK_ENVIRONMENT_DYNAMIC });

			grabbedBlock.object = nullptr;
			grabbedBlock.type = BT_NONE;
		}
	}
	void Player::Respawn()
	{
		health = C_maxPlayerHealth;
		sPtr<Map> map = Engine::Singleton<Map>::Instance();
		Block* b = map->GetBlockOfType(BT_PLAYER);
		if (b)
		{
			SetPosition(map->MapToWorldPos(b->x, b->y));
			pos = GetPosition();
			vel = MakeVec2(0.0f, 0.0f);
			SetVelocity(vel);
			SetLockRotation(true);
			SetRotation(0.0f);
			currentState = S_ALIVE;
			SetFrictionCoeff(0.0f);
			SetRestitutionCoeff(0.0f);
		}
		Engine::Singleton<TextLayerManager>::Instance()->DeleteLayer(TEXT_LAYER_SCREEN_OVERLAY);
	}

	Player::Player()
		: PhysicsObject(
			true,									// Register to System
			true,									// Is Dynamic
			Engine::Graphics::Transform(),			// Transform
			Engine::AssetManager::GetMeshSquare(),	// Mesh
			nullptr,								// Texture
			1.0f,									// Transparency
			Engine::Graphics::Color()				// Color
		)
		, currentBlock(nullptr)
		, highlightedBlock(nullptr)
		, currentState(S_ALIVE)
		, xindex(0)
		, yindex(0)
		, mx(0)
		, my(0)
		, vel(Vec2Zero())
		, force(Vec2Zero())
		, isGrounded(false)
		, isSwimming(false)
		, dashAvailable(false)
		, radiusPingTransparency(0.0f)
	{
		SetLockRotation(true);

		SetFrictionCoeff(0.0f);
		SetRestitutionCoeff(0.0f);

		//Phys Link
		UnsetLayerMask({ Engine::Physics::MASK_ALL });
		SetLayerMask({ Engine::Physics::MASK_PLAYER, Engine::Physics::MASK_PROJECTILE });
		SetOnCollisionCallback(std::bind(&Player::OnCollision, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

		SetScale(C_playerCollisionScale);
		
		// Grapple Hook Initialisation
		grapple = std::make_shared<GrappleHook>();
		grapple->Init(MakeVec2(0.0f,0.0f));
		grapple->SetToFollowing(pos);

		// Arc Predictor Initialisation
		arcPredictor = std::make_shared<ArcPredictor>();
		arcPredictor->Init();

		radiusIndicator = new Object();
		radiusIndicator->SetMesh(Engine::AssetManager::GetMeshCircle(36));

		// Dash Indicator Initialisation
		dashIndicator = Engine::ObjectBuilder::CreateSquare(
			Engine::Graphics::Transform(0.0f, 0.0f, 0.0f, 100.0f, 70.0f), 
			Engine::SpriteBuilder::CreateSpriteSquare(Engine::IOManager::GetFilePath(Engine::PNG_ARROW)));
			
		// Grab Display Initialisation
		grabDisplay = std::make_shared<GrabDisplay>();
		grabDisplay->Init();

		// Health Initialisation
		health = C_maxPlayerHealth;
		isInvincible = 0.0f;
		healthDisplay = std::make_shared<HealthDisplay>();
		Vec2 startPos = MakeVec2(GetPosition().x, GetPosition().y) + C_healthPosStart;
		healthDisplay->Init(C_playerCollisionScale.x, C_playerCollisionScale.y, startPos);

		// Other
		grabRadius *= C_blockLength;
		playerAnimInit = false;
		initialPos = pos;
	}

	bool IsCollidingAABB(Vec2 apos, Vec2 asize, Vec2 bpos, Vec2 bsize)
	{
		float aminX = apos.x - asize.x / 2;
		float amaxX = apos.x + asize.x / 2;
		float aminY = apos.y - asize.y / 2;
		float amaxY = apos.y + asize.y / 2;
		float bminX = bpos.x - bsize.x / 2;
		float bmaxX = bpos.x + bsize.x / 2;
		float bminY = bpos.y - bsize.y / 2;
		float bmaxY = bpos.y + bsize.y / 2;
		return (
			aminX <= bmaxX &&
			amaxX >= bminX &&
			aminY <= bmaxY &&
			amaxY >= bminY
			);
	}

	void Player::OnNewLevelLoad()
	{
		// The block highlighted by raycasting is probably no longer alive.
		highlightedBlock = nullptr;
	}

	void Player::Update()
	{
		animationFSM.Update();
		spritePtr->Update(Engine::C_frametime);
		if (isInvincible > 0.0f)
		{
			isInvincible -= Engine::C_frametime;
		}
		pos = GetPosition();
		vel = GetVelocity();
		sPtr<Map> mapReference = Engine::Singleton<Map>::Instance();
		f32 leftlimit = -sCast<f32>(Engine::C_windowWidth) / 2.0f;
		f32 rightlimit = ((sCast<f32>(mapReference->width) - 1.0f) * C_blockLength) - sCast<f32>(Engine::C_windowWidth) / 2.0f;
		f32 lowerlimit = -(((sCast<f32>(mapReference->height) - 1.0f) * C_blockLength) - sCast<f32>(Engine::C_windowHeight) / 2.0f) - C_blockLength;

		/* Hack: Invincible */
		if (AEInputCheckTriggered(AEVK_8))
			C_hackInvincible = !C_hackInvincible;
		/* Hacks: Noclip */
		if (AEInputCheckTriggered(AEVK_9))
		{
			C_hackNoclip = !C_hackNoclip;
			SetIsDynamic(!C_hackNoclip);
		}
		if (C_hackNoclip)
		{
			if (AEInputCheckCurr(AEVK_W))
				pos += MakeVec2(0.0f, 10.0f);
			if (AEInputCheckCurr(AEVK_S))
				pos += MakeVec2(0.0f, -10.0f);
			if (AEInputCheckCurr(AEVK_A))
				pos += MakeVec2(-10.0f, 0.0f);
			if (AEInputCheckCurr(AEVK_D))
				pos += MakeVec2(10.0f, 0.0f);
		}
		/* Hacks: Nuke */
		if (AEInputCheckTriggered(AEVK_N))
			Nuke(mapReference);

		Coordinate index = mapReference->WorldToMapPos(pos.x, pos.y);
		xindex = index.x;
		yindex = index.y;
		currentBlock = mapReference->GetBlockAtIndex(xindex, yindex);

		if (currentBlock != nullptr)
		{
			if (currentBlock->type == BT_END)
			{
				Engine::Singleton<GameStateManager>::Instance()->SetIsLevelClear(true);
				Engine::Singleton<TextLayerManager>::Instance()->AddObject(TEXT_LAYER_SCREEN_OVERLAY, TextBuilder::CreateText("Congratulations!", 1.0f, MakeVec2(0.0f, 200.0f), C_colorWhite, true));
				Engine::Singleton<TextLayerManager>::Instance()->AddObject(TEXT_LAYER_SCREEN_OVERLAY, TextBuilder::CreateText(
					std::string("Press Left Mouse to go to ") + (mapReference->currLevel >= C_levelLast ? "continue" : "the next level"), 0.3f, MakeVec2(0.0f, 50.0f), C_colorWhite, true));
				Nuke(mapReference);
				currentState = S_COMPLETE;
				if (grabbedBlock.object != nullptr)
				{
					grabbedBlock.object.reset();
				}
			}
			isSwimming = currentBlock->type == BT_WATER || currentBlock->type == BT_LAVA;
			if (currentBlock->type == BT_LAVA)
			{
				TakeDamage(20.0f);
			}
		}
		index = mapReference->WorldToMapPos(pos.x, pos.y - C_playerCollisionScale.y / 2);
		Block* floorblock = mapReference->GetBlockAtIndex(xindex, sCast<s32>(index.y));
		if (floorblock != nullptr)
		{
			isSwimming |= floorblock->type == BT_WATER || floorblock->type == BT_LAVA;
			if (floorblock->type == BT_LAVA)
			{
				TakeDamage(20.0f);
			}
		}


		if (pos.x < leftlimit + C_playerCollisionScale.x / 2)//Left
		{
			pos.x = leftlimit + C_playerCollisionScale.x / 2;
			vel.x = 0;
		}
		if (pos.x > rightlimit - C_playerCollisionScale.x / 2)//Right
		{
			pos.x = rightlimit - C_playerCollisionScale.x / 2;
			vel.x = 0;
		}
		if (pos.y < lowerlimit + C_playerCollisionScale.y / 2)//Bottom
		{
			pos.y = lowerlimit + C_playerCollisionScale.y / 2;
			vel.y = 0;
			isGrounded = true;
		}
		if (currentState != S_DEAD)
		{// Movement
			SetRotation(0.0f);
			if (AEInputCheckCurr(AEVK_SPACE) && isGrounded)
			{
				AddForce(MakeVec2(0.0f, 1.0f), 550.0f * GetMass());
				vel.y = 0;
			}
			else if (AEInputCheckCurr(AEVK_SPACE) && isSwimming)
			{
				AddForce(MakeVec2(0.0f, 1.0f), 60.0f * GetMass());
			}
			if (AEInputCheckCurr(AEVK_S) && isSwimming)
			{
				AddForce(MakeVec2(0.0f, -1.0f), 20.0f * GetMass());
			}
			f32 laccel= isGrounded ? defaultAccel  : (isSwimming ? swimAccel : airAccel);
			f32 raccel= isGrounded ? defaultAccel  : (isSwimming ? swimAccel : airAccel);

			if (vel.x < 0)
				laccel = laccel * pow((-vel.x / laccel +1),-0.5f)-4;
			else if (vel.x > 0)
				raccel = raccel * pow((vel.x / raccel +1),-0.5f)-4;

			laccel = laccel > 0.0f ? laccel : 0.0f;
			raccel = raccel > 0.0f ? raccel : 0.0f;

			if (AEInputCheckCurr(AEVK_D))
				AddForce(MakeVec2(1.0f, 0.0f), raccel * GetMass());
			if (AEInputCheckCurr(AEVK_A))
				AddForce(MakeVec2(-1.0f, 0.0f), laccel * GetMass());

			if (AEInputCheckTriggered(DASH_BUTTON))
			{
				chargingDash = true;
			}
			Vec2 accel = force / GetMass();
			vel += accel;
		}
		if (isSwimming)
		{
			vel *= 0.88f;
		}
		// Apply force as acceleration
		force = MakeVec2(0.0f, 0.0f);

		Vec2 campos = MakeVec2(
			pos.x > 0 ? (pos.x < rightlimit - Engine::C_windowWidth / 2 ? pos.x : rightlimit - Engine::C_windowWidth / 2) : 0,
			pos.y < 0 ? (pos.y > lowerlimit + Engine::C_windowHeight / 2 ? pos.y : lowerlimit + Engine::C_windowHeight / 2) : 0
		);
		Vec2 tpos = MakeVec2(
			pos.x > 0.0f ? (pos.x < rightlimit - Engine::C_windowWidth / 2 ? 0.0f : pos.x - (rightlimit - Engine::C_windowWidth / 2)) : pos.x
			,
			(pos.y > 0.0f ? pos.y : (pos.y < lowerlimit + Engine::C_windowHeight / 2 ? pos.y - (lowerlimit + Engine::C_windowHeight / 2) : 0.0f)) + C_blockLength
		);
		Debug::AddValue("Player:Y", pos.y);
		Debug::AddValue("Player:TY", tpos.y);
		//Move cam
		if (!C_hackFreecam && !Singleton<GameStateManager>::Instance()->GetIsLevelClear())
			camReference->SetPosition(campos);
		//Ground Friction
		if (isGrounded)
		{
			if (floorblock)
			{
				switch (floorblock->type)
				{
				case BT_ICE:
					break;
				default:
					if (Vec2LengthSquared(vel) > 0)
					{
						float len = sqrt(vel.x * vel.x + vel.y * vel.y);
						Vec2 dir = vel / len;
						float friction = len * 0.1f < 1.0f ? 1.0f : len * 0.1f;
						AddForce(-dir, (friction > len ? len : friction) * GetMass());
					}
					break;
				}
			}
			dashAvailable = dashTime <= 0.0f ? 1 : -1;
		}
	

		//Ability cycling
		if (AEInputCheckTriggered(AEVK_K))
		{
			TakeDamage(2000.0f);
		}
		AEInputGetCursorPosition(&mx, &my);
		Vec2 wmousepos = MakeVec2(sCast<f32>(mx - Engine::C_windowWidth / 2), sCast<f32>(-my + Engine::C_windowHeight / 2)) + campos;

		if (highlightedBlock != nullptr && highlightedBlock->object != nullptr)
			highlightedBlock->object->SetIsHighlighted(false);
		// Raycast display
		Engine::Physics::PhysicsRaycastData rcd;

		switch (currentState)
		{
		case S_ALIVE:
		{
			//GRAB
			if (grabbedBlock.object == nullptr)
			{
				if (Engine::Physics::PhysicsManager::Raycast(pos, Vec2Normalize(wmousepos - pos), { Engine::Physics::MASK_ENVIRONMENT }, &rcd))
				{
					grabDisplay->RenderThisFrame(true);
					grabDisplay->SetColor(Engine::MakeColor(255, 0, 0, 255));
					f32 len = rcd.distance < grabRadius ? rcd.distance : grabRadius;
					Vec2 dir = Vec2Normalize(rcd.collisionPoint - pos);
					grabDisplay->SetDisplay(pos, pos + dir * len, 5.0f);

					if (rcd.distance < grabRadius)
					{
						///Raycasted method
						if (rcd.distance <= grabRadius && rcd.hitObj->GetIsDynamic() == false)
						{
							PhysBlock* block = dCast<PhysBlock*>(rcd.hitObj->GetPhysObject());
							if (block != nullptr)
							{
								Coordinate c = mapReference->WorldToMapPos(block->GetPosition() - MakeVec2(1.0f, -1.0f));
								Block* b = mapReference->GetBlockAtIndex(c.x, c.y);
								if (b != nullptr && Block::GetBlockPickable(b->type))
								{
									// If the block is pickable, we want the grabDisplay to show it
									grabDisplay->SetColor(Engine::MakeColor(0, 255, 0, 255));

									highlightedBlock = b;
									Engine::Debug::AddValue("BlockHit", rcd.hitObj->GetPhysObject()->GetPosition());
									Engine::Debug::AddValue("CollisionPoint", rcd.collisionPoint);
									block->SetIsHighlighted(true);
									if (AEInputCheckTriggered(GRAB_BUTTON))
									{
										grabDisplay->RenderThisFrame(false);
										grabbedBlock.type = b->type;
										grabbedBlock.object = b->object;
										grabbedBlock.object->UnsetLayerMask({ Engine::Physics::MASK_ALL });
										b->object->SetIsHighlighted(false);
										mapReference->UnsetBlock(b);
										Engine::AudioManager::playAudio(Engine::SFX_PICK, false);
									}
								}
							}
						}
					}
				}
				else
				{
					if (AEInputCheckTriggered(GRAB_BUTTON))
					{
						radiusPingTransparency = 0.5f;
					}
					if (highlightedBlock != nullptr && highlightedBlock->object != nullptr)
						highlightedBlock->object->SetIsHighlighted(false);
					highlightedBlock = nullptr;
				}
			}
			else
			{
				// Update ArcPredictor
				sPtr<PhysBlock> po = sCast<sPtr<PhysBlock>>(grabbedBlock.object);
				Vec2 dir = MakeVec2(wmousepos.x - po->GetPosition().x, wmousepos.y - po->GetPosition().y);
				dir = Vec2Normalize(&dir);
				Vec2 throwVel = dir * 1000.0f;// +vel;
				arcPredictor->SetPredictor(po->GetPosition(), throwVel);

				// Throw grabbed block
				if (AEInputCheckTriggered(GRAB_BUTTON))
				{
					Engine::AudioManager::playAudio(Engine::SFX_THROW, false);
					po->SetIsDynamic(true);
					// po->SetLockRotation(true);
					po->SetVelocity(throwVel);
					switch (grabbedBlock.type)
					{
					case BT_ROCK:
						po->SetMass(1.0f);
						po->SetRestitutionCoeff(0.05f);
						po->SetFrictionCoeff(0.9f);
						break;
					}
					po->timer = 0.2f;
					po->solid = false;
					Engine::Singleton<LevelObjectsManager>::Instance()->AddObject(OBJECTS_LAYER_FOREGROUND, grabbedBlock.object);
					grabbedBlock.object->UnsetLayerMask({ Engine::Physics::MASK_ALL });
					grabbedBlock.object->SetLayerMask({ Engine::Physics::MASK_ENVIRONMENT_DYNAMIC });

					grabbedBlock.object = nullptr;
					grabbedBlock.type = BT_NONE;
				}
			}
			//GRAPPLE
			// Update Grapple Hook
			grapple->Update(vel, pos, force, wmousepos);
			//DASH
			Vec2 mouseDir = wmousepos - pos;
			mouseDir = Vec2Normalize(mouseDir);
			dashIndicator->SetPosition(pos + (mouseDir * C_dashIndicatorDistance));
			dashIndicator->SetRotation(sCast<f32>(atan2f(mouseDir.y, mouseDir.x)) * 180.0f / PI);

			if (AEInputCheckReleased(DASH_BUTTON) && dashAvailable > 0 && chargingDash)
			{
				Vec2 dir = MakeVec2(wmousepos.x - pos.x, wmousepos.y - pos.y);
				dir = Vec2Normalize(&dir);
				vel = dir * 800.0f;
				dashTime = 1.0f;
				--dashAvailable;
				Engine::AudioManager::playAudio(Engine::SFX_DASH, false);
				chargingDash = false;
			}
			break;
		}
		case S_COMPLETE:
		{
			if (AEInputCheckTriggered(GRAB_BUTTON))
			{
				Engine::Singleton<GameStateManager>::Instance()->SetIsLevelClear(false);
				mapReference->NextLevel();
				vel = Vec2Zero();
				currentState = S_ALIVE;
				health = C_maxPlayerHealth;
			}
			break;
		}
		case S_DEAD:
		{
			if (AEInputCheckTriggered(GRAB_BUTTON))
				Respawn();
			break;
		}
		}
		dashTime -= Engine::C_frametime;
		if (dashTime < 0.0f)
		{
			dashTime = 0.0f;
			if (dashAvailable == -1)
				dashAvailable = 1;
		}
		// Air Resistance (Simplified)
		if (Vec2LengthSquared(vel) > 0)
		{
			Vec2 dragDir = Vec2Normalize(MakeVec2(-vel.x, -vel.y));

			// We just assume drag coefficient * cross sectional area = 0.005f
			f32 drag = Vec2LengthSquared(vel) * 0.5f * isSwimming ? C_waterFluidDensity : C_airFluidDensity * (dragCoEff + (dashDragCoEff * (dashTime)));
			AddForce(dragDir * drag * Engine::C_frametime);
		}

		// Limit velocity
		if (Vec2LengthSquared(vel) > C_physicsMaxVelocity * C_physicsMaxVelocity)
			vel = Vec2Normalize(vel) * C_physicsMaxVelocity;

		// Set the renderer position
		SetVelocity(vel);
		SetPosition(pos);
		isGrounded = false;

		// Update health display
		healthDisplay->SetHealthDisplay(health / C_maxPlayerHealth, pos - initialPos, pos, GetRotation());
	}

	void Player::Render()
	{
		if (currentState != S_DEAD && !Singleton<GameStateManager>::Instance()->GetIsLevelClear())
		{
			if (grabbedBlock.object != nullptr)
				arcPredictor->Render();
			else
				grabDisplay->Render();
			grapple->Render();
			if (AEInputCheckCurr(DASH_BUTTON) && chargingDash)
			{
				// If dash is available, arrow is green
				if (dashAvailable > 0)
				{
					dashIndicator->SetColorTint(Engine::MakeColor(0, 255, 0, 255));
				}
				else // Else arrow is red
				{
					dashIndicator->SetColorTint(Engine::MakeColor(255, 0, 0, 255));
				}

				dashIndicator->Render();
			}
		}
		// We want the collision box to be smaller than the sprite, so we're setting the
		// render scale here, then reverting
		Vec2 prevScale = transform.GetScale();
		transform.SetScale(C_playerRenderScale);
		Engine::Object::Render();
		transform.SetScale(prevScale);

		radiusPingTransparency -= Engine::C_frametime;
		if (radiusPingTransparency > 0.0f)
		{
			radiusIndicator->SetPosition(pos);
			radiusIndicator->SetScale(MakeVec2(grabRadius, grabRadius) * 4);
			radiusIndicator->SetTransparency(radiusPingTransparency);
			radiusIndicator->Render();
		}

		if (grabbedBlock.object != nullptr)
		{
			grabbedBlock.object->SetPosition(pos + MakeVec2(0.0f, 1.0f * C_blockLength) + vel * Engine::C_frametime);
			grabbedBlock.object->Render();
		}

		healthDisplay->Render();
	}

	void Player::AddForce(Vec2 dir) {
		float f = sqrt(dir.x * dir.x + dir.y * dir.y);
		dir /= f;
		AddForce(dir.x, dir.y, f);//Using AddForce(x,y,force)
	}
	void Player::AddForce(Vec2 dir, float f) {
		float fa = sqrt(dir.x * dir.x + dir.y * dir.y);
		dir /= fa;
		AddForce(dir.x, dir.y, f);//Using AddForce(x,y,force)
	}
	void Player::AddForce(float x, float y, float f)
	{
		this->force.x += x * f;
		this->force.y += y * f;
	}

	Player::~Player()
	{
		delete radiusIndicator;
		abilityText.reset();
	}
	void Player::OnCollision(Engine::PhysicsObject& thisObj, Engine::PhysicsObject& otherObj, Engine::Physics::CollisionData& collisionData)
	{
		UNREFERENCED_PARAMETER(thisObj);
		UNREFERENCED_PARAMETER(otherObj);
		if (collisionData.collisionNormal.y > 0.8f)
			isGrounded = true;

		// Redirect velocity direction while grappling to allow sliding along the collision surface more easily
		if (grapple->GetStatus() == GH_ATTACHED && Vec2Dot(vel, collisionData.collisionNormal) > 0.0f)
		{
			Vec2 rot90Normal = Vec2Rot90(collisionData.collisionNormal);
			float dotVal = Vec2Dot(vel, rot90Normal);
			if (std::fabs(dotVal) > 2.0f)
			{
				if (dotVal < 0.0f)
					rot90Normal = -rot90Normal;
				
				SetPosition(GetPosition() + rot90Normal * collisionData.penetrationDepth * 0.5f);
			}
		}
	}

	void Player::InitAnimationTree()
	{
		if (playerAnimInit)
		{
			return;
		}
		playerAnimInit = true;

		AddToTree(SpriteBuilder::CreateSpriteSquare("Textures/Player/PlayerIdleLeft.png",
			true, 2, 3, 1.0f, 6), PLAYER_SPRITE::PL_IDLE_LEFT);
		AddToTree(SpriteBuilder::CreateSpriteSquare("Textures/Player/PlayerMoveLeft.png",
			true, 2, 3, C_playerWalkSpeed, 6), PLAYER_SPRITE::PL_MOVE_LEFT);
		AddToTree(SpriteBuilder::CreateSpriteSquare("Textures/Player/PlayerJumpLeft.png"),
			PLAYER_SPRITE::PL_JUMP_LEFT);
		AddToTree(SpriteBuilder::CreateSpriteSquare("Textures/Player/PlayerFallLeft.png"),
			PLAYER_SPRITE::PL_FALL_LEFT);

		AddToTree(SpriteBuilder::CreateSpriteSquare("Textures/Player/PlayerIdleRight.png",
			true, 2, 3, 1.0f, 6), PLAYER_SPRITE::PL_IDLE_RIGHT);
		AddToTree(SpriteBuilder::CreateSpriteSquare("Textures/Player/PlayerMoveRight.png",
			true, 2, 3, C_playerWalkSpeed, 6), PLAYER_SPRITE::PL_MOVE_RIGHT);
		AddToTree(SpriteBuilder::CreateSpriteSquare("Textures/Player/PlayerJumpRight.png")
			, PLAYER_SPRITE::PL_JUMP_RIGHT);
		AddToTree(SpriteBuilder::CreateSpriteSquare("Textures/Player/PlayerFallRight.png"),
			PLAYER_SPRITE::PL_FALL_RIGHT);

		AddToTree(SpriteBuilder::CreateSpriteSquare("Textures/Player/PlayerIdleGrabLeft.png",
			true, 2, 3, 1.0f, 6), PLAYER_SPRITE::PL_GRAB_IDLE_LEFT);
		AddToTree(SpriteBuilder::CreateSpriteSquare("Textures/Player/PlayerMoveGrabLeft.png", 
			true, 2, 3, C_playerWalkSpeed, 6), PLAYER_SPRITE::PL_GRAB_MOVE_LEFT);
		AddToTree(SpriteBuilder::CreateSpriteSquare("Textures/Player/PlayerJumpGrabLeft.png")
			, PLAYER_SPRITE::PL_GRAB_JUMP_LEFT);
		AddToTree(SpriteBuilder::CreateSpriteSquare("Textures/Player/PlayerFallGrabLeft.png")
			, PLAYER_SPRITE::PL_GRAB_FALL_LEFT);

		AddToTree(SpriteBuilder::CreateSpriteSquare("Textures/Player/PlayerIdleGrabRight.png",
			true, 2, 3, 1.0f, 6), PLAYER_SPRITE::PL_GRAB_IDLE_RIGHT);
		AddToTree(SpriteBuilder::CreateSpriteSquare("Textures/Player/PlayerMoveGrabRight.png", 
			true, 2, 3, C_playerWalkSpeed, 6), PLAYER_SPRITE::PL_GRAB_MOVE_RIGHT);
		AddToTree(SpriteBuilder::CreateSpriteSquare("Textures/Player/PlayerJumpGrabRight.png")
			, PLAYER_SPRITE::PL_GRAB_JUMP_RIGHT);
		AddToTree(SpriteBuilder::CreateSpriteSquare("Textures/Player/PlayerFallGrabRight.png")
			, PLAYER_SPRITE::PL_GRAB_FALL_RIGHT);

		animationFSM.InsertState(new PlayerIdleLeft(this, &animationFSM), PLAYER_SPRITE::PL_IDLE_LEFT);
		animationFSM.InsertState(new PlayerMoveLeft(this, &animationFSM), PLAYER_SPRITE::PL_MOVE_LEFT);
		animationFSM.InsertState(new PlayerJumpLeft(this, &animationFSM), PLAYER_SPRITE::PL_JUMP_LEFT);
		animationFSM.InsertState(new PlayerFallLeft(this, &animationFSM), PLAYER_SPRITE::PL_FALL_LEFT);
		animationFSM.InsertState(new PlayerIdleRight(this, &animationFSM), PLAYER_SPRITE::PL_IDLE_RIGHT);
		animationFSM.InsertState(new PlayerMoveRight(this, &animationFSM), PLAYER_SPRITE::PL_MOVE_RIGHT);
		animationFSM.InsertState(new PlayerJumpRight(this, &animationFSM), PLAYER_SPRITE::PL_JUMP_RIGHT);
		animationFSM.InsertState(new PlayerFallRight(this, &animationFSM), PLAYER_SPRITE::PL_FALL_RIGHT);

		animationFSM.InsertState(new PlayerGrabIdleLeft(this, &animationFSM), PLAYER_SPRITE::PL_GRAB_IDLE_LEFT);
		animationFSM.InsertState(new PlayerGrabMoveLeft(this, &animationFSM), PLAYER_SPRITE::PL_GRAB_MOVE_LEFT);
		animationFSM.InsertState(new PlayerGrabJumpLeft(this, &animationFSM), PLAYER_SPRITE::PL_GRAB_JUMP_LEFT);
		animationFSM.InsertState(new PlayerGrabFallLeft(this, &animationFSM), PLAYER_SPRITE::PL_GRAB_FALL_LEFT);
		animationFSM.InsertState(new PlayerGrabIdleRight(this, &animationFSM), PLAYER_SPRITE::PL_GRAB_IDLE_RIGHT);
		animationFSM.InsertState(new PlayerGrabMoveRight(this, &animationFSM), PLAYER_SPRITE::PL_GRAB_MOVE_RIGHT);
		animationFSM.InsertState(new PlayerGrabJumpRight(this, &animationFSM), PLAYER_SPRITE::PL_GRAB_JUMP_RIGHT);
		animationFSM.InsertState(new PlayerGrabFallRight(this, &animationFSM), PLAYER_SPRITE::PL_GRAB_FALL_RIGHT);

		animationFSM.Init(animationFSM.GetState(PLAYER_SPRITE::PL_IDLE_RIGHT));
	}
	void Player::ChangeAnimation(PLAYER_SPRITE newSprite)
	{
		SetSprite(playerSpriteAnims[newSprite]);
	}
	void Player::AddToTree(sPtr<Engine::Sprite> sprite, PLAYER_SPRITE key)
	{
		playerSpriteAnims.emplace(key, sprite);
	}
}