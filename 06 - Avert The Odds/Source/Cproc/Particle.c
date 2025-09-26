/*!
@copyright	DIGIPEN INSTITUTE OF TECHNOLOGY SINGAPORE
@file 		Particle.c
@author 	Lau Jia Win (jiawin.lau)
@course     CSD1401 Software Engineering Project 1
@section    a
@tutorial   Final Project
@date 		06/11/2023
@brief 		This file contains the function definitions for
			the Particle system in the game.
*//*______________________________________________________________________*/
#include "Particle.h"
#include <stdio.h>			// debug: printf

// ===== Private variables =====
ParticleObject particleList[PARTICLEDEF_MAX_NUMBER];		// List of all particleObjects
//CP_Image particleImageList[PARTICLEDEF_MAX_NUMBER];		// List of images for particles - NOT USED
SpriteParticle particleSpriteList[PARTICLEDEF_MAX_NUMBER];	// List of particle sprites

CP_Vector* cameraOffsetPtr = NULL;							// Pointer to camera offset for particle rendering in world
bool isParticleLoaded = false;								// Prevents loading again if loaded already
CP_Font particleNumberFont = NULL;							// font for damage number
CP_Color damageValueColor;									// color for damage number (enemy)
CP_Color playerHitValueColor;								// color for damage number (player)
// =============================

#pragma region Particle Standard Functions

void ParticleInitSimple()
{
	for (int i = 0; i < PARTICLEDEF_MAX_NUMBER; i++)
	{
		particleList[i].type = PARTICLE_TYPE_UNASSIGNED;
		particleList[i].isAlive = false;
		particleList[i].posX = particleList[i].posY = 0;
		particleList[i].animTime = 0.0f;
		particleList[i].frameIndex = 0;
	}

	if (!isParticleLoaded)
	{
		// Load images - NOT USED
		//particleImageList[PARTICLE_TYPE_ENEMY_HIT] = CP_Image_Load("./Assets/upgradesBarActive.png");

		// Load font for damage numbers
		particleNumberFont = CP_Font_Load("./Assets/font/8514oem.ttf");		// Erm surely wont have problem loading fonts mulitple times
		/*if (particleNumberFont == NULL)
			printf("Particle: Font NOT loaded!\n");*/

		CP_Font_Set(particleNumberFont);
		CP_Settings_TextSize(PARTICLEDEF_FONTSIZE_MIN);
		damageValueColor = CP_Color_Create(255, 255, 0, 255);
		playerHitValueColor = CP_Color_Create(204, 0, 0, 255);

		isParticleLoaded = true;

		// Load sprite here (default for all, save on constant init particles (loading file))
		SpriteParticleInit(&particleSpriteList[PARTICLE_TYPE_EXPLOSION], "explosion_particle.png", PARTICLEDEF_ANIM_PLAYTIME, PARTICLEDEF_ALPHA);
		SpriteParticleInit(&particleSpriteList[PARTICLE_TYPE_MENU_STAR], "menustar_particle.png", PARTICLEDEF_ANIM_PLAYTIME, PARTICLEDEF_ALPHA);
	}
}

// Call when entering scene, to reset/Setup all particles
// Uses cameraoffset to render properly
void ParticleInitGame(CP_Vector* cameraOffset)
{
	cameraOffsetPtr = cameraOffset;

	//if (cameraOffset == NULL)
	//{
	//	printf("Particle: NULL camera offset passed in!\n");
	//}

	ParticleInitSimple();
}

