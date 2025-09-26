/******************************************************************************/
/*!
\file   G_TiledBackground.cpp
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Kendrick Sim Hean Guan (100%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\brief
  This is the implementation file for a class that repeatedly tiles sprites
  across the screen to simulate an infinite background.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#include "pch.h"
#include "G_TiledBackground.h"
#include "E_Camera.h"
#include "G_Map.h"

namespace Game {

	TiledBackgroundObject::TiledBackgroundObject(const std::string& textureFilepath, const Vec2& tileSize, const Vec2& position, bool tileHorizontally, bool tileVertically)
		: Object(
			Transform(position, 0.0f, CalcScale(tileSize, tileHorizontally, tileVertically)),
			SpriteBuilder::CreateTexOffsetSpriteSquare(textureFilepath, CalcUVLength(tileSize, tileHorizontally, tileVertically)),
			1.0f, C_colorWhite
		)
		, anchorPosition(position)
		, tileSize(tileSize)
		, tileHorizontally(tileHorizontally)
		, tileVertically(tileVertically)
	{

	}

	Vec2 TiledBackgroundObject::GetScreenToTileSizeRatio(const Vec2& _tileSize) const
	{
		return MakeVec2(
			Util::RoundUp(sCast<f32>(C_windowWidth) / _tileSize.x) + 2.0f, // Add 2.0f offset as half of this object might be off screen depending on cam pos
			Util::RoundUp(sCast<f32>(C_windowHeight) / _tileSize.y) + 2.0f
		);
	}
	Vec2 TiledBackgroundObject::CalcUVLength(const Vec2& _tileSize, bool _tileHorizontally, bool _tileVertically) const
	{
		Vec2 tileSizeToScreenRatio = GetScreenToTileSizeRatio(_tileSize);
		return MakeVec2(
			(_tileHorizontally ? tileSizeToScreenRatio.x : 1.0f),
			(_tileVertically ? tileSizeToScreenRatio.y : 1.0f)
		);
	}
	Vec2 TiledBackgroundObject::CalcScale(const Vec2& _tileSize, bool _tileHorizontally, bool _tileVertically) const
	{
		Vec2 tileSizeToScreenRatio = GetScreenToTileSizeRatio(_tileSize);
		return MakeVec2(
			(_tileHorizontally ? _tileSize.x * tileSizeToScreenRatio.x : _tileSize.x),
			(_tileVertically ? _tileSize.y * tileSizeToScreenRatio.y : _tileSize.y)
		);
	}

	Vec2 TiledBackgroundObject::CalcPosThisFrame(const Vec2& camPos) const
	{
		return MakeVec2(
			(tileHorizontally ? Util::RoundDown((camPos.x + anchorPosition.x + tileSize.x * 0.5f) / tileSize.x) * tileSize.x : anchorPosition.x),
			(tileVertically ? Util::RoundDown((camPos.y + anchorPosition.y + tileSize.y * 0.5f) / tileSize.y) * tileSize.y : anchorPosition.y)
		);
	}

	void TiledBackgroundObject::Render()
	{
		if (tileHorizontally || tileVertically)
		{
			sPtr<const Camera> cam = Singleton<CameraManager>::Instance()->GetActiveCam();
			Vec2 camPos = (cam ? cam->GetPosition() : Vec2Zero());

			SetPosition(CalcPosThisFrame(camPos));
		}

		Object::Render();
	}

	Vec2 TiledBackgroundObject::CalcPosBasedOnMapAndParallax(float tileHeight, float parallaxStrength, float percentHeightOnMap)
	{
		float mapBottom = Singleton<Map>::Instance()->height * -C_blockLength + C_windowHeight;
		float posBottomHeight = mapBottom * parallaxStrength + tileHeight * 0.5f - C_windowHeight * 0.5f;
		float posTopHeight = C_windowHeight * 0.5f - tileHeight * 0.5f;
		return MakeVec2(0.0f, Util::Lerp(posTopHeight, posBottomHeight, percentHeightOnMap));
	}


	MovingTiledBackgroundObject::MovingTiledBackgroundObject(const std::string& textureFilepath, const Vec2& tileSize, const Vec2& position, const Vec2& velocity, bool tileHorizontally, bool tileVertically)
		: TiledBackgroundObject(textureFilepath, tileSize, position, tileHorizontally, tileVertically)
		, offset(Vec2Zero())
		, velocity(velocity)
	{
	}

	Vec2 MovingTiledBackgroundObject::CalcPosThisFrame(const Vec2& camPos) const
	{
		return TiledBackgroundObject::CalcPosThisFrame(camPos) + offset;
	}

	void MovingTiledBackgroundObject::Update()
	{
		offset += velocity * C_frametime;
		offset.x = Util::Wrap(offset.x, -tileSize.x * 0.5f, tileSize.x * 0.5f);
		offset.y = Util::Wrap(offset.y, -tileSize.y * 0.5f, tileSize.y * 0.5f);
	}

}
