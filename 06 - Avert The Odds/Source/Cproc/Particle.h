/*!
@copyright	DIGIPEN INSTITUTE OF TECHNOLOGY SINGAPORE
@file 		Particle.h
@author 	Lau Jia Win (jiawin.lau)
@course     CSD1401 Software Engineering Project 1
@section    a
@tutorial   Final Project
@date 		06/11/2023
@brief 		This file contains the function declarations, enum and struct for
			the Particle system in the game.
*//*______________________________________________________________________*/
#pragma once
#include <stdbool.h>		// bool
#include "CProcessing.h"	// CP_Vector, CP: Render, Color
#include "Sprite.h"

#define PARTICLEDEF_MAX_NUMBER 50					// Max number of particles allocated

#define PARTICLEDEF_LIFETIME_DECR_RATE 500.F		// The rate the particle dies
#define PARTICLEDEF_LIFETIME 255.F					// Time the particle is alive for
#define PARTICLEDEF_ANIM_PLAYTIME 1.F				// Animation playtime of the particle
//#define PARTICLEDEF_ALPHA_MAX 					// REPLACED BY PARTICLEDEF_LIFETIME
#define PARTICLEDEF_ALPHA 255						// The alpha value used for all particles
#define PARTICLEDEF_SIZE_SCALE 50.F					// Scale for simple explosion

// Tweening
#define PARTICLEDEF_FONTSIZE_MIN 30.F				// Minimum font size
#define PARTICLEDEF_FONTSIZE_MAX 40.F				// Max font size it will popup too
#define PARTICLEDEF_TWEEN_MIN 0.F					// The minimum offset for popup
#define PARTICLEDEF_TWEEN_MAX 20.F					// The max offset for popup
#define PARTICLEDEF_ONEDIVBYLIFETIME (1.F / PARTICLEDEF_LIFETIME)	// Used for calculation


typedef enum PARTICLE_TYPE {
	PARTICLE_TYPE_UNASSIGNED = -1,			
	PARTICLE_TYPE_EXPLOSION = 0,
	PARTICLE_TYPE_ENEMY_HIT,
	PARTICLE_TYPE_PLAYER_HIT,

	PARTICLE_TYPE_MENU_STAR,

	PARTICLE_TYPE_TOTAL_PARTICLES_TYPES
} PARTICLE_TYPE;


// ==============================================================
// Particle Object
// 
// Special particle with number value [Damage number]
typedef struct ParticleEnemyHit {
	int damageNumber;
} ParticleEnemyHit;

typedef struct ParticlePlayerHit {
	int damageNumber;
} ParticlePlayerHit;

typedef struct ParticleMenuStar {
	CP_Vector* sharedPosition;
	bool* isWithinBorder;
} ParticleMenuStar;


typedef struct ParticleObject
{
	bool isAlive;					// Flag used in render/update/spawn
	PARTICLE_TYPE type;				// Type of particle used in render
	float lifeTime;					// Determine the time particle is active for
	float posX, posY;				// Position of particle

	// Sprite animation use
	float animTime;
	int frameIndex;

	union
	{
		ParticleEnemyHit enemyHitParticle;		// Special particle with number value [Damage number]
		ParticlePlayerHit playerHitParticle;	// Special particle with number value [Damage number]
		ParticleMenuStar menuStarParticle;		// Special particle with direction and doesn't use lifetime
	};
} ParticleObject;
//
// ==============================================================


// ==============================================================
// Call these 5 functions in the game scene file respectively
//
void ParticleInitSimple(void);
void ParticleInitGame(CP_Vector* cameraOffset);	// Loads particle images, assigns cameraOffset pointer for render
void ParticleUpdate(void);						// Updates lifetime for particles
void ParticleRender(void);						// Render particles
void ParticleExit(void);						// Kills all particles (set inactive)
void ParticleQuitGame(void);						// ONLY WHEN EXIT GAME: Unloads particle images
//
// ==============================================================


// ==============================================================
// Spawn a particle object
// optionalvalue - special particle that needs value [Damage number]
void ParticleSpawn(PARTICLE_TYPE _type, float _posX, float _posY, float lifetime, int optionalValue);
// Only used by MENU_STAR
void ParticleSpawnSharePos(PARTICLE_TYPE _type, CP_Vector* _sharedPos, float lifetime, bool* optionalValue);
//
// ==============================================================

// Popup calculation
static float ParticleEaseOutQuad(float start, float end, float value);