// Update the lifetime of the particles alive
void ParticleUpdate()
{
	for (int i = 0; i < PARTICLEDEF_MAX_NUMBER; i++)
	{
		// Check if not alive, if so, skip
		if (!particleList[i].isAlive)
			continue;

		particleList[i].lifeTime -= PARTICLEDEF_LIFETIME_DECR_RATE * CP_System_GetDt();

		// Update sprite here <special case>
		switch (particleList[i].type)
		{
		case PARTICLE_TYPE_EXPLOSION:
		{	// Update THIS particle's animTime and frameIndex
			SpriteParticleUpdate(
			/* Sprite to update */	&particleSpriteList[PARTICLE_TYPE_EXPLOSION],
			/* Current animTime */	&particleList[i].animTime,
			/* Current Frame */		&particleList[i].frameIndex);

			// Check if lifetime over
			if (particleList[i].lifeTime <= 0.f)
			{
				// set unalive
				particleList[i].isAlive = false;
			}
			break;
		}
		case PARTICLE_TYPE_MENU_STAR:
		{
			// Check if already out of bounds (set by MenuStarParticle)
			if (*(particleList[i].menuStarParticle.isWithinBorder) == false)
			{
				particleList[i].isAlive = false;
			}

			// Update THIS particle's animTime and frameIndex
			SpriteParticleUpdate(
				/* Sprite to update */	&particleSpriteList[PARTICLE_TYPE_MENU_STAR],
				/* Current animTime */	&particleList[i].animTime,
				/* Current Frame */		&particleList[i].frameIndex);
			break;
		}
		default:
		{
			// Check if lifetime over
			if (particleList[i].lifeTime <= 0.f)
			{
				// set unalive
				particleList[i].isAlive = false;
			}
			break;
		}
		}
	}
}

// Render Particles active
void ParticleRender()
{
	for (int i = 0; i < PARTICLEDEF_MAX_NUMBER; i++)
	{
		if (!particleList[i].isAlive)
			continue;

		char tempText[5] = { 0 };
		switch (particleList[i].type)
		{
		case PARTICLE_TYPE_EXPLOSION:
		{
			if (cameraOffsetPtr == NULL)		// cameraOffeset must not be NULL for this
				continue;

			// Render sprite here
			SpriteParticlePlayAnim(
			/* Sprite */				particleSpriteList[PARTICLE_TYPE_EXPLOSION],
			/* Frame to render */		particleList[i].frameIndex,
			/* Position to render */	CP_Vector_Set(particleList[i].posX + cameraOffsetPtr->x, particleList[i].posY + cameraOffsetPtr->y));
			break;
		}
		case PARTICLE_TYPE_ENEMY_HIT:
		{
			if (cameraOffsetPtr == NULL)		// cameraOffeset must not be NULL for this
				continue;


			// Text that shows damage
			CP_Settings_Fill(damageValueColor);
			sprintf_s(tempText, sizeof(tempText), "%d", (char)particleList[i].enemyHitParticle.damageNumber);

			CP_Settings_TextSize(ParticleEaseOutQuad(PARTICLEDEF_FONTSIZE_MAX, PARTICLEDEF_FONTSIZE_MIN, particleList[i].lifeTime * PARTICLEDEF_ONEDIVBYLIFETIME));
			CP_Font_DrawText(
				tempText,
				particleList[i].posX + cameraOffsetPtr->x,
				particleList[i].posY + cameraOffsetPtr->y - ParticleEaseOutQuad(PARTICLEDEF_TWEEN_MAX, PARTICLEDEF_TWEEN_MIN, particleList[i].lifeTime * PARTICLEDEF_ONEDIVBYLIFETIME));
			break;
		}
		case PARTICLE_TYPE_PLAYER_HIT:
		{
			if (cameraOffsetPtr == NULL)		// cameraOffeset must not be NULL for this
				continue;


			// Text that shows damage
			CP_Settings_Fill(playerHitValueColor);
			sprintf_s(tempText, sizeof(tempText), "%d", (char)particleList[i].playerHitParticle.damageNumber);

			CP_Settings_TextSize(ParticleEaseOutQuad(PARTICLEDEF_FONTSIZE_MAX, PARTICLEDEF_FONTSIZE_MIN, particleList[i].lifeTime * PARTICLEDEF_ONEDIVBYLIFETIME));
			CP_Font_DrawText(
				tempText,
				particleList[i].posX + cameraOffsetPtr->x,
				particleList[i].posY + cameraOffsetPtr->y - ParticleEaseOutQuad(PARTICLEDEF_TWEEN_MAX, PARTICLEDEF_TWEEN_MIN, particleList[i].lifeTime * PARTICLEDEF_ONEDIVBYLIFETIME));
			break;
		}
		case PARTICLE_TYPE_MENU_STAR:
		{
			// Render sprite here
			SpriteParticlePlayAnim(
				/* Sprite */				particleSpriteList[PARTICLE_TYPE_MENU_STAR],
				/* Frame to render */		particleList[i].frameIndex,
				/* Position to render */	CP_Vector_Set(particleList[i].menuStarParticle.sharedPosition->x, particleList[i].menuStarParticle.sharedPosition->y));
			break;
		}
		/*

		INSERT

		PARTICLE

		GRAPHICS OR ANIMATION

		HERE



		*/
		default:
		{
			// DEFAULT - Old explosion
			// Currently alpha based on lifetime
			CP_Settings_Fill(CP_Color_Create(128, 127, 65, (int)particleList[i].lifeTime));
			// Normalize to 0->1, then multiply by scale
			float size = ((PARTICLEDEF_LIFETIME - particleList[i].lifeTime) / PARTICLEDEF_LIFETIME) * PARTICLEDEF_SIZE_SCALE;
			// Render
			CP_Graphics_DrawCircle(particleList[i].posX + cameraOffsetPtr->x, particleList[i].posY + cameraOffsetPtr->y, size);
			break;
		}
		}
	}
}

