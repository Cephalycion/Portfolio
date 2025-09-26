/******************************************************************************/
/*!
\file   E_Error.cpp
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Kendrick Sim Hean Guan (100%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\brief
  This is the implementation file for a system that prints error messages to the
  screen.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#include "pch.h"
#include "E_Asset.h"
#include <iostream>
#include <cstdarg>

namespace Engine {

	Error::Error()
		: fontId(AssetManager::GetFont(C_errFontFilepath))
	{
	}

	void Error::RaiseErr_Internal(const std::string& caller, const std::string& msg, ERROR_PRIORITYLEVEL priorityLevel)
	{
		static const char* prefix[] = {
			"ERR - ",
			"WARN - ",
			"INFO - "
		};
		
		// Bug Fix: Output msg.c_str() to fix erroneous block characters printed to cout when passed
		//			a string made by vsprintf() for some reason. Don't know why that happens since
		//			there's a \0 already, but this fixes it.
		std::cout << prefix[priorityLevel] << caller << " : " << msg.c_str() << '\n';

		logVec.emplace_back(prefix[priorityLevel] + caller + " : " + msg, priorityLevel, fontId);

		// Remove oldest entry if we've reached the max error count
		if (logVec.size() > C_errMaxLogEntries)
			logVec.erase(logVec.begin());
	}

	void Error::RaiseErr(const std::string& caller, const std::string& msg, ERROR_PRIORITYLEVEL priorityLevel)
	{
		Singleton<Error>::Instance()->RaiseErr_Internal(caller, msg, priorityLevel);
	}

	void Error::RaiseErrFormatted(const std::string& caller, const std::string& msg, ERROR_PRIORITYLEVEL priorityLevel, ...)
	{
		// Create va_list to hold all of our variadic parameters
		va_list valist;
		va_start(valist, priorityLevel);

		// Execute sprintf to format the msg with the variadic parameters
		size_t bufferSize = (size_t)C_errMaxLength + 1;
		std::string buffer{};
		buffer.resize(bufferSize);
		if (vsprintf_s(&buffer[0], bufferSize, msg.c_str(), valist) < 0)
		{
			// Failed to sprintf.
			va_end(valist);
			Error::RaiseErrFormatted("Error::RaiseErrFormatted", 
				"Failed to convert variadic arguments to string! Caller/FormatMsg: %s/%s", ERROR_ERR, caller.c_str(), msg.c_str());
			return;
		}

		// Cleanup va_list
		va_end(valist);

		// Raise the error with the formatted string
		Singleton<Error>::Instance()->RaiseErr_Internal(caller, buffer, priorityLevel);
	}

	void Error::Update(f32 dt)
	{
		for (auto errIter = logVec.begin(); errIter != logVec.end(); )
		{
			errIter->Update(dt);
			if (errIter->IsExpired())
				errIter = logVec.erase(errIter);
			else
				++errIter;
		}
	}

	void Error::RenderAll()
	{
		if (!C_errDoRender)
			return;

		f32 renderY = sCast<f32>(C_windowHeight) * - 0.5f + 10.0f;
		for (auto errIter = logVec.rbegin(); errIter != logVec.rend(); ++errIter)
		{
			errIter->Render(renderY);
			renderY += C_errFontSize * C_assetFontSize;
		}
	}

	namespace INTERNAL {

		Color Error_Log::GetColor(ERROR_PRIORITYLEVEL priority)
		{
			switch (priority)
			{
			case ERROR_ERR:
				return Color(1.0f, 0.0f, 0.0f, 1.0f);
			case ERROR_WARN:
				return Color(1.0f, 1.0f, 0.0f, 1.0f);
			case ERROR_INFO:
				return Color();
			default:
				Error::RaiseErr("Error_Log::GetColor", "Invalid error priority!", ERROR_ERR);
				return Color();
			}
		}

		Error_Log::Error_Log(const std::string& msg, ERROR_PRIORITYLEVEL priority, s8 fontId)
			: text(new Text(msg, C_errFontSize, MakeVec2(C_windowWidth * -0.5f, 0.0f), GetColor(priority), false, fontId, TEXT_ALIGN_LEFT, TEXT_ALIGN_BOTTOM))
			, priority(priority)
			, elapsedTime(0.0f)
		{
		}
		Error_Log::Error_Log(const Error_Log& other)
			: text(other.text)
			, priority(other.priority)
			, elapsedTime(other.elapsedTime)
		{
		}
		Error_Log::Error_Log(Error_Log&& other) noexcept
			: text(other.text)
			, priority(other.priority)
			, elapsedTime(other.elapsedTime)
		{
		}
		Error_Log& Error_Log::operator=(const Error_Log& other)
		{
			text = other.text;
			priority = other.priority;
			elapsedTime = other.elapsedTime;
			return *this;
		}

		bool Error_Log::IsExpired() const
		{
			return elapsedTime >= C_errEntryLifetime;
		}

		void Error_Log::Update(f32 dt)
		{
			elapsedTime += dt;
			if (elapsedTime < C_errEntryLifetime)
				text->SetAlpha(min((C_errEntryLifetime - elapsedTime) / C_errFadeLength, 1.0f));
		}

		void Error_Log::Render(f32 posY)
		{
			text->SetPosition(MakeVec2(text->GetPosition().x, posY));
			text->Render();
		}
	}
}
