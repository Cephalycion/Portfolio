/******************************************************************************/
/*!
\file   E_AEEngineInterface.cpp
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Kendrick Sim Hean Guan (100%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\brief
  This is the implementation file to make certain calls to AEEngine more convenient.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#include "pch.h"
#include "E_AEEngineInterface.h"
#include "E_Constants.h"

using namespace Engine;

#pragma region GRAPHICS

namespace Engine { namespace Graphics {

	Color MakeColor(u8 r, u8 g, u8 b, u8 a)
	{
		return Color(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
	}
}}

void AEGfxSetBackgroundColor(const Engine::Graphics::Color& c)
{
	AEGfxSetBackgroundColor(c.r, c.g, c.b);
}
void AEGfxSetBlendColor(const Engine::Graphics::Color& c)
{
	AEGfxSetBlendColor(c.r, c.g, c.b, c.a);
}
void AEGfxSetColorToMultiply(const Engine::Graphics::Color& c)
{
	AEGfxSetColorToMultiply(c.r, c.g, c.b, c.a);
}
void AEGfxSetColorToAdd(const Engine::Graphics::Color& c)
{
	AEGfxSetColorToAdd(c.r, c.g, c.b, c.a);
}

void AEGfxPrint(s8 fontId, const std::string& text, const Vec2& position, f32 scale, const Engine::Graphics::Color& color)
{
	AEGfxPrint(fontId, text.c_str(), position.x, position.y, scale, color.r, color.g, color.b, color.a);
}

#pragma endregion

#pragma region MATH

Vec2 Vec2FromAngle(f32 angle)
{
	f32 piAngle = AEDegToRad(angle);
	return { sinf(piAngle), cosf(piAngle) };
}

Vec2 Vec2Abs(const Vec2& vec)
{
	return { std::abs(vec.x), std::abs(vec.y) };
}
Vec2& Vec2Neg(Vec2* vec)
{
	vec->x *= -1.0f;
	vec->y *= -1.0f;
	return *vec;
}

Vec2& Vec2Abs(Vec2* vec)
{
	vec->x = std::abs(vec->x);
	vec->y = std::abs(vec->y);
	return *vec;
}

f32 Vec2Length(const Vec2& vec)
{
	return std::sqrt(vec.x * vec.x + vec.y * vec.y);
}
f32 Vec2LengthSquared(const Vec2& vec)
{
	return vec.x * vec.x + vec.y * vec.y;
}

f32 Vec2Distance(const Vec2& thisVec, const Vec2& other)
{
	Vec2 distVec = other - thisVec;
	return Vec2Length(distVec);
}
f32 Vec2DistanceSquared(const Vec2& thisVec, const Vec2& other)
{
	Vec2 distVec = other - thisVec;
	return Vec2LengthSquared(distVec);
}

f32 Vec2Dot(const Vec2& thisVec, const Vec2& other)
{
	return thisVec.x * other.x + thisVec.y * other.y;
}
f32 Vec2Cross(const Vec2& thisVec, const Vec2& other)
{
	return thisVec.x * other.y - thisVec.y * other.x;
}
f32 Vec2CrossMagnitude(const Vec2& thisVec, const Vec2& other)
{
	return std::sqrt(Vec2Cross(thisVec, other));
}

Vec2 Vec2Normalize(const Vec2& vec)
{
	return vec / Vec2Length(vec);
}
Vec2& Vec2Normalize(Vec2* vec)
{
	f32 length = Vec2Length(*vec);
	vec->x /= length;
	vec->y /= length;
	return *vec;
}

Vec2 Vec2Project(const Vec2& thisVec, const Vec2& projectOnVec)
{
	Vec2 result{};
	AEVec2Project(&result, cCast<Vec2*>(&thisVec), cCast<Vec2*>(&projectOnVec));
	return result;
}
Vec2 Vec2ProjectPerp(const Vec2& thisVec, const Vec2& projectOnVec)
{
	Vec2 result{};
	AEVec2ProjectPerp(&result, cCast<Vec2*>(&thisVec), cCast<Vec2*>(&projectOnVec));
	return result;
}

f32 Vec2ToAngle(const Vec2& thisVec)
{
	return AERadToDeg(std::atan2f(thisVec.x, thisVec.y));
}

Vec2 Vec2Rot90(const Vec2& thisVec)
{
	return MakeVec2(-thisVec.y, thisVec.x);
}
Vec2& Vec2Rot90(Vec2* thisVec)
{
	f32 temp = thisVec->x;
	thisVec->x = -thisVec->y;
	thisVec->y = temp;
	return *thisVec;
}

Vec2 operator-(const Vec2& thisVec)
{
	return MakeVec2(-thisVec.x, -thisVec.y);
}

Vec2 operator+(const Vec2& thisVec, const Vec2& other)
{
	return MakeVec2(thisVec.x + other.x, thisVec.y + other.y);
}
Vec2& operator+=(Vec2& thisVec, const Vec2& other)
{
	thisVec.x += other.x;
	thisVec.y += other.y;
	return thisVec;
}

Vec2 operator-(const Vec2& thisVec, const Vec2& other)
{
	return MakeVec2(thisVec.x - other.x, thisVec.y - other.y);
}
Vec2& operator-=(Vec2& thisVec, const Vec2& other)
{
	thisVec.x -= other.x;
	thisVec.y -= other.y;
	return thisVec;
}

Vec2 operator*(const Vec2& thisVec, f32 scale)
{
	return { thisVec.x * scale, thisVec.y * scale };
}
Vec2 operator*(f32 scale, const Vec2& thisVec)
{
	return thisVec * scale;
}
Vec2& operator*=(Vec2& thisVec, f32 scale)
{
	thisVec.x *= scale;
	thisVec.y *= scale;
	return thisVec;
}

Vec2 operator/(const Vec2& thisVec, f32 divisor)
{
	return { thisVec.x / divisor, thisVec.y / divisor };
}
Vec2& operator/=(Vec2& thisVec, f32 divisor)
{
	thisVec.x /= divisor;
	thisVec.y /= divisor;
	return thisVec;
}

Vec2 operator/(const Vec2& thisVec, const Vec2& divisor)
{
	return { thisVec.x / divisor.x, thisVec.y / divisor.y };
}
Vec2& operator/=(Vec2& thisVec, const Vec2& divisor)
{
	thisVec.x /= divisor.x;
	thisVec.y /= divisor.y;
	return thisVec;
}

std::ostream& operator<<(std::ostream& os, const Vec2& thisVec)
{
	os << '(' << thisVec.x << ',' << thisVec.y << ')';
	return os;
}

bool Vec2IsNearOriented(const Vec2& thisVec, const Vec2& otherVec)
{
	static const f32 maxSinVal = Util::PowSqr(sinf(AEDegToRad(1.0f)));

	f32 thisLengthSqr = Vec2LengthSquared(thisVec);
	f32 otherLengthSqr = Vec2LengthSquared(otherVec);

	f32 sinVal = Vec2Dot(thisVec, otherVec) / thisLengthSqr / otherLengthSqr;

	return std::abs(sinVal) <= maxSinVal;
}

f32 Vec2AngBtwNormalized(const Vec2& thisVec, const Vec2& otherVec)
{
	return AERadToDeg(std::asinf(Vec2Dot(thisVec, otherVec)));
}


Mtx33 operator*(const Mtx33& thisMtx, const Mtx33& other)
{
	Mtx33 result{};
	AEMtx33Concat(&result, cCast<Mtx33*>(&thisMtx), cCast<Mtx33*>(&other));
	return result;
}

Vec2 operator*(const Mtx33& thisMtx, const Vec2& other)
{
	return MakeVec2(
		thisMtx.m[0][0] * other.x + thisMtx.m[0][1] * other.y + thisMtx.m[0][2],
		thisMtx.m[1][0] * other.x + thisMtx.m[1][1] * other.y + thisMtx.m[1][2]
	);
}

#pragma endregion
