/******************************************************************************/
/*!
\file   E_Utility.h
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Kendrick Sim Hean Guan (100%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\brief
  This is an interface file for utility functions.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#pragma once
#include "E_Constants.h"

namespace Engine { namespace Util {

	constexpr s32 Round(f32 x) { return sCast<s32>(x + 0.5f); }
	constexpr s32 RoundUp(f32 x) { return sCast<s32>(x - C_epsilon) + (x >= 0.0f ? 1 : 0); }
	constexpr s32 RoundDown(f32 x) { return sCast<s32>(x >= 0.0f ? x : x - 1.0f); }

	constexpr f32 PowSqr(f32 x) { return x * x; }
	constexpr f32 Clamp(f32 x, f32 min, f32 max)
	{
		if (x < min)
			return min;
		else if (x > max)
			return max;
		else
			return x;
	}
	constexpr f32 Wrap(f32 x, f32 min, f32 max)
	{
		if (x < min)
			x += (sCast<s32>((min - x) / (max - min)) + 1) * (max - min);
		else if (x > max)
			x -= (sCast<s32>((x - max) / (max - min)) + 1) * (max - min);
		return x;
	}

	/* Lerp functions */
	// Linear lerp based on ratio from 0 to 1
	constexpr f32 Lerp(f32 from, f32 to, f32 ratio)
	{
		return from * (1.0f - ratio) + to * ratio;
	}
	// Produce a ratio based on an ease in curve from 0 to 1
	constexpr f32 LerpRatioEaseIn(f32 ratio)
	{
		return ratio * ratio;
	}
	// Produce a ratio based on an ease out curve from 0 to 1
	constexpr f32 LerpRatioEaseOut(f32 ratio)
	{
		ratio = 1.0f - ratio;
		return 1.0f - ratio * ratio;
	}
	// Produce a ratio based on an ease in out curve from 0 to 1
	constexpr f32 LerpRatioEaseInOut(f32 ratio)
	{
		return Lerp(LerpRatioEaseIn(ratio), LerpRatioEaseOut(ratio), ratio);
	}

	// Convert string to a variable type
	// Implemented in .cpp for the various variable types required.
	template <typename T>
	T StrToVar(const std::string& s);

}}
