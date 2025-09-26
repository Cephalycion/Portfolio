/******************************************************************************/
/*!
\file   G_BlockTexture.h
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Kendrick Sim Hean Guan (100%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\brief
  This is an interface file for a system that loads and stores block
  textures, and provides a standard interface for blocks to access their
  specific appropriate texture.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#pragma once
#include "pch.h"
#include "E_Sprite.h"
#include "G_Block.h" // For BLOCK_TYPE

namespace Game {

	using namespace Engine;

	enum BLOCK_TEXTURE_TYPE
	{
		// 0b1000 == TOP
		// 0b0100 == BOTTOM
		// 0b0010 == LEFT
		// 0b0001 == RIGHT

		BLOCK_TEXTURE_NONE = 0b0000,
		BLOCK_TEXTURE_ALL = 0b1111,

		BLOCK_TEXTURE_TOP = 0b1000,
		BLOCK_TEXTURE_BOTTOM = 0b0100,
		BLOCK_TEXTURE_LEFT = 0b0010,
		BLOCK_TEXTURE_RIGHT = 0b0001,

		BLOCK_TEXTURE_TOP_BOTTOM = 0b1100,
		BLOCK_TEXTURE_TOP_LEFT = 0b1010,
		BLOCK_TEXTURE_TOP_RIGHT = 0b1001,
		BLOCK_TEXTURE_BOTTOM_LEFT = 0b0110,
		BLOCK_TEXTURE_BOTTOM_RIGHT = 0b0101,
		BLOCK_TEXTURE_LEFT_RIGHT = 0b0011,

		BLOCK_TEXTURE_TOP_BOTTOM_LEFT = 0b1110,
		BLOCK_TEXTURE_TOP_BOTTOM_RIGHT = 0b1101,
		BLOCK_TEXTURE_TOP_LEFT_RIGHT = 0b1011,
		BLOCK_TEXTURE_BOTTOM_LEFT_RIGHT = 0b0111,
		
		BLOCK_TEXTURE_TOTAL = 0b10000
	};

	enum BLOCK_TEXTURE_VARIANT
	{
		BLOCK_TEXTURE_VARIANT_MAIN = 0,
		BLOCK_TEXTURE_VARIANT_1, // For the second texture set of a single block type (such as liquids)

		BLOCK_TEXTURE_VARIANT_TOTAL
	};

	// 1 Set of sprites for 1 block type
	class BlockTextureCollectionSingle
	{
	protected:
		std::array<sPtr<Sprite>, BLOCK_TEXTURE_TOTAL> sprites;
		std::vector<sPtr<Sprite>> animatedSprites;

		// Adds specified sprite to animatedSprites if it is animated.
		void TrackIfAnimated(const sPtr<Sprite>& sprite);

	public:
		// Initialize non-animated textures for a block texture collection. Initializer list must
		// contain the filepaths of the 16 types of textures listed in the enum above.
		void Init(std::initializer_list<std::string> blockTextures);
		// Initialize potentially animated textures for a block texture collection. Initializer
		// list must contain all sprites of the 16 types of textures listed in the enum above.
		void Init(std::initializer_list<sPtr<Sprite>> blockSprites);

		// Update animated sprites, if any
		void Update(float dt);

		sPtr<Sprite> GetSprite(BLOCK_TEXTURE_TYPE type) const;

	};

	// Set(s) of sprites collection for 1 block type
	class BlockTextureCollectionFull
	{
	protected:
		std::unordered_map<BLOCK_TEXTURE_VARIANT, sPtr<BlockTextureCollectionSingle>> singleCollections;

	public:
		// Add a collection for 1 variant of the block type.
		void AddCollection(BLOCK_TEXTURE_VARIANT variant, const sPtr<BlockTextureCollectionSingle> collectionSingle);

		// Update animated sprites, if any
		void Update(float dt);

		sPtr<Sprite> GetSprite(BLOCK_TEXTURE_TYPE type, BLOCK_TEXTURE_VARIANT variant) const;
	};

	class BlockTextureManager
	{
	protected:
		std::unordered_map<BLOCK_TYPE, sPtr<BlockTextureCollectionFull>> textureCollections;

		std::string MakeFilepath(const std::string& filename, const std::string& variant);

		void LoadCollection(BLOCK_TYPE type, BLOCK_TEXTURE_VARIANT variant, const std::string& filename);
		void LoadLiquidCollection(BLOCK_TYPE type, BLOCK_TEXTURE_VARIANT variant, const std::string& filename,
			const Vec2& uvLength, const Vec2& uvScrollSpeed, const Vec2& uvStartOffset = Vec2Zero());

		void InsertSingleCollection(BLOCK_TYPE type, BLOCK_TEXTURE_VARIANT variant, const sPtr<BlockTextureCollectionSingle>& collectionSingle);

	public:
		BlockTextureManager();

		void UpdateTexturesAnimation(float dt);

		sPtr<Sprite> GetSprite(BLOCK_TYPE blockType, BLOCK_TEXTURE_VARIANT variant, BLOCK_TEXTURE_TYPE textureType);

	};

}

