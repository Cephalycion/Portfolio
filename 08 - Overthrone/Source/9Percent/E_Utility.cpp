/******************************************************************************/
/*!
\file   E_Utility.cpp
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Kendrick Sim Hean Guan (100%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\brief
  This is the implementation file for utility functions.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#include "pch.h"
#include "E_Utility.h"

namespace Engine { namespace Util {

	template <>
	bool StrToVar<bool>(const std::string& s)
	{
		return sCast<bool>(std::stoi(s));
	}
	template <>
	int StrToVar<int>(const std::string& s)
	{
		return std::stoi(s);
	}
	template <>
	std::string StrToVar<std::string>(const std::string& s)
	{
		return s;
	}

}}
