/******************************************************************************/
/*!
\file   E_Asset.h
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
  This is an interface file for creating meshes, textures and fonts.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#pragma once
#include "pch.h"
#include "E_Constants.h"
#include <AEEngine.h>
#include <unordered_map>
#include <string>

namespace Engine { namespace Graphics {

	class Mesh
	{
	protected:
		// BugFix: Use raw pointer because we need to call AEGfxMeshFree() to free the memory properly,
		//		     which deletes the memory block.
		//		   Smart pointers will call c++ delete on the pointer again after that call, causing a crash.
		AEGfxVertexList* mesh;
		AEGfxMeshDrawMode drawMode;

	public:
		Mesh(AEGfxVertexList* mesh, AEGfxMeshDrawMode drawMode);
		~Mesh();
		
		void Render() const;
	};

	class Texture
	{
	protected:
		AEGfxTexture* texture;

	public:
		Texture(AEGfxTexture* texture);
		~Texture();

		void ActivateTexture() const;
		void ActivateTexture(f32 xOffset, f32 yOffset) const;
	};
	
	class AssetManager
	{
	protected:
		std::unordered_map<std::string, sPtr<Mesh>> meshMap;
		std::unordered_map<std::string, sPtr<Texture>> textureMap;
		std::unordered_map<std::string, s8> fontMap;

		// Searches the map of loaded meshes for requested mesh. If not found, runs
		// the supplied CreateMeshFunc and inserts it into the map.
		static sPtr<Mesh> LoadMesh(const std::string& meshName, std::function<AEGfxVertexList*()> CreateMeshFunc);

	public:
		~AssetManager();

		static sPtr<Mesh> GetMeshSquare();
		static sPtr<Mesh> GetMeshCircle(int sides);
		static sPtr<Mesh> GetMeshSpriteSheet(s32 numRows, s32 numCols);
		static sPtr<Mesh> GetMeshSquareCustomUV(f32 uLength, f32 vLength);

		static sPtr<Texture> GetTexture(const std::string& textureFile);
		
		// Font files should be placed within Assets/Fonts/
		static s8 GetFont(const std::string& fontFile);
	};
}}
