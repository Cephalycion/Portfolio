/*!
@copyright	DIGIPEN INSTITUTE OF TECHNOLOGY SINGAPORE
@file 		MenuStarParticle.h
@author 	Lau Jia Win (jiawin.lau)
@course     CSD1401 Software Engineering Project 1
@section    a
@tutorial   Final Project
@date 		14/11/2023
@brief 		This file contains the function declarations and struct for
			the logic behind stars in the background of main menu scene,
			using the particle system.
*//*______________________________________________________________________*/
#pragma once
#include "CProcessing.h"
#include <stdbool.h>

#define MSPDEF_MAX_STARS		20
#define MSPDEF_DIR				CP_Vector_Set(1.F, -1.F)
#define MSPDEF_SPEED			100.F

#define MSPDEF_SPAWN_LEFT_X		100		// Able to spawn slightly outside on left
#define MSPDEF_SPAWN_LEFT_Y		-50		// Able to spawn slightly outside on left
#define MSPDEF_WAVE_TIMER_MIN	3
#define MSPDEF_WAVE_TIMER_MAX	5
#define MSPDEF_WAVE_SPAWN_MIN	3
#define MSPDEF_WAVE_SPAWN_MAX	5

// Keep track of stars in the scene
typedef struct StarObject
{
	bool isAlive;
	CP_Vector position;

} StarObject;

// ==============================================================
// Call in MainMenu.c
void MenuStarParticleInit();
void MenuStarParticleUpdate();
void MenuStarParticleSetBorder(int newBorderX, int newBorderY);
//
// ==============================================================
