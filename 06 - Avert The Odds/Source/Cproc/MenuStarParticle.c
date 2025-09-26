/*!
@copyright	DIGIPEN INSTITUTE OF TECHNOLOGY SINGAPORE
@file 		MenuStarParticle.c
@author 	Lau Jia Win (jiawin.lau)
@course     CSD1401 Software Engineering Project 1
@section    a
@tutorial   Final Project
@date 		14/11/2023
@brief 		This file contains the function definitions for
			the logic behind stars in the background of main menu scene,
			using the particle system.
*//*______________________________________________________________________*/
#include "MenuStarParticle.h"	// StarObject struct
#include "Particle.h"			// Particle system
#include <stdlib.h>				// rand, srand
#include <time.h>				// time for rand

// ===== Private variables =====
float waveTimer = 0;
StarObject starObjectList[MSPDEF_MAX_STARS];

int screenBorderX = 720;
int screenBorderY = 1270;
// =============================

#pragma region Private Functions

// Get a random spawn cooldown time
float MSPGetRandomTime()
{
	return (float)(rand() % (MSPDEF_WAVE_TIMER_MAX + 1 - MSPDEF_WAVE_TIMER_MIN) + MSPDEF_WAVE_TIMER_MIN);
}

// Get a random number to spawn
int MSPGetNumberToSpawn()
{
	return (rand() % (MSPDEF_WAVE_SPAWN_MAX + 1 - MSPDEF_WAVE_SPAWN_MIN) + MSPDEF_WAVE_SPAWN_MIN);
}

// Get a random position in the screen
float MSPGetRandomPos(int maxRange, int minRange)
{
	return (float)(rand() % (maxRange + 1 - minRange) + minRange);
}

// Get any inactive star objects
StarObject* MSPGetStarObject()
{
	for (int i = 0; i < MSPDEF_MAX_STARS; i++)
	{
		if (!starObjectList[i].isAlive)
			return &starObjectList[i];
	}

	return NULL;
}
#pragma endregion

#pragma region Update Functions (Call from MainMenu.c)

// Init and reset star positions and srand
void MenuStarParticleInit()
{
	for (int i = 0; i < MSPDEF_MAX_STARS; i++)
	{
		starObjectList[i].position = CP_Vector_Zero();
	}

	srand((unsigned int)time(0));
	waveTimer = 0.1f;
}

// Spawn, move and check boundary of stars
void MenuStarParticleUpdate()
{
	waveTimer -= CP_System_GetDt();

	if (waveTimer <= 0.f)
	{
		int num = MSPGetNumberToSpawn();
		for (int i = 0; i < num; i++)
		{
			StarObject* star = MSPGetStarObject();
			// Spawn a star
			if (star)
			{
				star->isAlive = true;
				star->position.x = MSPGetRandomPos(screenBorderX, MSPDEF_SPAWN_LEFT_X);
				star->position.y = MSPGetRandomPos(screenBorderY, 0);
				ParticleSpawnSharePos(PARTICLE_TYPE_MENU_STAR, &star->position, 999.f, &star->isAlive);
			}
		}
		waveTimer = MSPGetRandomTime();
	}

	//======Movement======
	// Find velocity
	CP_Vector movement = CP_Vector_Set(MSPDEF_DIR.x * MSPDEF_SPEED * CP_System_GetDt(), MSPDEF_DIR.y * MSPDEF_SPEED * CP_System_GetDt());

	for (int i = 0; i < MSPDEF_MAX_STARS; i++)
	{
		// Check if alive
		if (!starObjectList[i].isAlive)
			continue;

		// Move in direction
		starObjectList[i].position = CP_Vector_Add(starObjectList[i].position, movement);


		// Check if out of bounds
		if (starObjectList[i].position.x > screenBorderX || starObjectList[i].position.x < -MSPDEF_SPAWN_LEFT_X ||
			starObjectList[i].position.y > screenBorderY || starObjectList[i].position.y < 0)
		{
			starObjectList[i].isAlive = false;
		}
	}
}

// Set the border (Call when resolution is changed)
void MenuStarParticleSetBorder(int newBorderX, int newBorderY)
{
	screenBorderX = newBorderX;
	screenBorderY = newBorderY;
}
#pragma endregion
