/******************************************************************************/
/*!
\file   G_EnemyTurret.h
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Marc Alviz Evangelista (90%)
\par    email: marcalviz.e\@digipen.edu
\par    DigiPen login: marcalviz.e

\author Ryan Chan Kuan Fu (10%)
\par    email: c.kuanfuryan\@digipen.edu
\par    DigiPen login: c.kuanfuryan

\brief
  This is the interface file for all member functions of the classes EnemyTurret
  and TurretBuilder. 

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#pragma once
#include "pch.h"
#include "E_TurretStateMachine.h"
#include "G_EnemyBase.h"
#include "G_Projectile.h"
#include "E_LineRenderer.h"
#include <map>

namespace Game {
	using namespace Engine;

	enum TURRETSTATES {
		T_IDLE = 0, 
		T_AGGRO,
		T_NUMSTATES
	};

	class EnemyTurret : public EnemyBase {
		BaseStateMachine fsm;
		sPtr<LineRenderer> ln;
	public:
		EnemyTurret();
		EnemyTurret(const Transform& transform, const sPtr<Sprite>& spritePtr);
		~EnemyTurret();
		void Init();
		virtual void Update() override;
		virtual void Render() override;
		void DoAction(TURRETSTATES state);
	};

	class TurretBuilder
	{
	private:
		TurretBuilder() = delete;

	public:
		static sPtr<EnemyTurret> CreateTurret(const Transform& transform = Transform(0.0f, 0.0f, 0.0f, 100.0f, 100.0f),
			const sPtr<Sprite> spritePtr = SpriteBuilder::CreateSpriteSquare());
	};

}