/******************************************************************************/
/*!
\file   E_Error.h
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Kendrick Sim Hean Guan (100%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\brief
  This is an interface file for a system that prints error messages to the
  screen.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#pragma once
#include "E_Text.h"
#include <vector>

namespace Engine {
	namespace INTERNAL {
		class Error_Log;
	}


	enum ERROR_PRIORITYLEVEL
	{
		ERROR_ERR = 0,
		ERROR_WARN,
		ERROR_INFO
	};

	class Error
	{
	protected:
		std::vector<INTERNAL::Error_Log> logVec;
		s8 fontId;

		void RaiseErr_Internal(const std::string& caller, const std::string& msg, ERROR_PRIORITYLEVEL priorityLevel);

	public:
		Error();

		static void RaiseErr(const std::string& caller, const std::string& msg, ERROR_PRIORITYLEVEL priorityLevel = ERROR_ERR);
		// It's too bad C++ doesn't really have a nice alternative to sprintf(). Gotta do it C style.
		//
		// Note: This function doesn't support std::string, or any C++ types I believe.
		//       If you see corrupted characters, check if you've passed in a std::string for a %s. If so, change it to std::string.c_str().
		static void RaiseErrFormatted(const std::string& caller, const std::string& msg, ERROR_PRIORITYLEVEL priorityLevel = ERROR_ERR, ...);

		void Update(f32 dt);
		void RenderAll();
	};

	namespace INTERNAL {
		class Error_Log
		{
		protected:
			ERROR_PRIORITYLEVEL priority;
			sPtr<Text> text;

			f32 elapsedTime;

			static Color GetColor(ERROR_PRIORITYLEVEL priority);

		public:
			Error_Log(const std::string& msg, ERROR_PRIORITYLEVEL priority, s8 fontId);
			Error_Log(const Error_Log& other);
			Error_Log(Error_Log&& other) noexcept;
			Error_Log& operator=(const Error_Log& other);

			// Returns true if the error has been displayed for long enough
			bool IsExpired() const;

			void Update(f32 dt);
			void Render(f32 posY);
		};
	}
}
