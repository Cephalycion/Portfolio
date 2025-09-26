/******************************************************************************/
/*!
\file   E_ObjectLayer.h
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Kendrick Sim Hean Guan (100%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\brief
  This is an interface and implementation file for a template class that stores
  objects of an arbituary type in separate layers.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#pragma once
#include "pch.h"
#include "E_SafeVector.h"

namespace Engine {

	template <typename OBJECT_TYPE, typename LAYER_TYPE, int NUM_LAYERS>
	class IObjectLayerManager
	{
	protected:
		std::array<std::vector<sPtr<OBJECT_TYPE>>, NUM_LAYERS> objects;

		// To safely add objects while we're looping the objects vector.
		std::array<SafeVectorModifier<sPtr<OBJECT_TYPE>>, NUM_LAYERS> safeVecModifiers;

		IObjectLayerManager();

		// Run a function on a layer.
		// Return false to remove the object.
		void SafeRunMacro(LAYER_TYPE layer, const std::function<bool(const sPtr<OBJECT_TYPE>&)>& func);
		// Run the above SafeRunMacro() definition on all layers
		void SafeRunMacro(const std::function<bool(const sPtr<OBJECT_TYPE>&)>& func);

	public:
		// Flush objects in buffer to the objects vector
		void Flush(LAYER_TYPE layer);

		// Add an object within a layer
		virtual void AddObject(LAYER_TYPE layer, const sPtr<OBJECT_TYPE>& obj);
		// Remove an object from a layer
		// Try to avoid calling this function as it may be expensive.
		void RemoveObject(LAYER_TYPE layer, const sPtr<OBJECT_TYPE>& obj);

		// Remove all objects in a layer
		void DeleteLayer(LAYER_TYPE layer);
		// Remove all objects in all layers
		void DeleteAllLayers();

		// Update all objects in all layers
		virtual void Update(float dt) = 0;
		// Render all objects in a layer
		virtual void Render(LAYER_TYPE layer) = 0;

	};


	/* Implementations */

	template<typename OBJECT_TYPE, typename LAYER_TYPE, int NUM_LAYERS>
	IObjectLayerManager<OBJECT_TYPE, LAYER_TYPE, NUM_LAYERS>::IObjectLayerManager()
	{
		for (int i = 0; i < NUM_LAYERS; ++i)
			safeVecModifiers[i].Assign(&objects[i]);
	}

	template<typename OBJECT_TYPE, typename LAYER_TYPE, int NUM_LAYERS>
	void IObjectLayerManager<OBJECT_TYPE, LAYER_TYPE, NUM_LAYERS>::Flush(LAYER_TYPE layer)
	{
		safeVecModifiers[layer].Flush();
	}

	template<typename OBJECT_TYPE, typename LAYER_TYPE, int NUM_LAYERS>
	void IObjectLayerManager<OBJECT_TYPE, LAYER_TYPE, NUM_LAYERS>::SafeRunMacro(LAYER_TYPE layer, const std::function<bool(const sPtr<OBJECT_TYPE>&)>& func)
	{
		auto& objectsVec = objects[layer];
		for (const auto& obj : objectsVec)
			if (!func(obj))
				safeVecModifiers[layer].Remove(obj);
	}

	template<typename OBJECT_TYPE, typename LAYER_TYPE, int NUM_LAYERS>
	void IObjectLayerManager<OBJECT_TYPE, LAYER_TYPE, NUM_LAYERS>::SafeRunMacro(const std::function<bool(const sPtr<OBJECT_TYPE>&)>& func)
	{
		for (int i = 0; i < NUM_LAYERS; ++i)
			SafeRunMacro(sCast<LAYER_TYPE>(i), func);
	}

	template <typename OBJECT_TYPE, typename LAYER_TYPE, int NUM_LAYERS>
	void IObjectLayerManager<OBJECT_TYPE, LAYER_TYPE, NUM_LAYERS>::AddObject(LAYER_TYPE layer, const sPtr<OBJECT_TYPE>& obj)
	{
		safeVecModifiers[layer].PushBack(obj);
	}

	template<typename OBJECT_TYPE, typename LAYER_TYPE, int NUM_LAYERS>
	void IObjectLayerManager<OBJECT_TYPE, LAYER_TYPE, NUM_LAYERS>::RemoveObject(LAYER_TYPE layer, const sPtr<OBJECT_TYPE>& obj)
	{
		safeVecModifiers[layer].Remove(obj);
	}

	template<typename OBJECT_TYPE, typename LAYER_TYPE, int NUM_LAYERS>
	void IObjectLayerManager<OBJECT_TYPE, LAYER_TYPE, NUM_LAYERS>::DeleteLayer(LAYER_TYPE layer)
	{
		objects[layer].clear();
		safeVecModifiers[layer].Clear();
	}
	template<typename OBJECT_TYPE, typename LAYER_TYPE, int NUM_LAYERS>
	void IObjectLayerManager<OBJECT_TYPE, LAYER_TYPE, NUM_LAYERS>::DeleteAllLayers()
	{
		for (auto& v : objects)
			v.clear();
	}

	template<typename OBJECT_TYPE, typename LAYER_TYPE, int NUM_LAYERS>
	void IObjectLayerManager<OBJECT_TYPE, LAYER_TYPE, NUM_LAYERS>::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		for (auto& safeVecModifier : safeVecModifiers)
			safeVecModifier.Flush();
	}

}
