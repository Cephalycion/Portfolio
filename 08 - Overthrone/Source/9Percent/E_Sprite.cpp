/******************************************************************************/
/*!
\file   E_Sprite.cpp
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Marc Alviz Evangelista (75%)
\par    email: marcalviz.e\@digipen.edu
\par    DigiPen login: marcalviz.e

\author Kendrick Sim Hean Guan (25%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\brief
  This is the implementation file for Sprite classes that wrap Mesh and
  Texture objects to provide various methods to render arbituary meshes
  with arbituary textures, with additional settings as required.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#include "pch.h"
#include "E_Object.h"
#include "E_Camera.h"
#include "E_Asset.h"
#include "E_Sprite.h"

namespace Engine {

	Sprite::Sprite()
	{
	}

	Sprite::Sprite(const sPtr<Mesh>& meshPtr, const sPtr<Texture>& texturePtr)
		: meshPtr(meshPtr)
		, texturePtr(texturePtr)
	{
	}

	void Sprite::SetMesh(const sPtr<Mesh>& newMeshPtr)
	{
		meshPtr = newMeshPtr;
	}

	void Sprite::SetTexture(const sPtr<Texture>& newTexturePtr)
	{
		texturePtr = newTexturePtr;
	}

	sPtr<Texture> Sprite::GetTexture() const
	{
		return texturePtr;
	}

	void Sprite::ActivateTexture() const
	{
		texturePtr->ActivateTexture();
	}

	void Sprite::Render(Transform& transform, f32 transparency, const Color& colorTint)
	{
		if (texturePtr)
		{
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			ActivateTexture();
		}
		else
			AEGfxSetRenderMode(AE_GFX_RM_COLOR);

		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(transparency);
		AEGfxSetColorToMultiply(colorTint);

		Mtx33* transformByCamera = Singleton<CameraManager>::Instance()->ApplyTransform(transform);
		AEGfxSetTransform(transformByCamera->m);

		meshPtr->Render();
	}

	void Sprite::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);
	}


	TexOffsetSprite::TexOffsetSprite()
		: Sprite()
		, uOffset(0.0f)
		, vOffset(0.0f)
	{
	}
	TexOffsetSprite::TexOffsetSprite(const sPtr<Mesh>& meshPtr, const sPtr<Texture>& texturePtr, f32 xOffset, f32 yOffset)
		: Sprite(meshPtr, texturePtr)
		, uOffset(xOffset)
		, vOffset(yOffset)
	{
	}

	void TexOffsetSprite::SetUOffset(f32 newUOffset)
	{
		uOffset = newUOffset;
	}
	f32 TexOffsetSprite::GetUOffset() const
	{
		return uOffset;
	}
	void TexOffsetSprite::SetVOffset(f32 newVOffset)
	{
		vOffset = newVOffset;
	}
	f32 TexOffsetSprite::GetVOffset() const
	{
		return vOffset;
	}

	void TexOffsetSprite::ActivateTexture() const
	{
		texturePtr->ActivateTexture(uOffset, vOffset);
	}


	AnimatedSprite::AnimatedSprite()
		: TexOffsetSprite()
		, rows(1)
		, columns(1)
		, totalFrames(1)
		, animTime(1.0f)
		, animTimePerFrame(1.0f)
		, currentTime(0.0f)
		, currentRow(0)
		, currentCol(0)
		, currentFrame(0)
	{
	}

	AnimatedSprite::AnimatedSprite(const sPtr<Mesh>& meshPtr, const sPtr<Texture>& texturePtr, s32 numRows, s32 numCols, f32 runTime, s32 numFrames)
		: TexOffsetSprite(meshPtr, texturePtr, 0.0f, 0.0f)
		, rows(numRows)
		, columns(numCols)
		, totalFrames(numFrames)
		, animTime(runTime)
		, animTimePerFrame(runTime / (numRows * numCols))
		, currentRow(0)
		, currentCol(0)
		, currentFrame(0)
		, currentTime(0.0f)
	{

	}

	void AnimatedSprite::Update(float dt)
	{
		// Update the spritesheets variables here
		// Update current frames timing
		currentTime += dt;
		if (currentTime > animTimePerFrame)
		{
			currentTime = 0; // reset current time

			// update current row/current column
			// column first
			if (currentCol + 1 >= columns)
			{
				// if reach end then go down a row
				currentCol = 0;
				if (currentRow + 1 >= rows)	// Animation loops back to start
				{
					currentRow = 0;
				}
				else
				{
					++currentRow;
				}
			}
			else
			{
				++currentCol;
				// check if it reaches the end of non square sprite sheets
				if (currentRow * rows + currentCol >= totalFrames)
				{
					// reset back to 0/0
					currentCol = 0;
					currentRow = 0;
				}
			}

			// then update the offsets
			uOffset = (f32)(currentCol) / (f32)(columns);
			vOffset = (f32)(currentRow) / (f32)(rows);
		}
	}


	ScrollingSprite::ScrollingSprite()
		: TexOffsetSprite()
		, uvScrollSpeed(Vec2Zero())
	{
	}
	ScrollingSprite::ScrollingSprite(const sPtr<Mesh>& meshPtr, const sPtr<Texture>& texturePtr, const Vec2& uvScrollSpeed, const Vec2& uvStartOffset)
		: TexOffsetSprite(meshPtr, texturePtr, uvStartOffset.x, uvStartOffset.y)
		, uvScrollSpeed(uvScrollSpeed)
	{
	}

	void ScrollingSprite::Update(float dt)
	{
		uOffset = Util::Wrap(uOffset + uvScrollSpeed.x * dt, 0.0f, 1.0f);
		vOffset = Util::Wrap(vOffset + uvScrollSpeed.y * dt, 0.0f, 1.0f);
	}


	sPtr<Sprite> SpriteBuilder::CreateSprite(const sPtr<Mesh>& meshPtr, const sPtr<Texture>& texturePtr, bool isAnimated, s32 numRows, s32 numCols, f32 runTime, s32 numFrames)
	{
		if (isAnimated)
			return sPtr<Sprite>(new AnimatedSprite(
				meshPtr,
				texturePtr,
				numRows,
				numCols,
				runTime,
				numFrames
			));
		else
			return sPtr<Sprite>(new Sprite(
				meshPtr,
				texturePtr
			));
	}
	sPtr<Sprite> SpriteBuilder::CreateSpriteSquare(const std::string& texturePath, bool isAnimated, s32 numRows, s32 numCols, f32 runTime, s32 numFrames)
	{
		if (isAnimated)
			return sPtr<Sprite>(new AnimatedSprite(
				AssetManager::GetMeshSpriteSheet(numRows, numCols),
				texturePath.empty() ? nullptr : AssetManager::GetTexture(texturePath),
				numRows,
				numCols,
				runTime,
				numFrames
			));
		else
			return sPtr<Sprite>(new Sprite(
				AssetManager::GetMeshSquare(),
				texturePath.empty() ? nullptr : AssetManager::GetTexture(texturePath)
			));
	}
	sPtr<TexOffsetSprite> SpriteBuilder::CreateTexOffsetSpriteSquare(const std::string& texturePath, const Vec2& uvLength, const Vec2& uvOffset)
	{
		return sPtr<TexOffsetSprite>(new TexOffsetSprite(
			AssetManager::GetMeshSquareCustomUV(uvLength.x, uvLength.y),
			texturePath.empty() ? nullptr : AssetManager::GetTexture(texturePath),
			uvOffset.x,
			uvOffset.y
		));
	}
	sPtr<ScrollingSprite> SpriteBuilder::CreateScrollSpriteSquare(const std::string& texturePath,
		const Vec2& uvLength, const Vec2& uvScrollSpeed, const Vec2& uvStartOffset)
	{
		return sPtr<ScrollingSprite>(new ScrollingSprite(
			AssetManager::GetMeshSquareCustomUV(uvLength.x, uvLength.y),
			texturePath.empty() ? nullptr : AssetManager::GetTexture(texturePath),
			uvScrollSpeed,
			uvStartOffset
		));
	}
}