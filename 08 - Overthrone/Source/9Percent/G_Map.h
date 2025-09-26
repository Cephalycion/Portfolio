/******************************************************************************/
/*!
\file   G_Map.h
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
  This is the interface file for all member functions of a class called
  Map, along with different utility functions for the map.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#pragma once
#include "pch.h"
#include "G_Block.h"
#include "E_SceneBase.h"
#include "E_Camera.h"
#include "E_Object.h"

namespace Game
{
    struct Coordinate
    {
    public:
        int x, y;

        Coordinate();
        Coordinate(int x, int y);
    };

    class Map
    {
    protected:
        // Update the textures of the blocks in a 3x3 area around this coordinate
        void UpdateSurroundingTextures(s32 x, s32 y); // By Kendrick

    public:
        Block*** arr;
        size_t width, height;
        Engine::Scene::Scene_Base* scene;
        bool loaded = false;
        bool loadNext = false;
        s8 currLevel = 1;
        float updateCooldown = 0.0f;

        Block* GetBlockAtIndex(s32 x, s32 y);
        Block* GetBlockAtIndex(const Coordinate& coord);
        Block* GetBlockAtIndex(const Vec2& pos);
        Block* GetBlockOfType(BLOCK_TYPE type);
        Block* GetBlockOfType(BLOCK_TYPE type, int& x, int& y);

        // Sets a block to the block type, init-ing and updating the textures of the blocks around it.
        // Specifying an obj will force the block to be set to that obj.
        void SetBlockAtIndex(s32 x, s32 y, BLOCK_TYPE type, const sPtr<PhysBlock>& obj = nullptr); // By Kendrick
        void SetBlockAtIndex(const Coordinate& coord, BLOCK_TYPE type, const sPtr<PhysBlock>& obj = nullptr); // By Kendrick
        void SetBlockAtIndex(const Vec2& pos, BLOCK_TYPE type, const sPtr<PhysBlock>& obj = nullptr); // By Kendrick
        // Sets a block to the specified object, updating the textures
        // of the blocks around it.
        void SetBlock(Block* block, const sPtr<PhysBlock>& obj);

        // Removes the object from the block, if any.
        void UnsetBlockAtIndex(s32 x, s32 y); // By Kendrick
        // Removes the object from the specified block, if any.
        void UnsetBlock(Block* block); // By Kendrick

        Vec2 MapToWorldPos(int x, int y);
        Vec2 MapToWorldPos(const Coordinate& coord);
        Coordinate WorldToMapPos(float x, float y);
        Coordinate WorldToMapPos(const Vec2& pos);

        void Init(Engine::Scene::Scene_Base* _scene, const sPtr<Engine::Graphics::Camera>& camera);
        void Render();
        void Update();
        bool DoBlockInteraction(Block* thisblock, Block* otherblock, BLOCK_TYPE** grid);

        void NextLevel();
        void NextLevel(int levelID);
        void DeleteMap();

        Map() : loaded(false), width(0), height(0), arr(nullptr), scene{nullptr} {};
        ~Map();
    };
}