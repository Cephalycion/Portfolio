/******************************************************************************/
/*!
\file   E_Sprite.h
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
  This is an interface file for Sprite classes that wrap Mesh and
  Texture objects to provide various methods to render arbituary meshes
  with arbituary textures, with additional settings as required.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#pragma once
#include "pch.h"
#include "E_Asset.h"
#include "E_Transform.h"
#include "E_Physics.h"
#include <string>
#include <functional>

namespace Engine {

	using namespace Graphics;

	class Sprite
	{
	protected:
		sPtr<Mesh> meshPtr;
		sPtr<Texture> texturePtr;

		virtual void ActivateTexture() const;

	public:
		Sprite();
		Sprite(const sPtr<Mesh>& meshPtr, const sPtr<Texture>& texturePtr);


		void SetMesh(const sPtr<Mesh>& newMeshPtr);
		void SetTexture(const sPtr<Texture>& newTexturePtr);

		sPtr<Texture> GetTexture() const;

		void Render(Transform& transform, f32 transparency, const Color& colorTint);
		virtual void Update(float dt);

	};

	class TexOffsetSprite : public Sprite
	{
	protected:
		f32 uOffset;				// u & v offset about uv (0,0)
		f32 vOffset;				// to render the correct frame

		virtual void ActivateTexture() const override;

	public:
		TexOffsetSprite();
		TexOffsetSprite(const sPtr<Mesh>& meshPtr, const sPtr<Texture>& texturePtr, f32 xOffset = 0.0f, f32 yOffset = 0.0f);

		void SetUOffset(f32 newUOffset);
		f32 GetUOffset() const;
		void SetVOffset(f32 newVOffset);
		f32 GetVOffset() const;

	};

	class AnimatedSprite : public TexOffsetSprite
	{
	protected:
		const s32 rows;				// Number of Rows in the sprite sheet
		const s32 columns;			// Number of Columns in the sprite sheet
		const s32 totalFrames;		// Total number of frames in the sprite sheet

		const f32 animTime;			// Total time sprite takes to loop
		const f32 animTimePerFrame;	// Time taken by each frame

		f32 currentTime;			// Time on the current frame
		s32 currentRow;				// Current Row &
		s32 currentCol;				// Current Column to get the current frame of the spritesheet
		s32 currentFrame;			// Current frame of the animation

	public:
		AnimatedSprite();
		AnimatedSprite(const sPtr<Mesh>& meshPtr, const sPtr<Texture>& texturePtr, s32 numRows, s32 numCols, f32 runTime, s32 numFrames);

		virtual void Update(float dt);

	};

	class ScrollingSprite : public TexOffsetSprite
	{
	protected:
		Vec2 uvScrollSpeed;

	public:
		ScrollingSprite();
		ScrollingSprite(const sPtr<Mesh>& meshPtr, const sPtr<Texture>& texturePtr, const Vec2& uvScrollSpeed, const Vec2& uvStartOffset);

		virtual void Update(float dt);

	};

	class SpriteBuilder
	{
	private:
		SpriteBuilder() = delete;

	public:
		static sPtr<Sprite> CreateSprite(const sPtr<Mesh>& meshPtr, const sPtr<Texture>& texturePtr = nullptr,
			bool isAnimated = false, s32 numRows = 1, s32 numCols = 1, f32 runTime = 1.0f, s32 numFrames = 1);
		static sPtr<Sprite> CreateSpriteSquare(const std::string& texturePath = "",
			bool isAnimated = false, s32 numRows = 1, s32 numCols = 1, f32 runTime = 1.0f, s32 numFrames = 1);
		static sPtr<TexOffsetSprite> CreateTexOffsetSpriteSquare(const std::string& texturePath = "",
			const Vec2& uvLength = MakeVec2(1.0f, 1.0f), const Vec2& uvOffset = Vec2Zero());
		static sPtr<ScrollingSprite> CreateScrollSpriteSquare(const std::string& texturePath = "",
			const Vec2& uvLength = MakeVec2(1.0f, 1.0f), const Vec2& uvScrollSpeed = Vec2Zero(), const Vec2& uvStartOffset = Vec2Zero());
	};
}
