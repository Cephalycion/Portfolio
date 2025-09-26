/******************************************************************************/
/*!
\file   G_EnemyPatroller.h
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Marc Alviz Evangelista (100%)
\par    email: marcalviz.e\@digipen.edu
\par    DigiPen login: marcalviz.e

\brief
  This is the interface file for all member functions of classes called
  EnemyPatroller and PatrollerBuilder. Along with utility functions for EnemyPatroller.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#pragma once
#include "pch.h"
#include "G_EnemyBase.h"
#include "E_BaseStateMachine.h"
#include "E_LineRenderer.h"

namespace Game {

	enum PATROLLERSTATES {
		P_IDLE = 0,
		P_PATROL,
		P_AGGRO,
		P_EXPLODING,
		P_NUMSTATES
	};

	enum PATROLLER_SPRITE {
		PATROLLER_IDLE_LEFT,
		PATROLLER_IDLE_RIGHT,

		PATROLLER_MOVE_LEFT,
		PATROLLER_MOVE_RIGHT,

		PATROLLER_AIR_LEFT,
		PATROLLER_AIR_RIGHT,

		PATROLLER_EXPLODE_LEFT,
		PATROLLER_EXPLODE_RIGHT

	};

	class EnemyPatroller : public EnemyBase {
	private:
		BaseStateMachine fsm;
		BaseStateMachine animFsm;
		sPtr<LineRenderer> ln;
		f32 explosionRadius;
		f32 idlePatrolSwitchTimer;
		std::map<PATROLLER_SPRITE, sPtr<Engine::Sprite>> patrollerAnims;
		bool initAnim;
		bool isMoving;
		bool isExploding;
	public:
		EnemyPatroller(const Transform& transform, const sPtr<Sprite>& spritePtr);
		~EnemyPatroller();

		void Init();
		virtual void Update() override;
		virtual void Render() override;
		void DoAction(PATROLLERSTATES state);
		f32 GetExRadius() const;

		void ChangeAnimation(PATROLLER_SPRITE newSprite);
		void InitAnimationTree();

		bool GetIsMoving() const;
		void SetIsMoving(bool b);

		bool GetIsExploding() const;
		void SetIsExploding(bool b);
		
	};


	class PatrollerBuilder
	{
	private:
		PatrollerBuilder() = delete;

	public:
		static sPtr<EnemyPatroller> CreatePatroller(const Transform& transform = Transform(0.0f, 0.0f, 0.0f, 100.0f, 100.0f),
			const sPtr<Sprite> spritePtr = SpriteBuilder::CreateSpriteSquare());
	};


}