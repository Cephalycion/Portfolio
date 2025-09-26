/******************************************************************************/
/*!
\file   G_Map.cpp
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Matthew Chan Shao Jie (61%)
\par    email: m.chan\@digipen.edu
\par    DigiPen login: m.chan

\author Ryan Chan Kuan Fu (30%)
\par    email: c.kuanfuryan\@digipen.edu
\par    DigiPen login: c.kuanfuryan

\author Kendrick Sim Hean Guan (9%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\brief
  This is the implementation file for all member functions of a class called
  Map, along with different utility functions for the map.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#include "pch.h"
#include "E_Engine.h"
#include "G_Map.h"
#include "G_BlockEntity.h"
#include "G_Player.h"
#include "G_LevelObjects.h"
#include "G_BlockTexture.h"
#include "G_ParticleSystem.h"
#include "G_GameState.h"

namespace Game 
{
	Coordinate::Coordinate()
		: x(0), y(0)
	{
	}
	Coordinate::Coordinate(int x, int y)
		: x(x), y(y)
	{
	}

	void Map::SetBlockAtIndex(s32 x, s32 y, BLOCK_TYPE type, const sPtr<PhysBlock>& obj)
	{
		Block* block = GetBlockAtIndex(x, y);
		if (!block)
		{
			Error::RaiseErrFormatted("Map::SetBlockAtIndex", "Queried block at (%d, %d) does not exist!", ERROR_ERR, x, y);
			return;
		}

		block->type = type;
		if (obj)
			block->object = obj;
		else
			block->Init(x, y);

		UpdateSurroundingTextures(x, y);
	}
	void Map::SetBlockAtIndex(const Coordinate& coord, BLOCK_TYPE type, const sPtr<PhysBlock>& obj)
	{
		SetBlockAtIndex(coord.x, coord.y, type, obj);
	}
	void Map::SetBlockAtIndex(const Vec2& pos, BLOCK_TYPE type, const sPtr<PhysBlock>& obj)
	{
		SetBlockAtIndex(WorldToMapPos(pos), type, obj);
	}

	void Map::SetBlock(Block* block, const sPtr<PhysBlock>& obj)
	{
		block->type = obj->type;
		block->object = obj;

		UpdateSurroundingTextures(block->x, block->y);
	}

	void Map::UnsetBlockAtIndex(s32 x, s32 y)
	{
		Block* block = GetBlockAtIndex(x, y);
		if (!block)
		{
			Error::RaiseErrFormatted("Map::UnsetBlockAtIndex", "Queried block at (%d, %d) does not exist!", ERROR_ERR, x, y);
			return;
		}

		UnsetBlock(block);
	}

	void Map::UnsetBlock(Block* block)
	{
		// Shouldn't need to do anything if there is no object possessed by this block
		if (block->type == BT_NONE)
			return;

		// Set the orphaned block to independent texture
		block->UpdateTexture(BLOCK_TEXTURE_NONE);

		block->type = BT_NONE;
		block->object.reset();

		UpdateSurroundingTextures(block->x, block->y);
	}

	void Map::UpdateSurroundingTextures(s32 x, s32 y)
	{
		// Update the textures of the surrounding blocks
		for (int i = x - 1; i <= x + 1; ++i)
			for (int j = y - 1; j <= y + 1; ++j)
			{
				Block* blockToUpdate = GetBlockAtIndex(i, j);
				if (blockToUpdate)
					blockToUpdate->UpdateTexture();
			}
	}

	Block* Map::GetBlockAtIndex(s32 x, s32 y)
	{
		if (x < 0 || x >= width || y < 0 || y >= height)
			return nullptr;
		return arr[y][x];
	}

	Block* Map::GetBlockAtIndex(const Coordinate& coord)
	{
		return GetBlockAtIndex(coord.x, coord.y);
	}

	Block* Map::GetBlockAtIndex(const Vec2& pos)
	{
		return GetBlockAtIndex(WorldToMapPos(pos));
	}

	Block* Map::GetBlockOfType(BLOCK_TYPE type)
	{
		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				if (arr[y][x]->type == type)
				{
					return arr[y][x];
				}
			}
		}
		return nullptr;
	}
	Block* Map::GetBlockOfType(BLOCK_TYPE type,int& x, int& y)
	{
		for (y = 0; y < height; ++y)
		{
			for (x = 0; x < width; ++x)
			{
				if (arr[y][x]->type == type)
				{
					return arr[y][x];
				}
			}
		}
		return nullptr;
	}

	Vec2 Map::MapToWorldPos(int x, int y)
	{
		return MakeVec2(
			(sCast<f32>(x) * C_blockLength) + C_blockLengthHalf - (sCast<f32>(Engine::C_windowWidth) / 2.0f),
			(sCast<f32>(y) * -C_blockLength) - C_blockLengthHalf + (sCast<f32>(Engine::C_windowHeight / 2.0f)));
	}
	Vec2 Map::MapToWorldPos(const Coordinate& coord)
	{
		return MapToWorldPos(coord.x, coord.y);
	}

	Coordinate Map::WorldToMapPos(float x, float y)
	{
		return Coordinate(
			sCast<s32>(std::lroundf((x + sCast<f32>(Engine::C_windowWidth) / 2.0f - C_blockLengthHalf) / C_blockLength)),
			sCast<s32>(std::lroundf((sCast<f32>(Engine::C_windowHeight) / 2.0f - y - C_blockLengthHalf) / C_blockLength))
		);
	}
	Coordinate Map::WorldToMapPos(const Vec2& pos)
	{
		return WorldToMapPos(pos.x, pos.y);
	}

	void Map::Init(Engine::Scene::Scene_Base* _scene, const sPtr<Engine::Graphics::Camera>& camera)
	{
		scene = _scene;

		// Initialize the blocks with game objects
		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				Block* block = arr[y][x];
				block->Init(x, y);
			}
		}
		// Initialize game objects spawned by the blocks
		// This is done here after all blocks are inited so that block entities that rely on other block
		// entities having been created already work properly.
		Singleton<LevelObjectsManager>::Instance()->Flush(OBJECTS_LAYER_FOREGROUND);
		Singleton<LevelObjectsManager>::Instance()->RunMacro(OBJECTS_LAYER_FOREGROUND, [](const sPtr<Object>& obj) -> bool {
			sPtr<BlockEntity> blockEntity = std::dynamic_pointer_cast<BlockEntity>(obj);
			if (blockEntity)
				blockEntity->Init();
			return true;
		});

		// Set the blocks textures (Kendrick)
		for (int y = 0; y < height; ++y)
			for (int x = 0; x < width; ++x)
				arr[y][x]->UpdateTexture();

		// Player is initialised here
		int x = 0;
		int y = 0;
		Block* block = GetBlockOfType(BT_PLAYER,x,y);
		if (block != nullptr)
		{
			sPtr<Player> p = Engine::Singleton<Player>::Instance();
			p->pos = MapToWorldPos(x, y);
			p->SetPosition(p->pos);
			p->camReference = camera;
			Engine::Singleton<LevelObjectsManager>::Instance()->AddObject(OBJECTS_LAYER_FOREGROUND, p);
			p->currentState = Player::S_ALIVE;
			p->InitAnimationTree();
		}
	}

	void Map::Render()
	{
		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				Block* block = arr[y][x];
				
				if (block->object)
				{
					block->object->Render();
				}
			}
		}
	}
	void Map::Update()
	{
		static unsigned char tick = 0;
		updateCooldown -= Engine::C_frametime;
		if (updateCooldown <= 0.0f)
			updateCooldown += 0.1f;
		else
			return;
		++tick;
		tick %= 2;
		BLOCK_TYPE** currgrid;
		currgrid = new BLOCK_TYPE * [height];
		for (size_t h = 0; h < height; ++h)
		{
			currgrid[h] = new BLOCK_TYPE[width];
			for (size_t w = 0; w < width; ++w)
			{
				Block* b = GetBlockAtIndex(sCast<s32>(w), sCast<s32>(h));
				currgrid[h][w] = b->type;
			}
		}

		for (size_t x = 0; x < width; ++x)
		{
			for (size_t y = 0; y < height; ++y)
			{
				Block* b = GetBlockAtIndex(sCast<s32>(x), sCast<s32>(y));
				b->SetBlockProperties(b->object);
				Block* test;
				switch (b->type)
				{
				case BT_WATER:
					//Test neighbouring
					test = GetBlockAtIndex(sCast<s32>(x), sCast<s32>(y + 1));
					//If it doesn't fall
					if (!DoBlockInteraction(b, test, currgrid) && y < height)
					{
						//Search for nearby blocks to convert
						test = GetBlockAtIndex(sCast<s32>(x - 1), sCast<s32>(y));
						if (test)
						{
							if (test->type == BT_LAVA)
							{
								DoBlockInteraction(b, test, currgrid);
								continue;
							}
						}
						test = GetBlockAtIndex(sCast<s32>(x + 1), sCast<s32>(y));
						if (test)
						{
							if (test->type == BT_LAVA)
							{
								DoBlockInteraction(b, test, currgrid);
								continue;
							}
						}
						
						//Search for nearby air blocks
						if (y >= height-1)
							continue;

						size_t waterdist = 8;
						size_t testx = x - 1;
						size_t leftDist = waterdist + 1;
						size_t rightDist = waterdist + 1;
						//Get nearest air/lava gap
						for (size_t i = 1; i < 8; ++i)
						{
							if (currgrid[y][testx] != BT_NONE && currgrid[y][testx] != BT_LAVA)
								break;
							if (currgrid[y + 1][testx] == BT_NONE || currgrid[y + 1][testx] == BT_LAVA)
							{
								leftDist = i;
								break;
							}
							--testx;
						}

						testx = x + 1;
						for (size_t i = 1; i < 8; ++i)
						{
							if (currgrid[y][testx] != BT_NONE && currgrid[y][testx] != BT_LAVA)
								break;
							if (currgrid[y + 1][testx] == BT_NONE || currgrid[y + 1][testx] == BT_LAVA)
							{
								rightDist = i;
								break;
							}
							++testx;
						}

						if (leftDist < waterdist && leftDist < rightDist)
						{
							test = GetBlockAtIndex(sCast<s32>(x - 1), sCast<s32>(y));
 							DoBlockInteraction(b, test, currgrid);
						}
						else if (rightDist < waterdist)
						{
							test = GetBlockAtIndex(sCast<s32>(x + 1), sCast<s32>(y));
							DoBlockInteraction(b, test, currgrid);
						}
					}
					break;

				case BT_LAVA:
					if (tick != 0)
						break;
					//Test neighbouring
					test = GetBlockAtIndex(sCast<s32>(x), sCast<s32>(y + 1));
					//If it doesn't fall
					if (!DoBlockInteraction(b, test, currgrid)&&y<height)
					{
						//Search for nearby blocks to convert
						test = GetBlockAtIndex(sCast<s32>(x - 1), sCast<s32>(y));
						if (test)
						{
							if (test->type == BT_WATER)
							{
								DoBlockInteraction(b, test, currgrid);
								continue;
							}
						}
						test = GetBlockAtIndex(sCast<s32>(x + 1), sCast<s32>(y));
						if (test)
						{
							if (test->type == BT_WATER)
							{
								DoBlockInteraction(b, test, currgrid);
								continue;
							}
						}

						//Search for nearby air blocks
						if (y >= height-1)
							continue;

						size_t waterdist = 8;
						size_t testx = x - 1;
						test = GetBlockAtIndex(sCast<s32>(testx), sCast<s32>(y));
						size_t leftDist = waterdist+1;
						size_t rightDist = waterdist+1;
						for (size_t i = 1; i < 8; ++i)
						{
							if (currgrid[y][testx] != BT_NONE && currgrid[y][testx] != BT_WATER)
								break;
							if (currgrid[y + 1][testx] == BT_NONE || currgrid[y + 1][testx] == BT_WATER)
							{
								leftDist = i;
								break;
							}
							--testx;
						}
						testx = x + 1;
						test = GetBlockAtIndex(sCast<s32>(testx), sCast<s32>(y));
						for (size_t i = 1; i < 8; ++i)
						{
							if (currgrid[y][testx] != BT_NONE&&currgrid[y][testx] != BT_WATER)
								break;
							if (currgrid[y + 1][testx] == BT_NONE||currgrid[y + 1][testx] == BT_WATER)
							{
								rightDist=i;
								break;
							}
							++testx;
						}
						
						if (leftDist < waterdist && leftDist < rightDist)
						{
							test = GetBlockAtIndex(sCast<s32>(x-1), sCast<s32>(y));
							DoBlockInteraction(b, test, currgrid);
						}
						else if (rightDist < waterdist)
						{
							test = GetBlockAtIndex(sCast<s32>(x + 1), sCast<s32>(y));
							DoBlockInteraction(b, test, currgrid);
						}
					}
					break;
				}
			}
		}

		for (s32 x = 0; x < width; ++x)
		{
			for (s32 y = 0; y < height; ++y)
			{
				Block* b = GetBlockAtIndex(x, y);
				if (currgrid[y][x] == BT_NONE)
					UnsetBlockAtIndex(x, y);
				else if (currgrid[y][x] != b->type)
					SetBlockAtIndex(x, y, currgrid[y][x]);
			}
		}

		for (size_t h = 0; h < height; ++h)
		{
			delete[] currgrid[h];
		}
		delete[] currgrid;
	}
	bool Map::DoBlockInteraction(Block* thisblock, Block* otherblock, BLOCK_TYPE** grid)
	{
		//DoBlockInteraction(b, test)
		if (thisblock == nullptr || otherblock == nullptr)
			return false;
		switch (thisblock->type)
		{
		case BT_WATER:
		{
			switch (otherblock->type)
			{
			case BT_NONE:
				if (grid[otherblock->y][otherblock->x] == BT_NONE)
				{
					grid[otherblock->y][otherblock->x] = thisblock->type;
					grid[thisblock->y][thisblock->x] = BT_NONE;
				}
				return true;
				break;
			case BT_LAVA:
				grid[otherblock->y][otherblock->x] = BT_ROCK;
				grid[thisblock->y][thisblock->x] = BT_NONE;
				Engine::Singleton<Engine::ParticleManager>::Instance()->AddObject(
					Engine::PARTICLE_LAYER_ABOVE_MAP,
					sPtr<Engine::ParticleSpawnerBase>(new ParticleSpawnerSmoke(MapToWorldPos(otherblock->x, otherblock->y)))
				);
				return true;
				break;
			case BT_WATER:
				//Search horizontally for air blocks
				int leftDist = -1;
				int rightDist = -1;
				for (int l = otherblock->x; l > 0; --l)
				{
					if (grid[otherblock->y][l] == BT_NONE)
					{
						leftDist = l;
						if (grid[otherblock->y][leftDist] != BT_NONE)
							leftDist = -1;
						break;
					}
					else if (grid[otherblock->y][l] != BT_WATER)
					{
						break;
					}
				}
				for (int r = otherblock->x; r < width - 1; ++r)
				{
					if (grid[otherblock->y][r] == BT_NONE)
					{
						rightDist = r;
						if (grid[otherblock->y][rightDist] != BT_NONE)
							rightDist = -1;
						break;
					}
					else if (grid[otherblock->y][r] != BT_WATER)
					{
						break;
					}
				}
				if (leftDist > 0 && (otherblock->x - leftDist) < (rightDist - otherblock->x))
				{
 					grid[otherblock->y][leftDist] = BT_WATER;
					grid[thisblock->y][thisblock->x] = BT_NONE;
					return true;
				}
				else if (rightDist > 0)
				{
					grid[otherblock->y][rightDist] = BT_WATER;
					grid[thisblock->y][thisblock->x] = BT_NONE;
					return true;
				}
				return false;
				break;
			}
		}
		break;
		case BT_LAVA:
		{
			switch (otherblock->type)
			{
			case BT_NONE:
				grid[otherblock->y][otherblock->x] = thisblock->type;
				grid[thisblock->y][thisblock->x] = BT_NONE;
				return true;
				break;
			case BT_WATER:
			{
				if (otherblock->y > thisblock->y)
				{
					grid[otherblock->y][otherblock->x] = BT_ROCK;
					grid[thisblock->y][thisblock->x] = BT_NONE;
					Engine::Singleton<Engine::ParticleManager>::Instance()->AddObject(
						Engine::PARTICLE_LAYER_ABOVE_MAP,
						sPtr<Engine::ParticleSpawnerBase>(new ParticleSpawnerSmoke(MapToWorldPos(otherblock->x, otherblock->y)))
					);
					return true;
				}
			}
			break;
			}
		}
		break;
		}
		return false;
	}

	void Map::NextLevel()
	{
		NextLevel(currLevel + 1);
	}
	void Map::NextLevel(int levelID)
	{
		Singleton<GameStateManager>::Instance()->SetIsLevelClear(false);
		currLevel = sCast<s8>(levelID);
		if ((!C_hackDemo && currLevel > C_levelLast) ||
			(C_hackDemo && currLevel > C_levelLast + 1))
		{
			Singleton<ProgramEngine>::Instance()->SwitchToScene(Engine::Scene::SCENE_CREDITS);
		}
		loadNext = true;
	}
	void Map::DeleteMap()
	{
		// Delete the Block pointers individually
		// Shouldn't have memory leaks. If you run into problems, contact Ryan.
		for (size_t i = 0; i < height; ++i)
		{
			for (size_t j = 0; j < width; ++j)
			{
				delete arr[i][j];
			}
			delete[] arr[i];
		}
		delete[] arr;
		arr = nullptr;
	}

	Map::~Map()
	{
		DeleteMap();
	}
}
