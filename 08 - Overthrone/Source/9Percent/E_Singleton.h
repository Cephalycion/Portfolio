/******************************************************************************/
/*!
\file   E_Singleton.h
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Kendrick Sim Hean Guan (100%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\brief
  This is an interface and implementation file for template class that
  creates singleton objects of arbituary types.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#pragma once
#include "E_Constants.h"

/* Forward Declarations */
namespace Engine {
	namespace Physics {
		class PhysicsManager;
	}
}

namespace Engine {

	template <typename T>
	class Singleton
	{
	protected:
		// Disable creating instances of this class
		Singleton<T>() = delete;
		virtual ~Singleton<T>() = delete;

		static sPtr<T> instance;

	public:
		// Get a ptr to the singleton-ed class instance
		static sPtr<T> Instance()
		{
			if (!instance)
				instance.reset(new T());
			return instance;
		}
		// Free memory occupied by this singleton
		static void Destroy()
		{
			instance.reset();
		}
		// Check if the singleton instance exists.
		static bool IsInitialized()
		{
			return instance.get();
		}
	};

	template <typename T>
	sPtr<T> Singleton<T>::instance = sPtr<T>();


	/* Disallowed specializations */
	template <>
	sPtr<Engine::Physics::PhysicsManager> Singleton<Engine::Physics::PhysicsManager>::Instance() = delete;
	template <>
	void Singleton<Engine::Physics::PhysicsManager>::Destroy() = delete;
}