// Call when exiting scene, "killing" all particles
void ParticleExit(void)
{
	for (int i = 0; i < PARTICLEDEF_MAX_NUMBER; i++)
	{
		particleList[i].isAlive = false;
	}
}

// Call when exiting GAME, UNLOAD all particles
void ParticleQuitGame()
{
	for (int i = 0; i < PARTICLEDEF_MAX_NUMBER; i++)
	{
		//CP_Image_Free(&particleImageList[i]); - NOT USED
		SpriteParticleReleaseVars(&particleSpriteList[i]);
	}
	isParticleLoaded = false;
}
#pragma endregion


#pragma region Particle Spawn Functions

// Spawn particles
void ParticleSpawn(PARTICLE_TYPE _type, float _posX, float _posY, float lifetime, int value)
{
	for (int i = 0; i < PARTICLEDEF_MAX_NUMBER; i++)
	{
		// Find one that is not alive
		if (!particleList[i].isAlive)
		{
			particleList[i].isAlive = true;

			particleList[i].type = _type;
			particleList[i].lifeTime = lifetime;
			particleList[i].posX = _posX;
			particleList[i].posY = _posY;
			// Sprite animation reset
			particleList[i].animTime = 0.0f;
			particleList[i].frameIndex = 0;

			// Special cases for that particle
			switch (_type)
			{
			case PARTICLE_TYPE_ENEMY_HIT:
			{
				particleList[i].enemyHitParticle.damageNumber = value;
				break;
			}
			case PARTICLE_TYPE_PLAYER_HIT:
			{
				particleList[i].playerHitParticle.damageNumber = value;
				break;
			}
			default:
				break;
			}

			return;
		}
	}
	// printf("Particle: No spawn, exceeded max particle amount!\n");
}

// USED ONLY BY MENU_STAR
void ParticleSpawnSharePos(PARTICLE_TYPE _type, CP_Vector* _sharedPos, float lifetime, bool* optionalValue)
{
	if (_type != PARTICLE_TYPE_MENU_STAR)
	{
		// printf("Particle: No spawn, wrong function called for particle type!\n");
		return;
	}

	for (int i = 0; i < PARTICLEDEF_MAX_NUMBER; i++)
	{
		// Find one that is not alive
		if (!particleList[i].isAlive)
		{
			particleList[i].isAlive = true;

			particleList[i].type = _type;
			particleList[i].lifeTime = lifetime;
			particleList[i].menuStarParticle.sharedPosition = _sharedPos;
			particleList[i].menuStarParticle.isWithinBorder = optionalValue;
			// Sprite animation reset
			particleList[i].animTime = 0.0f;
			particleList[i].frameIndex = 0;

			return;
		}
	}
	// printf("Particle: No spawn, exceeded max particle amount!\n");
}
#pragma endregion

// Popup calculation
static float ParticleEaseOutQuad(float start, float end, float value)
{
	end -= start;
	return -end * value * (value - 2) + start;
}
