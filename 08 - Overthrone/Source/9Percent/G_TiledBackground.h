/******************************************************************************/
/*!
\file   G_TiledBackground.h
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Kendrick Sim Hean Guan (100%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\brief
  This is an interface file for a class that repeatedly tiles sprites
  across the screen to simulate an infinite background.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#pragma once
#include "pch.h"
#include "E_Object.h"

namespace Game {

	using namespace Engine;

	class TiledBackgroundObject : public Object
	{
	protected:
		Vec2 anchorPosition;
		Vec2 tileSize;
		bool tileHorizontally;
		bool tileVertically;

		Vec2 GetScreenToTileSizeRatio(const Vec2& tileSize) const;
		Vec2 CalcUVLength(const Vec2& tileSize, bool tileHorizontally, bool tileVertically) const;
		Vec2 CalcScale(const Vec2& tileSize, bool tileHorizontally, bool tileVertically) const;
		
		virtual Vec2 CalcPosThisFrame(const Vec2& camPos) const;

	public:
		TiledBackgroundObject(const std::string& textureFilepath, const Vec2& tileSize, const Vec2& position, bool tileHorizontally = true, bool tileVertically = true);

		virtual void Render() override;

		static Vec2 CalcPosBasedOnMapAndParallax(float tileHeight, float parallaxStrength, float percentHeightOnMap = 1.0f);

	};

	class MovingTiledBackgroundObject : public TiledBackgroundObject
	{
	protected:
		Vec2 offset;
		Vec2 velocity;

		virtual Vec2 CalcPosThisFrame(const Vec2& camPos) const override;

	public:
		MovingTiledBackgroundObject(const std::string& textureFilepath, const Vec2& tileSize, const Vec2& position, const Vec2& velocity, bool tileHorizontally = true, bool tileVertically = true);

		virtual void Update() override;
	};

}

