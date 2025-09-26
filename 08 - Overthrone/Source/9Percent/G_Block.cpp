/******************************************************************************/
/*!
\file   G_Block.cpp
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Matthew Chan Shao Jie (75%)
\par    email: m.chan\@digipen.edu
\par    DigiPen login: m.chan
	
\author Kendrick Sim Hean Guan (11%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s	
	
\author Marc Alviz Evangelista (9%)
\par    email: marcalviz.e\@digipen.edu
\par    DigiPen login: marcalviz.e

\author Ryan Chan Kuan Fu (5%)
\par    email: c.kuanfuryan\@digipen.edu
\par    DigiPen login: c.kuanfuryan

\brief
  This is the implementation file for all member functions of a class called 
  Block, along with different utility functions for blocks.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#include "pch.h"
#include "E_IOManager.h"
#include "E_Engine.h"
#include "G_LevelObjects.h"
#include "G_BlockTexture.h"
#include "G_Player.h"
#include "G_Map.h"
#include "G_DoorBlock.h"
#include "G_ButtonBlock.h"
#include "G_LiquidEmitter.h"
#include "G_Sign.h"
#include "G_ParticleSystem.h"
#include "G_EnemyTurret.h"
#include "G_EnemyBoss.h"
#include "G_EnemyPatroller.h"

namespace Game
{
	std::vector<std::string> SplitData(std::string data)
	{
		std::vector<std::string> output;
		size_t p=0,c=0;
		while (p != std::string::npos)
		{
			c = data.find('|',p+1);
			output.push_back(data.substr( p > 0 ? p + 1 : p, c - (p > 0 ? p + 1:p)));
			p = c;
		}
		return output;
	}

	void Block::Init(int _x, int _y)
	{
		this->x = _x;
		this->y = _y;
		Vec2 pos = Engine::Singleton<Map>::Instance()->MapToWorldPos(x, y);
		Engine::Graphics::Transform transform(pos.x, pos.y, 0.0f, C_blockLength, C_blockLength);
		if (object != nullptr)
			object.reset();
		switch (type)
		{
		case BT_LIQUIDEMITTER:
		{
			sPtr<LiquidEmitter> le = Engine::ObjectBuilder::CreateGenericSquare<LiquidEmitter>(transform, Engine::SpriteBuilder::CreateSpriteSquare(C_bTexFilepath + std::string("Emitter/Emitter.png")));
			le->ID = ReadData<std::string>();
			le->x = x;
			le->y = y;
			le->emittedBlock = sCast<BLOCK_TYPE>(ReadData<int>());
			le->isActive = ReadData<bool>();
			le->emitCount = ReadData<int>();
			Engine::Singleton<LevelObjectsManager>::Instance()->AddObject(OBJECTS_LAYER_FOREGROUND, le);
			type = BT_NONE;
			break;
		}
		case BT_DOOR:
		{
			sPtr<DoorBlock> d = Engine::ObjectBuilder::CreateGenericSquare<DoorBlock>(transform, Engine::SpriteBuilder::CreateSpriteSquare(), 1.0f, Engine::C_colorWhite);
			d->ID = ReadData<std::string>();
			d->dir = sCast<DoorBlock::DOOR_DIRECTION>(ReadData<int>());
			d->x = x;
			d->y = y;
			Engine::Singleton<LevelObjectsManager>::Instance()->AddObject(OBJECTS_LAYER_FOREGROUND, d);
			type = BT_NONE;
			break;
		}
		case BT_BUTTON:
		{
			sPtr<ButtonBlock> b = Engine::ObjectBuilder::CreateGenericSquare<ButtonBlock>(Engine::Graphics::Transform(pos.x, pos.y, 0.0f, C_blockLength, C_blockLength * 0.2f), Engine::SpriteBuilder::CreateSpriteSquare(), 1.0f);
			b->ID = ReadData<std::string>();
			b->x = x;
			b->y = y;
			b->residentBlock = this;
			Engine::Singleton<LevelObjectsManager>::Instance()->AddObject(OBJECTS_LAYER_FOREGROUND, b);
			type = BT_NONE;
			break;
		}
		// Enemies
		case BT_ENEMY:
		{
			sPtr<EnemyPatroller> b = Game::PatrollerBuilder::CreatePatroller(transform);
			//b->SetColorTint(Color(0.9f, 0.2f, 0.85f));
			b->Init();
			b->UnsetLayerMask({ Engine::Physics::MASK_ALL });
			b->SetLayerMask({ Engine::Physics::MASK_ENEMY, Engine::Physics::MASK_ENVIRONMENT_DYNAMIC });
			Engine::Singleton<LevelObjectsManager>::Instance()->AddObject(OBJECTS_LAYER_FOREGROUND, b);
			type = BT_NONE;
			break;
		}
		case BT_TURRET:
		{
			sPtr<EnemyTurret> b = Game::TurretBuilder::CreateTurret(transform, SpriteBuilder::CreateSpriteSquare("Textures/Turret/EnemyTurret.png", true, 2, 4, 1.0f, 8));
			b->UnsetLayerMask({ Engine::Physics::MASK_ALL });
			b->SetLayerMask({ Engine::Physics::MASK_ENEMY , Engine::Physics::MASK_ENVIRONMENT_DYNAMIC });
			Engine::Singleton<LevelObjectsManager>::Instance()->AddObject(OBJECTS_LAYER_FOREGROUND, b);
			Singleton<PhysicsManager>::Instance()->RegisterSpecialStatic(b->physicsComp);
			type = BT_NONE;
			b->Init();
			break;
		}
		case BT_BOSS:
		{
			sPtr<EnemyBoss> b = Engine::ObjectBuilder::CreateGenericSquare<EnemyBoss>(transform);
			//b->SetColorTint(Engine::C_colorRed);
			b->UnsetLayerMask({ Engine::Physics::MASK_ALL });
			b->SetLayerMask({ Engine::Physics::MASK_ENEMY, Engine::Physics::MASK_ENVIRONMENT_DYNAMIC });
			Engine::Singleton<LevelObjectsManager>::Instance()->AddObject(OBJECTS_LAYER_FOREGROUND, b);
			Singleton<PhysicsManager>::Instance()->RegisterSpecialStatic(b->physicsComp);
			b->SetGravityEnabled(false);
			type = BT_NONE;
			b->SetBossAnimInit(false);
			b->Init();
			break;
		}
		case BT_WATER:
		case BT_LAVA:
		{
			object = Engine::ObjectBuilder::CreateGenericSquare<PhysBlock>(transform);
			object->SetIsDynamic(false);
			object->UnsetLayerMask({ Engine::Physics::MASK_ALL });
			break;
		}
		case BT_PLAYER:
		{
			object = Engine::ObjectBuilder::CreateGenericSquare<PhysBlock>(transform);
			object->SetIsDynamic(false);
			object->UnsetLayerMask({ Engine::Physics::MASK_ALL });
			object->SetLayerMask({ Engine::Physics::MASK_ENVIRONMENT });
			break;
		}
		case BT_END:
		{
			object = Engine::ObjectBuilder::CreateGenericSquare<PhysBlock>(transform);
			object->SetIsDynamic(false);
			object->UnsetLayerMask({ Engine::Physics::MASK_ALL });
			object->SetLayerMask({ Engine::Physics::MASK_ENVIRONMENT });
			break;
		}
		case BT_STICKY:
		case BT_NONGRAPPLE:
		case BT_INDESTRUCTIBLE:
		case BT_ROCK:
		case BT_ICE:
		{
			object = Engine::ObjectBuilder::CreateGenericSquare<PhysBlock>(transform);
			object->SetIsDynamic(false);
			object->UnsetLayerMask({ Engine::Physics::MASK_ALL });
			object->SetLayerMask({ Engine::Physics::MASK_ENVIRONMENT, Engine::Physics::MASK_ENVIRONMENT_DYNAMIC,  Engine::Physics::MASK_PLAYER, Engine::Physics::MASK_ENEMY });
			break;
		}
		// Background
		case BT_SIGN:
		{
			sPtr<BG_Sign> sign(new BG_Sign(pos, ReadData<std::string>()));
			Engine::Singleton<LevelObjectsManager>::Instance()->AddObject(OBJECTS_LAYER_BACKGROUND, sign);
			type = BT_NONE;
			break;
		}
		default:
		{
			Error::RaiseErrFormatted("Block::Init", "Invalid id %d", ERROR_ERR, type);
			break;
		}
		case BT_NONE:
		case BT_DOORCOVERED:
		{
			break;
		}
		}
		if (object != nullptr)
		{
			object->SetColorTint(PhysBlock::GetBlockTint(type));

			object->SetOnSettleCallback(std::bind(&Block::OnBlockSettle, this, std::placeholders::_1));
			object->SetOnCollisionCallback(std::bind(&Block::OnCollision, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
			dCast<PhysBlock*>(object.get())->type = type;
			SetBlockProperties(object);
		}
	}
	void Block::Update()
	{

	}
	bool Block::IsFillable()
	{
		switch (type)
		{
		case BT_NONE:
		case BT_WATER:
		case BT_LAVA:
			return true;
			break;
		}
		return false;
	}
	void Block::FillBlock(BLOCK_TYPE _type)
	{
		if (this->type == BT_PLAYER)
			return;
		if (object)
			object.reset();
		this->type = _type;
		Init(x, y);
	}
	Block::~Block()
	{
	}
	wPtr<const PhysBlock> Block::GetPhysBlockWeak() const
	{
		return object;
	}

	void Block::UpdateTexture()
	{
		auto map = Singleton<Map>::Instance();
		int connectionsFlag = 0b0000;

		// Set bit if a side of this block is connected to another block of the same type
		auto CheckSide = [&map, &connectionsFlag, &type = type](int x, int y, int flag) -> void {
			Block* block = map->GetBlockAtIndex(x, y);
			if (!block)
				return;
			switch (type)
			{
			case BT_WATER:
			case BT_LAVA:
				if (block->type != BT_NONE)
					// If we're liquid, always count any block touching us as blocking.
					connectionsFlag |= flag;
			default:
				switch (block->type)
				{
				case BT_WATER:
				case BT_LAVA:
					// If we're touching liquid, always count it as blocking.
					connectionsFlag |= flag;
					break;
				default:
					// Only count as blocking if this block's type is the same as us.
					if (block->type == type)
						connectionsFlag |= flag;
				}
			}
		};

		CheckSide(x, y - 1, BLOCK_TEXTURE_TOP);
		CheckSide(x, y + 1, BLOCK_TEXTURE_BOTTOM);
		CheckSide(x - 1, y, BLOCK_TEXTURE_LEFT);
		CheckSide(x + 1, y, BLOCK_TEXTURE_RIGHT);

		UpdateTexture(sCast<BLOCK_TEXTURE_TYPE>(connectionsFlag));
	}
	void Block::UpdateTexture(BLOCK_TEXTURE_TYPE textureType)
	{
		auto sprite = Singleton<BlockTextureManager>::Instance()->GetSprite(type, sCast<BLOCK_TEXTURE_VARIANT>(x & 1), textureType);

		// If no sprite, no texture has been assigned to this block type.
		// Do nothing and let the object continue using its previous sprite.
		if (sprite)
			UpdateTexture(sprite);
	}
	void Block::UpdateTexture(const sPtr<Sprite>& sprite)
	{
		object->SetSprite(sprite);
		object->SetColorTint(Color());
		object->SetHasTexture(true);
	}

	bool Block::GetBlockPickable(BLOCK_TYPE type)
	{
		switch (type)
		{
		case BT_NONE:
		case BT_INDESTRUCTIBLE:
		case BT_NONGRAPPLE:
		case BT_STICKY:
		case BT_END:
		case BT_ICE:
		{
			return false;
			break;
		}
		case BT_ROCK:
		{
			return true;
			break;
		}
		}
		return false;
	}
	void Block::SetBlockProperties(sPtr<PhysBlock>obj)
	{
		if (!obj)
			return;
		switch (type)
		{
		default:
		{
			obj->SetFrictionCoeff(0.9f);
		obj->SetMass(1.0f);
		obj->SetRestitutionCoeff(0.5f);
		break;
		}
		case BT_ICE:
		{
			obj->SetFrictionCoeff(0.01f);
			obj->SetMass(1.0f);
			obj->SetRestitutionCoeff(0.01f);
			break;
		}
		}
		switch (type)
		{
		default:
		{
			object->UnsetLayerMask({ Engine::Physics::MASK_ALL });
			object->SetLayerMask({ Engine::Physics::MASK_ENVIRONMENT, Engine::Physics::MASK_ENVIRONMENT_DYNAMIC,Engine::Physics::MASK_PLAYER });
			break;
		}
		case BT_PLAYER:
		case BT_END:
		case BT_LIQUIDEMITTER:
		{
			object->UnsetLayerMask({ Engine::Physics::MASK_ALL });
			object->SetLayerMask({ Engine::Physics::MASK_ENVIRONMENT_DYNAMIC });
			break;
		}
		case BT_WATER:
		case BT_LAVA:
		case BT_NONE:
		{
			object->UnsetLayerMask({ Engine::Physics::MASK_ALL });
			break;
		}
		}
	}
	void Block::OnBlockSettle(Engine::PhysicsObject& thisObj)
	{
		LockBlock(thisObj, true);
	}
	void Block::LockBlock(Engine::PhysicsObject& thisObj, bool aircheck = false)
	{
		sPtr<Map> map = Engine::Singleton<Map>::Instance();
		Coordinate gridPos = map->WorldToMapPos(thisObj.GetPosition());
		s32 ycheck = gridPos.y + 1;
		if (aircheck)
		{
			if (ycheck <= 0 || ycheck < map->height)
			{
				Block* b = map->GetBlockAtIndex(sCast<s32>(gridPos.x), ycheck);
				if (b != nullptr)
				{
					if (b->type == BT_NONE||b->type==BT_ICE)
						return;
				}
			}
		}
		if (gridPos.x < 0 || gridPos.y < 0 || gridPos.x >= map->width || gridPos.y >= map->height)
			return;

		Block* block = map->GetBlockAtIndex(sCast<s32>(gridPos.x), sCast<s32>(gridPos.y));
		sPtr<Player> p = Engine::Singleton<Player>::Instance();
		if ((sCast<s32>(gridPos.x) == p->xindex && sCast<s32>(gridPos.y) == p->yindex) || !block->IsFillable())//(block->type!=BT_NONE&&block->type!=BT_END))
		{
			thisObj.UnsetLayerMask({ Engine::Physics::MASK_ALL });
			thisObj.SetLayerMask({ Engine::Physics::MASK_ENVIRONMENT_DYNAMIC });
			return;
		}
		map->SetBlock(block, std::dynamic_pointer_cast<PhysBlock>(thisObj.shared_from_this()));

		thisObj.SetIsDynamic(false);
		thisObj.SetRotation(0.0f);
		thisObj.SetPosition(map->MapToWorldPos(sCast<s32>(gridPos.x), sCast<s32>(gridPos.y)));
		thisObj.SetVelocity(MakeVec2(0.0f, 0.0f));
		thisObj.SetAngVelocity(0.0f);
		thisObj.UnsetLayerMask({ Engine::Physics::MASK_ALL });
		thisObj.SetLayerMask({ Engine::Physics::MASK_ENVIRONMENT, Engine::Physics::MASK_ENVIRONMENT_DYNAMIC,  Engine::Physics::MASK_PLAYER, Engine::Physics::MASK_ENEMY });
		Engine::Singleton<LevelObjectsManager>::Instance()->RunMacro(OBJECTS_LAYER_FOREGROUND, [&thisObj](const sPtr<Engine::Object>& obj) -> bool {
			return obj != thisObj.shared_from_this();
		});
	}

	void Block::OnCollision(Engine::PhysicsObject& thisObj, Engine::PhysicsObject& otherObj, Engine::Physics::PhysicsCollisionData& collisionData)
	{
		///Testing the blocktype of collided object
		sPtr<PhysBlock> pb = std::dynamic_pointer_cast<PhysBlock>(otherObj.shared_from_this());
		if (pb != nullptr)
		{
			switch (pb->type)
			{
			case BT_STICKY:
				sPtr<Map> map = Engine::Singleton<Map>::Instance();
				Coordinate gridPos = map->WorldToMapPos(thisObj.GetPosition() );
				Block* block = map->GetBlockAtIndex(sCast<s32>(gridPos.x+1), sCast<s32>(gridPos.y));
				if (block)
					if (block->type == BT_STICKY)
						LockBlock(thisObj, false);
				block = map->GetBlockAtIndex(sCast<s32>(gridPos.x-1), sCast<s32>(gridPos.y));
				if (block)
					if (block->type == BT_STICKY)
						LockBlock(thisObj, false);
				block = map->GetBlockAtIndex(sCast<s32>(gridPos.x), sCast<s32>(gridPos.y+1));
				if (block)
					if (block->type == BT_STICKY)
						LockBlock(thisObj, false);
				block = map->GetBlockAtIndex(sCast<s32>(gridPos.x), sCast<s32>(gridPos.y-1));
				if (block)
					if (block->type == BT_STICKY)
						LockBlock(thisObj, false);
				break;
			}
		}
		// Spawn particles
		if (Vec2LengthSquared(collisionData.relativeVelocity) < 500.0f * 500.0f)
			return;
		Vec2 burstDir = (thisObj.GetIsDynamic() ? collisionData.collisionNormal : -collisionData.collisionNormal);
		Engine::Singleton<Engine::ParticleManager>::Instance()->AddObject(
			Engine::PARTICLE_LAYER_GAME,
			sPtr<Engine::ParticleSpawnerBase>(new ParticleSpawnerBlockHit(collisionData.collisionPoint, burstDir, collisionData.relativeVelocity, thisObj.shared_from_this()))
		);
	}


	PhysBlock::PhysBlock()
		: PhysicsObject()
		, hasTexture(false)
		, type(BT_NONE)
	{
	}

	bool PhysBlock::GetHasTexture() const
	{
		return hasTexture;
	}
	void PhysBlock::SetHasTexture(bool newHasTexture)
	{
		hasTexture = newHasTexture;
	}

	void PhysBlock::SetIsHighlighted(bool isHighlighted)
	{
		SetColorTint(GetBlockTint(type, isHighlighted ? 1 : 0));
	}

	void PhysBlock::SetIsDynamic(bool isDynamic)
	{
		PhysicsObject::SetIsDynamic(isDynamic);
		SetColorTint(GetBlockTint(type, isDynamic? 2:0));
	}

	void PhysBlock::Update()
	{
		if (this->physicsComp->GetIsDynamic())
		{
			sPtr<Map> map = Engine::Singleton<Map>::Instance();
			Coordinate blockindex = map->WorldToMapPos(GetPosition());
			Block* b = map->GetBlockAtIndex(blockindex);
			submerged = false;
			if (b != nullptr)
			{
				switch (b->type)
				{
				case BT_WATER:
				case BT_LAVA:
					submerged = true;
					break;
				}
			}
			if (submerged)
			{
				this->SetVelocity(GetVelocity() * 0.9f);
				this->SetAngVelocity(GetAngVelocity() * 0.9f);
				
				this->ApplyForce(MakeVec2(0.0f, 750.0f*this->GetMass()));
			}
		}
		if (!solid)
		{
			timer -= Engine::C_frametime;
			if (timer <= 0.0f)
			{
				solid = true;
				SetLayerMask({ Engine::Physics::MASK_PLAYER });
			}
		}
	}

	//States: 0 - static, 1 - highlighted, 2 - dynamic
	Engine::Color PhysBlock::GetBlockTint(BLOCK_TYPE type, s32 state)
	{
		f32 r = 0.0f, g = 0.0f, b = 0.0f, a = 1.0f;
		switch (type)
		{
		default:
		{
			switch (state)
			{
			default:
			case 0:
			{
				r = g = b = 1.0f;
				break;
			}
			case 1:
			{
				r = g = b = C_grabTintMult;
				break;
			}
			case 2:
			{
				r = g = b = C_dynamicTintMult;
				break;
			}
			}
			break;
		}
		case BT_END:
		{
			r = 0.573f;
			g = 0.816f;
			b = 0.314f;
			a = 0.7f;
			break;
		}
		case BT_PLAYER:
		{
			r = 0.314f;
			g = 0.573f;
			b = 0.816f;
			a = 0.5f;
			break;
		}
		case BT_NONE:
		{
			a = 0.0f;
			break;
		}
		}
		return Color(r, g, b, a);
	}
}