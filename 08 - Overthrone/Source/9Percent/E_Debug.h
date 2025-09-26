/******************************************************************************/
/*!
\file   E_Debug.h
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Kendrick Sim Hean Guan (100%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\brief
  This is an interface file for a system that prints variables to the screen
  for debug purposes.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#include "E_AEEngineInterface.h"
#include "E_Singleton.h"
#include "E_Text.h"
#include <string>
#include <unordered_map>

namespace Engine {

	namespace INTERNAL {

		// Base class to be able to be pointed to by pointers
		class VariableTracker_Base
		{
		protected:
			std::string name;

			virtual void AppendVariableToStr(std::string* str) = 0;

		public:
			VariableTracker_Base(const std::string& name)
				: name(name) {}

			void PrintToStr(std::string* out);
		};

		// Intermediate template class to consolidate shared template code
		// between the child classes
		template <typename T>
		class VariableTracker_TemplateBase : public VariableTracker_Base
		{
		protected:
			// Convert variable to string.
			// Override this if necessary in your custom specialization
			virtual void AppendVariableToStr(std::string* str) override
			{
				str->append(std::to_string(*GetVariable()));
			}
			
			virtual const T* GetVariable() const = 0;

		public:
			VariableTracker_TemplateBase(const std::string& name)
				: VariableTracker_Base(name) {}
		};

		template <typename T>
		class VariableTracker : public VariableTracker_TemplateBase<T>
		{
		protected:
			const T* ptr;

			virtual const T* GetVariable() const override
			{
				return ptr;
			}

		public:
			VariableTracker(const std::string& name, const T* ptr)
				: VariableTracker_TemplateBase<T>(name), ptr(ptr) {}

			bool IsEqual(const T* otherPtr)
			{
				return ptr == otherPtr;
			}
		};

		template <typename T>
		class VariableTracker_Value : public VariableTracker_TemplateBase<T>
		{
		protected:
			T value;

			virtual const T* GetVariable() const override
			{
				return &value;
			}

		public:
			VariableTracker_Value(const std::string& name, const T value)
				: VariableTracker_TemplateBase<T>(name), value(value) {}
		};

		/* Specializations */
		template<>
		void VariableTracker_TemplateBase<Vec2>::AppendVariableToStr(std::string* str);
	}

	class Debug
	{
	protected:
		std::vector<sPtr<INTERNAL::VariableTracker_Base>> trackersVec;
		std::vector<sPtr<INTERNAL::VariableTracker_Base>> valuesVec;

		// For rendering each variable tracker
		Text text;

	public:
		Debug();

		// Track a persistant variable.
		template<typename T>
		static void AddTracker(const std::string& name, const T* ptr)
		{
			Singleton<Debug>::Instance()->trackersVec.emplace_back(
				new INTERNAL::VariableTracker<T>(name, ptr)
			);
		}
		template<typename T>
		static void AddValue(const std::string& name, const T value)
		{
			Singleton<Debug>::Instance()->valuesVec.emplace_back(
				new INTERNAL::VariableTracker_Value<T>(name, value)
			);
		}

		template<typename T>
		static void RemoveTracker(const T* ptr)
		{
			auto dInstance = Singleton<Debug>::Instance();

			// Find the tracker pointing to the ptr
			auto trackerIter = std::find_if(dInstance->trackersVec.begin(), dInstance->trackersVec.end(),
				[ptr](const sPtr<INTERNAL::VariableTracker_Base>& tracker) -> bool {
					return sCast<INTERNAL::VariableTracker<T>*>(tracker.get())->IsEqual(ptr);
				}
			);

			if (trackerIter != dInstance->trackersVec.end())
				dInstance->trackersVec.erase(trackerIter);
		}

		void RemoveAllValueTrackers();
		void RenderAll();
	};
}
