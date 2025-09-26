/******************************************************************************/
/*!
\file   G_Block.h
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
  This is the interface file for all member functions of a class called Block,
  along with different utility functions for blocks.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#pragma once
#include "pch.h"
#include "E_Object.h"
#include "G_Constants.h"

// Forward declarations
namespace Game {
	enum BLOCK_TEXTURE_TYPE : int;
}

namespace Game
{
	enum BLOCK_TYPE
	{
		// 0X SERIES -> ENVIRONMENT
		BT_NONE = 0,
		BT_INDESTRUCTIBLE = 9,
		BT_NONGRAPPLE = 8,
		BT_STICKY = 1,
		BT_ROCK = 2,
		BT_ICE = 3,
		BT_WATER = 4,
		BT_LAVA = 5,

		// 1X SERIES -> PLAYER RELATED
		BT_PLAYER = 10,

		// 2X SERIES -> ENEMY RELATED
		BT_ENEMY = 20,			// Patrolling enemy
		BT_TURRET = 21,
		BT_BOSS = 22,

		// 3X SERIES -> PUZZLE RELATED
		BT_BUTTON = 30,
		BT_DOOR = 31,//Place to the top of the opening (HDOOR deprecated due to having omni-directional doors)
		BT_LIQUIDEMITTER = 32,//Spills Liquids
		
		BT_DOORCOVERED = 310,//Do not use in level creator LOL

		// END
		BT_END = 100,

		// 20X SERIES -> BACKGROUND
		BT_SIGN = 200,
	};

	class PhysBlock : public Engine::PhysicsObject
	{
	protected:
		// Whether this block has a texture (and therefore should not have a tint)
		bool hasTexture;

	public:
		PhysBlock();

		bool GetHasTexture() const;
		void SetHasTexture(bool newHasTexture);

		void SetIsHighlighted(bool isHighlighted);

		virtual void SetIsDynamic(bool isDynamic) override;

		BLOCK_TYPE type;

		void Update() override;
		bool submerged = false;
		float timer = 0.2f;
		bool solid = false;
		// state
		// 0 == solid
		// 1 == highlighted
		// 2 == dynamic
		static Engine::Color GetBlockTint(BLOCK_TYPE type, s32 state = 0);
	};

	class Block
	{
	protected:
		// Reads data from the string data (delimited by '|') and removes it from the string each time this is called
		// \author kendrickheanguan.s
		template <typename T>
		T ReadData();

	public:
		BLOCK_TYPE type;
		std::string data;
		sPtr<PhysBlock> object;
		int x=0, y=0;

		void Init(int x, int y);
		void Update();

		Block() : type(BT_NONE), object(nullptr) {};
		~Block();

		// Getter to the object
		wPtr<const PhysBlock> GetPhysBlockWeak() const;

		bool IsFillable();

		//Physics Functions
		void OnBlockSettle(Engine::PhysicsObject& thisObj);
		void OnCollision(Engine::PhysicsObject& thisObj, Engine::PhysicsObject& otherObj, Engine::Physics::PhysicsCollisionData& collisionData);

		void FillBlock(BLOCK_TYPE type);
		void SetBlockProperties(sPtr<PhysBlock>obj);
		void LockBlock(Engine::PhysicsObject& thisObj, bool aircheck);

		// Set the texture of the object possessed by this block based on the blocks
		// surrounding it.
		// \author kendrickheanguan.s
		void UpdateTexture();
		// Force a block to be of a certain texture type, but still based on its BLOCK_TYPE
		void UpdateTexture(BLOCK_TEXTURE_TYPE textureType);
		// Force a block to be of a certain texture
		void UpdateTexture(const sPtr<Engine::Sprite>& sprite);

		static bool GetBlockPickable(BLOCK_TYPE type);
	};


	template <typename T>
	T Block::ReadData()
	{
#if _DEBUG
		if (data.empty())
			Engine::Error::RaiseErr("Block::ReadData", "data string is empty!");
#endif

		size_t endIndex = data.find_first_of('|');
		T var = Engine::Util::StrToVar<T>(data.substr(0, endIndex));
		if (endIndex == std::string::npos)
			data.clear();
		else
			data.erase(0, endIndex + 1);
		return var;
	}
}