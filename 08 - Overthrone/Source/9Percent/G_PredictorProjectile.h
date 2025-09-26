/******************************************************************************/
/*!
\file   G_PredictorProjectile.h
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Ryan Chan Kuan Fu (100%)
\par    email: c.kuanfuryan\@digipen.edu
\par    DigiPen login: c.kuanfuryan

\brief
  This bad boy can render any differentiable equation. Sine wave? Spiral? No
  problem. Just make sure you know your math! Used by EnemyBoss.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#pragma once
#include "pch.h"
#include "E_Object.h"
#include "G_Constants.h"
#include "G_ArcPredictor.h"
#include "G_Projectile.h"

// author - Ryan
// PredictorProjectile is an offshoot of ArcPredictor for usage with Boss attacks.
// It will instantiate a series of AP_UNITs and fire a projectile after a set delay.
// Exit() should be called after the projectile has expired.
namespace Game
{
	enum PREDICTOR_TYPE
	{
		PT_SPIRAL,
		PT_SINE,
		PT_SQUARED,

		PT_END
	};
	class PredictorProjectile
	{
	protected:
		std::vector<sPtr<AP_UNIT>> units;
		sPtr<Projectile> projectile;
		bool active;
		bool shoot;
		s32 currentID;
		f32 pTime;
		Vec2 startPos;
		PREDICTOR_TYPE pType;
		f32 pRot;
		bool positive;

		Vec2 PositionAtTimestep(f32 timestep);
		f32 AngleAtTimestep(f32 timestep);
		void ApplyRotationAboutStartPoint(sPtr<Engine::Object> obj);

	public:
		PredictorProjectile();
		~PredictorProjectile();

		void Init(PREDICTOR_TYPE type, Vec2 pos, f32 rot, bool isPositive);
		void Update();
		void Render();
		void Exit();
		bool isActive();
		void setActive(bool b);
	};
}