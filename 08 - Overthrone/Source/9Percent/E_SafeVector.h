/******************************************************************************/
/*!
\file   E_SafeVector.h
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Kendrick Sim Hean Guan (100%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\brief
  This is an interface and implementation file for a class that queues
  additions/removals to/from a std::vector, to fix crashes that happen
  when these changes occur while another part of the code is still
  looping through the std::vector.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#pragma once
#include "pch.h"

namespace Engine {

	// Provide functions that safely add objects to vectors, even
	// while looping the vector.
	template <typename ElementType>
	class SafeVectorModifier
	{
	protected:
		// Pointer to the working vector.
		std::vector<ElementType>* vec;
		// Buffers all elements to be added to the working vector at a
		// later point in time.
		std::set<ElementType> addBuffer;
		// Buffers all elements to be removed from the working vector
		// at a later point in time.
		std::set<ElementType> removeBuffer;

	public:
		SafeVectorModifier(std::vector<ElementType>* vec = nullptr);

		// Assign another vector to this adder
		void Assign(std::vector<ElementType>* newVec);

		void PushBack(const ElementType& e);
		void Remove(const ElementType& e);

		// Flush changes into the vector. Call this only when no code
		// is looping the vector.
		void Flush();
		// Remove all pending changes without flushing them into the vector.
		void Clear();

	};

	/* Implementations */

	template<typename ElementType>
	SafeVectorModifier<ElementType>::SafeVectorModifier(std::vector<ElementType>* vec)
		: vec(vec)
	{
	}

	template<typename ElementType>
	void SafeVectorModifier<ElementType>::Assign(std::vector<ElementType>* newVec)
	{
		Flush();
		vec = newVec;
	}

	template<typename ElementType>
	void SafeVectorModifier<ElementType>::PushBack(const ElementType& e)
	{
		if (!addBuffer.insert(e).second)
			Error::RaiseErr("SafeVectorModifier<ElementType>::PushBack", "Element already queued for insertion. This duplicate element will not be added!", ERROR_ERR);
	}

	template<typename ElementType>
	inline void SafeVectorModifier<ElementType>::Remove(const ElementType& e)
	{
		if (!removeBuffer.insert(e).second)
			Error::RaiseErr("SafeVectorModifier<ElementType>::Remove", "Element already queued for removal. Please fix logic to not call this twice and avoid this performance penalty.", ERROR_WARN);
	}

	template<typename ElementType>
	void SafeVectorModifier<ElementType>::Flush()
	{
		if (!removeBuffer.empty())
		{
			for (const auto& e : removeBuffer)
			{
				auto iter = std::find(vec->begin(), vec->end(), e);
#ifdef _DEBUG
				if (iter != vec->end())
					vec->erase(iter);
				else
					Error::RaiseErr("SafeVectorModifier<ElementType>::Flush", "Failed to find element to remove!");
#else
				vec->erase(iter);
#endif
			}
			removeBuffer.clear();
		}

		if (!addBuffer.empty())
		{
			for (const auto& e : addBuffer)
				vec->emplace_back(e);
			addBuffer.clear();
		}
	}

	template <typename ElementType>
	void SafeVectorModifier<ElementType>::Clear()
	{
		addBuffer.clear();
		removeBuffer.clear();
	}

}
