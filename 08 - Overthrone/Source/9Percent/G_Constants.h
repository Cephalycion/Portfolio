/******************************************************************************/
/*!
\file   G_Constants.h
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Ryan Chan Kuan Fu (35%)
\par    email: c.kuanfuryan\@digipen.edu
\par    DigiPen login: c.kuanfuryan

\author Kendrick Sim Hean Guan (35%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\author Marc Alviz Evangelista (21%)
\par    email: marcalviz.e\@digipen.edu
\par    DigiPen login: marcalviz.e

\author Matthew Chan Shao Jie (9%)
\par    email: m.chan\@digipen.edu
\par    DigiPen login: m.chan

\brief
  Bunch of different const values to be used with game-related classes 
  and functions.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#pragma once
#include "pch.h"

namespace Game {

	// =========================
	// g_map
	// =========================
	const s32 C_levelLast = 3; // Final level id

	// =========================
	// g_scene_splash
	// =========================
	const f32 C_logoLength = 2.8f; // Total time that the logo will stay on screen.
	const f32 C_logoFadeLength = 0.4f; // The time it takes for the logo to fade in / out.
	const f32 C_logo9PercentTimeTillGlitch = 1.2f; // Time it takes for the 9Percent logo to first glitch
	const f32 C_logo9PercentGlitchLength = 0.06f; // Length of time that the 9Percent logo stays glitched
	const f32 C_logo9PercentReglitchTime = 0.2f; // Time after each glitch before 9Percent logo glitches again
	const s32 C_logo9PercentTimesGlitch = 2; // Number of times to glitch 9Percent logo

	// Blocks
	const f32 C_blockLength = 50.0f;
	const f32 C_blockLengthHalf = C_blockLength * 0.5f;
	const f32 C_airFluidDensity = 1.293f;
	const f32 C_waterFluidDensity = 12.93f;

	// Grapple Hook
	const f32 C_grappleTipSize = 25.0f;
	const f32 C_springStiffness = 5.0f;
	const f32 C_grapplePullSpeed = 2000.0f;
	const f32 C_grappleThrowSpeed = 1500.0f;
	const f32 C_grappleMinExtension = 150.0f;

	// Arc Predictor
	const s32 C_arcPredictorLength = 30;
	const f32 C_arcUnitSize = 10.0f;
	const f32 C_arcPredictorInterval = 0.05f;
	const f32 C_arcPredictorSpeed = 0.3f;

	// Predictor Projectile
	const s32 C_predictorProjectileLength = 50;
	const f32 C_predictorProjectileInterval = 0.3f;
	const f32 C_predictorProjectileIterationSpeed = 10.0f;
	const f32 C_projectileSpeed = 700.0f;
	const f32 C_predictorProjectileSineWidth = 80.0f;
	const f32 C_predictorProjectileSineHeight = 80.0f;
	const f32 C_predictorProjectileSpiralR = 30.0f;
	const f32 C_predictorProjectileSpeed = 5.0f;
	const f32 C_predictorProjectileSquareScale = 120.0f;

	// Laser
	const f32 C_laserWidth = 5.0f;

	// Grab Display
	const f32 C_grabSize = 25.0f;
	const f32 C_grabTintMult = 1.5f; // Amount to multiply the color value when a block is highlighted
	const f32 C_dynamicTintMult = 0.7f; // Amount to multiply the color value when a block is highlighted

	// Dash Indicator
	const f32 C_dashIndicatorDistance = 100.0f;

	// =========================
	// Player
	// =========================
	const Vec2 C_playerCollisionScale = MakeVec2(C_blockLengthHalf, C_blockLength * 0.95f);
	const Vec2 C_playerRenderScale = MakeVec2(C_blockLength, C_blockLength * 0.95f);
	const f32 C_maxPlayerHealth = 100.0f;
	const f32 C_invincibilityTimer = 1.0f;
	const f32 C_healthLength = 60.0f;
	const Vec2 C_healthPosStart = MakeVec2(-C_healthLength * 0.5f, 45.0f);
	const f32 C_healthWidth = 12.0f;
	const f32 C_healthOutlineThickness = 4.0f;
	const f32 C_healthFlashSpeed = 15.0f;

	// =========================
	// g_enemyPatroller
	// =========================
	const f32 C_enemyPatrolExplosionTime = 0.5f; // Length of the explosion
	
	// =========================
	// g_blockTexture
	// =========================
	extern const char* C_bTexFilepath;
	extern const char* C_bTexFiletype;
	const Vec2 C_bTexWaterScrollSpeed = MakeVec2(1.0f, 0.0f);
	const Vec2 C_bTexWaterUVLength = MakeVec2(0.5f, 1.0f);
	const Vec2 C_bTexLavaScrollSpeed = MakeVec2(0.5f, 0.0f);
	const Vec2 C_bTexLavaUVLength = MakeVec2(0.5f, 1.0f);

	// =========================
	// g_sign
	// =========================
	const f32 C_signDist = 150.0f; // Distance before activating sign text

	// =========================
	// g_enemyBoss
	// =========================
	const f32 C_bossDeathTime = 3.5f; // How long to wait after the boss dies before deleting it
	const f32 C_bossDeathShakeInterval = 0.05f; // The interval between each shake
	const f32 C_bossDeathLerpTime = 1.5f; // How long does the camera take to lerp to the boss position
	const f32 C_bossDeathAfterTime = 2.0f; // How long to linger the camera on the boss after it's finished dying

	// =========================
	// e_particleSystem
	// =========================
	// BlockHit
	const f32 C_psBlockHit_lifespan = 1.0f;
	const f32 C_psBlockHit_fadeThreshold = 0.25f; // Start fading once lifespan falls below this threshold
	const f32 C_psBlockHit_minLifetimeFactor = 0.4f; // Minimum multiplier for lifetime randomness
	const f32 C_psBlockHit_spreadFactor = 1.0f; // Scale of angle spread randomness (higher value -> more packed together)
	const f32 C_psBlockHit_spreadMaxAngle = 55.0f; // Maximum angle adjustment from randomness.
	const f32 C_psBlockHit_speedFactor = 0.2f; // Scale of speed multiplier randomness (>0.0f no random, to 0.5f fully random)
	const f32 C_psBlockHit_reqVelSqrPerParticle = 40000.0f; // A particle is spawned per this amount in the relative velocity squared
	// Smoke
	const f32 C_psSmoke_lifespan = 1.5f;
	const f32 C_psSmoke_fadeThreshold = 0.25f; // Start fading once lifespan falls below this threshold
	const f32 C_psSmoke_minLifetimeFactor = 0.5f; // Minimum multiplier for lifetime randomness
	const f32 C_psSmoke_spreadMaxAngle = 45.0f; // Maximum angle adjustment to velocity
	const f32 C_psSmoke_speedMin = 20.0f; // Minimum speed
	const f32 C_psSmoke_speedMax = 60.0f; // Maximum speed
	const s32 C_psSmoke_numParticles = 10; // Number of particles spawned
	// Death Explosion
	const f32 C_psEnemyHit_lifespan = 1.0f;
	const f32 C_psEnemyHit_fadeThreshold = 0.1f; // Start fading once lifespan falls below this threshold
	const f32 C_psEnemyHit_minLifetimeFactor = 0.4f; // Minimum multiplier for lifetime randomness
	const f32 C_psEnemyHit_spreadMaxAngle = 180.0f; // Maximum angle adjustment to velocity
	const f32 C_psEnemyHit_speedMin = 300.0f; // Minimum speed
	const f32 C_psEnemyHit_speedMax = 600.0f; // Maximum speed
	const s32 C_psEnemyHit_numParticlesHit = 8; // Number of particles spawned on hit
	const s32 C_psEnemyHit_numParticlesDeath = 50; // Number of particles spawned on death
	// Death Shine
	const f32 C_psDShine_uLength = 64.0f / 240.0f; // Length of u in uv texture coordinates
	const f32 C_psDShine_transitionLength = 80.0f / 240.0f; // Amount of u to change by to move the sprite texture to the next state
	const f32 C_psDShine_numRays = 16; // Number of rays
	const f32 C_psDShine_thickness = 15.0f; // Thickness of each ray
	const f32 C_psDShine_lifespan = C_bossDeathTime + 1.0f;
	// Boss Die Explosion
	const f32 C_psBossExplo_lifespan = 1.0f;
	const f32 C_psBossExplo_spawnRate = 0.03f; // The interval between the spawning of each particle
	const f32 C_psBossExplo_spreadMaxAngle = 70.0f; // Maximum angle adjustment to velocity
	const f32 C_psBossExplo_speedMin = 200.0f; // Minimum speed
	const f32 C_psBossExplo_speedMax = 400.0f; // Maximum speed

	// =========================
	// Parallax Background
	// =========================
	const Vec2 C_bgHillsSize = MakeVec2(1800.0f, 900.0f);
	const Vec2 C_bgWallSize = MakeVec2(1200.0f, 600.0f);

	// =========================
	// Animation Speeds
	// =========================
	const f32 C_playerWalkSpeed = 0.75f;
	const f32 C_bossAttackAnimSpeed = 0.75f;
	const f32 C_EnemyGreenDrainRate = 0.5f;
	const f32 C_patrollerFuseTimer = 1.5f;


	/* HACKS */
	extern bool C_hackFreecam;
	extern bool C_hackNoclip;
	extern bool C_hackInvincible;
	extern bool C_hackDemo;
}
