/*!
@copyright	DIGIPEN INSTITUTE OF TECHNOLOGY SINGAPORE
@file       Sprite.c
@author     Marc Alviz Evangelista(marcalviz.e),
			Chan Kuan Fu Ryan (c.kuanfuryan)
@course     CSD1401 Software Engineering Project 1
@section    a
@tutorial   Final Project
@date       18/11/2023
@brief      This file contains the declarations of functions which pertains
			to sprites in game
*//*_________________________________________________________________________*/
#pragma once
#include "cprocessing.h"
#include <stdbool.h>

typedef enum SPRITE_DIRECTION
{
	SPRITE_DEFAULT = 0,
	LEFT,
	RIGHT
} SPRITE_DIRECTION;

typedef struct Sprite {

	CP_Image sprRightSpritesheet;
	CP_Image sprLeftSpriteSheet;

	SPRITE_DIRECTION sprDir;
	
	bool sprSingleFrame;	// Static sprite

	int sprAlpha;

	int sprNumRows;
	int sprNumCols;

	float sprHeight;
	float sprWidth;

	CP_Vector sprScale;

	float sprTotalFrames;	// Number of frames of the animation loop
	float sprTimePerFrame;
	float sprAnimPlayTime;	// Time for the animation to loop through
	float sprAnimTime;		// Current time through the animation

	int sprAnimIndex;		// Current frame of the animation loop

} Sprite;

typedef struct SpriteParticle {
	CP_Image sprParticleSpritesheet;

	int sprAlpha;

	float sprHeight;
	float sprWidth;

	CP_Vector sprScale;

	float sprTotalFrames;	// Number of frames of the animation loop
	float sprTimePerFrame;
	float sprAnimPlayTime;	// Time for the animation to loop through

} SpriteParticle;

// Specifically for particles 
void SpriteParticleInit(SpriteParticle* sprite, char* spriteName, float playTime, int alpha);
void SpriteBetterInit(Sprite* sprite, char* spriteName, float animPlayTime, int alpha, int numRows, int numCols, int numFrames);

// Plays the animation at frame(int) and pos
void SpriteParticlePlayAnim(SpriteParticle sprite, int frameIndex, CP_Vector pos);
// Update the frame number, given a frame number and anim time as input
void SpriteParticleUpdate(SpriteParticle* sprite, float* oldAnimTime, int* oldFrameIndex);

// Releases the CP image inside the sprite
void SpriteParticleReleaseVars(SpriteParticle* sprite);

// Takes in the sprite to init, the name of the sprite image, if its a single frame, animation time and alpha
void SpriteInit(Sprite* sprite, char* spriteName, bool singleFrame, float animPlayTime, int alpha);
void SpriteInitDirectionless(Sprite* sprite, char* spriteName, bool singleFrame, float animPlayTime, int alpha);

// Draws the sprite at its current index in the spritesheet
void SpritePlayAnim(Sprite sprite, CP_Vector pos);
void SpritePlayAnimIndex(Sprite sprite, CP_Vector pos, int index, float radius);

void SpriteBetterPlayAnim(Sprite sprite, CP_Vector pos);
// Updates the sprite's timer and index according to the amount of time passed
void SpriteUpdate(Sprite* sprite);

// Releases the CP image inside the sprite
void SpriteReleaseVars(Sprite* sprite);