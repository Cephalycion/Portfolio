/******************************************************************************/
/*!
\file   E_Asset.cpp
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Kendrick Sim Hean Guan (80%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\author Marc Alviz Evangelista (20%)
\par    email: marcalviz.e\@digipen.edu
\par    DigiPen login: marcalviz.e

\brief
  This is the implementation file for creating meshes, textures and fonts.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#include "pch.h"
#include "E_Asset.h"

namespace Engine { namespace Graphics {
	
	Mesh::Mesh(AEGfxVertexList* mesh, AEGfxMeshDrawMode drawMode)
		: mesh(mesh)
		, drawMode(drawMode)
	{
	}
	Mesh::~Mesh()
	{
		AEGfxMeshFree(mesh);
	}

	void Mesh::Render() const
	{
		AEGfxMeshDraw(mesh, drawMode);
	}


	AssetManager::~AssetManager()
	{
		for (const std::pair<std::string, s8>& fontIter : fontMap)
			AEGfxDestroyFont(fontIter.second);
	}

	sPtr<Mesh> AssetManager::LoadMesh(const std::string& meshName, std::function<AEGfxVertexList* ()> CreateMeshFunc)
	{
		auto aManager = Singleton<AssetManager>::Instance();

		auto meshIter = aManager->meshMap.find(meshName);
		if (meshIter != aManager->meshMap.end())
			return meshIter->second;

		// Create and insert the mesh
		AEGfxVertexList* vertexesPtr = CreateMeshFunc();
#ifdef _DEBUG
		if (!vertexesPtr)
		{
			Error::RaiseErrFormatted("AssetManager::LoadMesh", "CreateMeshFunc for %s failed!", ERROR_ERR, meshName.c_str());
			return nullptr;
		}
#endif
		sPtr<Mesh> meshPtr(new Mesh(vertexesPtr, AE_GFX_MDM_TRIANGLES));
		aManager->meshMap.insert(std::make_pair(meshName, meshPtr));

		return meshPtr;
	}

	sPtr<Mesh> AssetManager::GetMeshSquare()
	{
		return LoadMesh("INTERNAL_square", []() -> AEGfxVertexList* {
			AEGfxMeshStart();
			AEGfxTriAdd(
				-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,	// Bot-Left
				0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,	// Bot-Right
				-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f		// Top-Left
			);
			AEGfxTriAdd(
				0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,	// Bot-Right
				0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,		// Top-Right
				-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f		// Top-Left
			);
			return AEGfxMeshEnd();
		});
	}
	sPtr<Mesh> AssetManager::GetMeshCircle(int sides = 360)
	{
		return LoadMesh("INTERNAL_circle", [sides]() -> AEGfxVertexList* {
			AEGfxMeshStart();
			float angle = AEDegToRad(360.0f) / sCast<float>(sides);
			for (int i = 0; i < sides; ++i)
			{
				Vec2 spos = MakeVec2(cosf(angle * sCast<float>(i)), sinf(angle * sCast<float>(i)));
				Vec2 epos = MakeVec2(cosf(angle * sCast<float>(i + 1)), sinf(angle * sCast<float>(i + 1)));
				float div = 4;
				AEGfxTriAdd(
					spos.x / div, spos.y / div, 0xFFFFFFFF, spos.x / div + 0.5f, spos.y / div + 0.5f,	// L
					epos.x / div, epos.y / div, 0xFFFFFFFF, epos.x / div + 0.5f, epos.y / div + 0.5f,	// R
					0.0f, 0.0f, 0xFFFFFFFF, 0.5f, 0.5f		// center
				);

			}
			return AEGfxMeshEnd();
		});
	}

	sPtr<Mesh> AssetManager::GetMeshSpriteSheet(s32 numRows = 1, s32 numCols = 1)
	{
		return LoadMesh("squareSheet" + std::to_string(numRows) + "x" + std::to_string(numCols), [numRows, numCols]() -> AEGfxVertexList* {
			AEGfxMeshStart();
			AEGfxTriAdd(
				-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, (f32)(1.0f / (f32)numRows),							// Bot-Left
				0.5f, -0.5f, 0xFFFFFFFF, (f32)(1.0f / (f32)numCols), (f32)(1.0f / (f32)numRows),	// Bot-Right
				-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f													// Top-Left
			);
			AEGfxTriAdd(
				0.5f, -0.5f, 0xFFFFFFFF, (f32)(1.0f / (f32)numCols), (f32)(1.0f / (f32)numRows),	// Bot-Right
				0.5f, 0.5f, 0xFFFFFFFF, (f32)(1.0f / (f32)numCols), 0.0f,							// Top-Right
				-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f													// Top-Left
			);
			return AEGfxMeshEnd();
		});
	}

	sPtr<Mesh> AssetManager::GetMeshSquareCustomUV(f32 uLength, f32 vLength)
	{
		return LoadMesh("squareScroll" + std::to_string(uLength) + "x" + std::to_string(vLength), [uLength, vLength]() -> AEGfxVertexList* {
			AEGfxMeshStart();

			AEGfxTriAdd(
				-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, vLength,	// Bot-Left
				0.5f, -0.5f, 0xFFFFFFFF, uLength, vLength,	// Bot-Right
				-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f			// Top-Left
			);
			AEGfxTriAdd(
				0.5f, -0.5f, 0xFFFFFFFF, uLength, vLength,	// Bot-Right
				0.5f, 0.5f, 0xFFFFFFFF, uLength, 0.0f,		// Top-Right
				-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f			// Top-Left
			);

			return AEGfxMeshEnd();
		});
	}

	sPtr<Texture> AssetManager::GetTexture(const std::string& textureFile)
	{
		auto aManager = Singleton<AssetManager>::Instance();

		auto textureIter = aManager->textureMap.find(textureFile);
		if (textureIter != aManager->textureMap.end())
			return textureIter->second;

		AEGfxTexture* texture = AEGfxTextureLoad((C_assetTextureFilepath + textureFile).c_str());
		if (!texture)
		{
			Error::RaiseErrFormatted("AssetManager::GetTexture", "Failed to load texture %s!", ERROR_ERR, (C_assetTextureFilepath + textureFile).c_str());
			// TODO (Kendrick): Create bad texture programmatically
			return nullptr;
		}
		else
		{
			sPtr<Texture> texturePtr(new Texture(texture));
			aManager->textureMap.insert(std::make_pair(textureFile, texturePtr));
			return texturePtr;
		}
	}


	Texture::Texture(AEGfxTexture* texture)
		: texture(texture)
	{
	}
	Texture::~Texture()
	{
		AEGfxTextureUnload(texture);
	}

	void Texture::ActivateTexture() const
	{
		AEGfxTextureSet(texture, 0.0f, 0.0f);
	}

	void Texture::ActivateTexture(f32 xOffset, f32 yOffset) const
	{
		AEGfxTextureSet(texture, xOffset, yOffset);
	}


	s8 AssetManager::GetFont(const std::string& fontFile)
	{
		auto aManager = Singleton<AssetManager>::Instance();

		auto fontIter = aManager->fontMap.find(fontFile);
		if (fontIter != aManager->fontMap.end())
			return fontIter->second;

		s8 fontId = AEGfxCreateFont((C_assetFontFilepath + fontFile).c_str(), C_assetFontSize);
		if (fontId < 0)
		{
			Error::RaiseErrFormatted("GetFont", "Failed to open font file %s!", ERROR_ERR, (C_assetFontFilepath + fontFile).c_str());
			return fontId;
		}

		aManager->fontMap.insert(std::make_pair(fontFile, fontId));
		return fontId;
	}
}}
