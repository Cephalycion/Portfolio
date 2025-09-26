/******************************************************************************/
/*!
\file   E_AEEngineInterface.h
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Kendrick Sim Hean Guan (100%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\brief
  This is an interface file to make certain calls to AEEngine more convenient.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#pragma once
#include <AEEngine.h>
#include <cmath>
#include <string>
#include <iostream>

#pragma region MACROS

/* Alias type names so that we don't need to type AE */
using Vec2 = AEVec2;
using Mtx33 = AEMtx33;

#pragma endregion

#pragma region GRAPHICS

namespace Engine { namespace Graphics {

	// Represent a color value, since AEEngine doesn't seem to have one...
	struct Color {
		f32 r, g, b, a;

		Color(f32 r = 1.0f, f32 g = 1.0f, f32 b = 1.0f, f32 a = 1.0f)
			: r(r), g(g), b(b), a(a) {}
	};

	// Make a Color object from 8 bit color values
	Color MakeColor(u8 r, u8 g, u8 b, u8 a = 255);
}}


/* Declare outside the namespace so that calls to the original AEEngine functions
*  do not require prefixing a :: to differentiate them from these functions.
*/

void AEGfxSetBackgroundColor(const Engine::Graphics::Color& c);
void AEGfxSetBlendColor(const Engine::Graphics::Color& c);
void AEGfxSetColorToMultiply(const Engine::Graphics::Color& c);
void AEGfxSetColorToAdd(const Engine::Graphics::Color& c);

void AEGfxPrint(s8 fontId, const std::string& text, const Vec2& position, f32 scale, const Engine::Graphics::Color& color);

#pragma endregion

#pragma region MATH

constexpr Vec2 MakeVec2(f32 x, f32 y) { return { x, y }; }
constexpr Vec2 Vec2Zero() { return { 0.0f, 0.0f }; }
Vec2 Vec2FromAngle(f32 angle);

Vec2& Vec2Neg(Vec2* vec);
Vec2 Vec2Abs(const Vec2& vec);
Vec2& Vec2Abs(Vec2* vec);
f32 Vec2Length(const Vec2& vec);
f32 Vec2LengthSquared(const Vec2& vec);
f32 Vec2Distance(const Vec2& thisVec, const Vec2& other);
f32 Vec2DistanceSquared(const Vec2& thisVec, const Vec2& other);
f32 Vec2Dot(const Vec2& thisVec, const Vec2& other);
f32 Vec2Cross(const Vec2& thisVec, const Vec2& other);
f32 Vec2CrossMagnitude(const Vec2& thisVec, const Vec2& other);
Vec2 Vec2Normalize(const Vec2& vec);
Vec2& Vec2Normalize(Vec2* vec);
Vec2 Vec2Project(const Vec2& thisVec, const Vec2& projectOnVec);
Vec2 Vec2ProjectPerp(const Vec2& thisVec, const Vec2& projectOnVec);
f32 Vec2ToAngle(const Vec2& thisVec); // atan2() method
Vec2 Vec2Rot90(const Vec2& thisVec); // Rotate 90 degrees
Vec2& Vec2Rot90(Vec2* thisVec);

Vec2 operator-(const Vec2& thisVec);
Vec2 operator+(const Vec2& thisVec, const Vec2& other);
Vec2& operator+=(Vec2& thisVec, const Vec2& other);
Vec2 operator-(const Vec2& thisVec, const Vec2& other);
Vec2& operator-=(Vec2& thisVec, const Vec2& other);
Vec2 operator*(const Vec2& thisVec, f32 scale);
Vec2 operator*(f32 scale, const Vec2& thisVec);
Vec2& operator*=(Vec2& thisVec, f32 scale);
Vec2 operator/(const Vec2& thisVec, f32 divisor);
Vec2& operator/=(Vec2& thisVec, f32 divisor);
Vec2 operator/(const Vec2& thisVec, const Vec2& divisor);
Vec2& operator/=(Vec2& thisVec, const Vec2& divisor);

std::ostream& operator<<(std::ostream& os, const Vec2& thisVec);

bool Vec2IsNearOriented(const Vec2& thisVec, const Vec2& otherVec); // True if rotation difference is less than 1 degree
f32 Vec2AngBtwNormalized(const Vec2& thisVec, const Vec2& otherVec); // Get angle between 2 NORMALIZED vectors


Mtx33 operator*(const Mtx33& thisMtx, const Mtx33& other);
Vec2 operator*(const Mtx33& thisMtx, const Vec2& other);

#pragma endregion
