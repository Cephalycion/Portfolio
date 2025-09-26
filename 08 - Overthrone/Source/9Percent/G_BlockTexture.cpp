/******************************************************************************/
/*!
\file   G_BlockTexture.cpp
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Kendrick Sim Hean Guan (100%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\brief
  This is the implementation file for a system that loads and stores block
  textures, and provides a standard interface for blocks to access their
  specific appropriate texture.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#include "pch.h"
#include "G_BlockTexture.h"

namespace Game {

	void BlockTextureCollectionSingle::TrackIfAnimated(const sPtr<Sprite>& sprite)
	{
		if (std::dynamic_pointer_cast<AnimatedSprite>(sprite) || std::dynamic_pointer_cast<ScrollingSprite>(sprite))
		{
			// Ensure this animated sprite isn't inside the vector yet.
			auto iter = std::find_if(animatedSprites.begin(), animatedSprites.end(), [&sprite](const sPtr<Sprite>& regAnimSprite) -> bool {
				return regAnimSprite.get() == sprite.get();
			});
			if (iter == animatedSprites.end())
				animatedSprites.emplace_back(sprite);
		}
	}

	void BlockTextureCollectionSingle::Init(std::initializer_list<std::string> blockTextures)
	{
#ifdef _DEBUG
		if (blockTextures.size() != BLOCK_TEXTURE_TOTAL)
		{
			Error::RaiseErr("BlockTextureCollection::Init", "Invalid number of block texture filepaths!");
			return;
		}
#endif

		animatedSprites.clear();
		int index = 0;
		for (auto iter = blockTextures.begin(); iter != blockTextures.end(); ++iter, ++index)
			sprites[index] = SpriteBuilder::CreateSpriteSquare(*iter);
	}
	void BlockTextureCollectionSingle::Init(std::initializer_list<sPtr<Sprite>> blockSprites)
	{
#ifdef _DEBUG
		if (blockSprites.size() != BLOCK_TEXTURE_TOTAL)
		{
			Error::RaiseErr("BlockTextureCollection::Init", "Invalid number of block texture filepaths!");
			return;
		}
#endif

		animatedSprites.clear();
		int index = 0;
		for (auto iter = blockSprites.begin(); iter != blockSprites.end(); ++iter, ++index)
		{
			sprites[index] = *iter;
			TrackIfAnimated(*iter);
		}
	}

	void BlockTextureCollectionSingle::Update(float dt)
	{
		for (const auto& animSprite : animatedSprites)
			animSprite->Update(dt);
	}

	sPtr<Sprite> BlockTextureCollectionSingle::GetSprite(BLOCK_TEXTURE_TYPE type) const
	{
		return sprites[type];
	}


	void BlockTextureCollectionFull::AddCollection(BLOCK_TEXTURE_VARIANT variant, const sPtr<BlockTextureCollectionSingle> collectionSingle)
	{
		singleCollections.emplace(variant, collectionSingle);
	}

	void BlockTextureCollectionFull::Update(float dt)
	{
		for (auto& collection : singleCollections)
			collection.second->Update(dt);
	}

	sPtr<Sprite> BlockTextureCollectionFull::GetSprite(BLOCK_TEXTURE_TYPE type, BLOCK_TEXTURE_VARIANT variant) const
	{
		// If there's only 1 collection, select it
		if (singleCollections.size() == 1)
			return singleCollections.begin()->second->GetSprite(type);

		// Find the requested collection
		auto collectionIter = singleCollections.find(variant);
		if (collectionIter == singleCollections.end())
			return nullptr;

		return collectionIter->second->GetSprite(type);
	}


	BlockTextureManager::BlockTextureManager()
	{
		LoadCollection(BT_ROCK, BLOCK_TEXTURE_VARIANT_MAIN, "Rock");
		LoadCollection(BT_NONGRAPPLE, BLOCK_TEXTURE_VARIANT_MAIN, "Bedrock");
		LoadCollection(BT_INDESTRUCTIBLE, BLOCK_TEXTURE_VARIANT_MAIN, "Grapple");
		LoadCollection(BT_STICKY, BLOCK_TEXTURE_VARIANT_MAIN, "Sticky");
		LoadCollection(BT_ICE, BLOCK_TEXTURE_VARIANT_MAIN, "Ice");
		LoadLiquidCollection(BT_LAVA, BLOCK_TEXTURE_VARIANT_MAIN, "Lava", C_bTexLavaUVLength, C_bTexLavaScrollSpeed);
		LoadLiquidCollection(BT_LAVA, BLOCK_TEXTURE_VARIANT_1, "Lava", C_bTexLavaUVLength, C_bTexLavaScrollSpeed, MakeVec2(0.5f, 0.0f));
		LoadLiquidCollection(BT_WATER, BLOCK_TEXTURE_VARIANT_MAIN, "Water", C_bTexWaterUVLength, C_bTexWaterScrollSpeed);
		LoadLiquidCollection(BT_WATER, BLOCK_TEXTURE_VARIANT_1, "Water", C_bTexWaterUVLength, C_bTexWaterScrollSpeed, MakeVec2(0.5f, 0.0f));
	}

	void BlockTextureManager::UpdateTexturesAnimation(float dt)
	{
		for (const auto& collection : textureCollections)
			collection.second->Update(dt);
	}

	sPtr<Sprite> BlockTextureManager::GetSprite(BLOCK_TYPE blockType, BLOCK_TEXTURE_VARIANT variant, BLOCK_TEXTURE_TYPE textureType)
	{
		auto collectionIter = textureCollections.find(blockType);
		if (collectionIter == textureCollections.end())
			return nullptr;

		return collectionIter->second->GetSprite(textureType, variant);
	}

	std::string BlockTextureManager::MakeFilepath(const std::string& filename, const std::string& variant)
	{
		return C_bTexFilepath + filename + "/" + filename + "-" + variant + C_bTexFiletype;
	}

	void BlockTextureManager::LoadCollection(BLOCK_TYPE type, BLOCK_TEXTURE_VARIANT variant, const std::string& filename)
	{
		sPtr<BlockTextureCollectionSingle> collectionSingle(new BlockTextureCollectionSingle());
		collectionSingle->Init({
			MakeFilepath(filename, "Solo"),			// 0b0000 - Center
			MakeFilepath(filename, "StubLeft"),		// 0b0001 - Right
			MakeFilepath(filename, "StubRight"),	// 0b0010 - Left
			MakeFilepath(filename, "Horizontal"),	// 0b0011 - Left_Right
			MakeFilepath(filename, "StubUp"),		// 0b0100 - Bottom
			MakeFilepath(filename, "TopLeft"),		// 0b0101 - Bottom_Right
			MakeFilepath(filename, "TopRight"),		// 0b0110 - Bottom_Left
			MakeFilepath(filename, "Floor"),		// 0b0111 - Bottom_Left_Right
			MakeFilepath(filename, "StubDown"),		// 0b1000 - Top
			MakeFilepath(filename, "BottomLeft"),	// 0b1001 - Top_Right
			MakeFilepath(filename, "BottomRight"),	// 0b1010 - Top_Left
			MakeFilepath(filename, "Ceiling"),		// 0b1011 - Top_Left_Right
			MakeFilepath(filename, "Vertical"),		// 0b1100 - Top_Bottom
			MakeFilepath(filename, "Left"),			// 0b1101 - Top_Bottom_Right
			MakeFilepath(filename, "Right"),		// 0b1110 - Top_Bottom_Left
			MakeFilepath(filename, "Fill"),			// 0b1111 - All
		});

		InsertSingleCollection(type, variant, collectionSingle);
	}

	void BlockTextureManager::LoadLiquidCollection(BLOCK_TYPE type, BLOCK_TEXTURE_VARIANT variant, const std::string& filename,
		const Vec2& uvLength, const Vec2& uvScrollSpeed, const Vec2& uvStartOffset)
	{
		sPtr<Sprite> spriteFilled = SpriteBuilder::CreateScrollSpriteSquare(MakeFilepath(filename, "Fill"), uvLength, uvScrollSpeed, uvStartOffset);
		sPtr<Sprite> spriteSurface = SpriteBuilder::CreateScrollSpriteSquare(MakeFilepath(filename, "Surface"), uvLength, uvScrollSpeed, uvStartOffset);
		sPtr<Sprite> spriteCenter = SpriteBuilder::CreateSpriteSquare(MakeFilepath(filename, "Solo"));

		sPtr<BlockTextureCollectionSingle> collectionSingle(new BlockTextureCollectionSingle());
		collectionSingle->Init({
			spriteCenter,	// 0b0000 - Center
			spriteSurface,	// 0b0001 - Right
			spriteSurface,	// 0b0010 - Left
			spriteSurface,	// 0b0011 - Left_Right
			spriteSurface,	// 0b0100 - Bottom
			spriteSurface,	// 0b0101 - Bottom_Right
			spriteSurface,	// 0b0110 - Bottom_Left
			spriteSurface,	// 0b0111 - Bottom_Left_Right
			spriteFilled,	// 0b1000 - Top
			spriteFilled,	// 0b1001 - Top_Right
			spriteFilled,	// 0b1010 - Top_Left
			spriteFilled,	// 0b1011 - Top_Left_Right
			spriteFilled,	// 0b1100 - Top_Bottom
			spriteFilled,	// 0b1101 - Top_Bottom_Right
			spriteFilled,	// 0b1110 - Top_Bottom_Left
			spriteFilled	// 0b1111 - All
		});

		InsertSingleCollection(type, variant, collectionSingle);
	}

	void BlockTextureManager::InsertSingleCollection(BLOCK_TYPE type, BLOCK_TEXTURE_VARIANT variant, const sPtr<BlockTextureCollectionSingle>& collectionSingle)
	{
		sPtr<BlockTextureCollectionFull> collectionFull;
		auto collectionFullIter = textureCollections.find(type);
		if (collectionFullIter == textureCollections.end())
		{
			collectionFull.reset(new BlockTextureCollectionFull());
			textureCollections.emplace(type, collectionFull);
		}
		else
			collectionFull = collectionFullIter->second;

		collectionFull->AddCollection(variant, collectionSingle);
	}

}
