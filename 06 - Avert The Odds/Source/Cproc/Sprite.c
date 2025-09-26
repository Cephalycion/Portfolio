/*!
@copyright	DIGIPEN INSTITUTE OF TECHNOLOGY SINGAPORE
@file       Sprite.c
@author     Marc Alviz Evangelista(marcalviz.e),
			Chan Kuan Fu Ryan (c.kuanfuryan)
@course     CSD1401 Software Engineering Project 1
@section    a
@tutorial   Final Project
@date       18/11/2023
@brief      This file contains the definitions of functions which pertains
			to sprites in game
*//*_________________________________________________________________________*/
#include "Sprite.h"


void SpriteUpdate(Sprite* sprite)
{
	if (!sprite->sprSingleFrame)
	{
		sprite->sprAnimTime += CP_System_GetDt() / sprite->sprAnimPlayTime;

		if (sprite->sprAnimTime > sprite->sprTimePerFrame)
		{
			sprite->sprAnimTime = 0.f;
			sprite->sprAnimIndex += 1;
			if (sprite->sprAnimIndex > sprite->sprTotalFrames - 1)
			{
				sprite->sprAnimIndex = 0;
			}
		}
	}
}


void SpriteBetterPlayAnim(Sprite sprite, CP_Vector pos)
{
	float spriteFrameLength = sprite.sprWidth / sprite.sprNumCols;
	
	int rowIndex = sprite.sprAnimIndex > sprite.sprNumCols - 1 ? sprite.sprAnimIndex / sprite.sprNumCols : 0;
	int colIndex = (sprite.sprAnimIndex - (rowIndex * sprite.sprNumCols)) % sprite.sprNumCols;
	CP_Image_DrawSubImage(sprite.sprRightSpritesheet,												// Image subimage is being drawn from
		pos.x, pos.y,																				// Position of where to draw 
		spriteFrameLength * sprite.sprScale.x, spriteFrameLength * sprite.sprScale.x,				// Width and Height of subimage
		spriteFrameLength * colIndex, spriteFrameLength * rowIndex,									// Left most pixel, Top most pixel
		spriteFrameLength * (colIndex + 1), spriteFrameLength * (rowIndex + 1), sprite.sprAlpha);	// Right most pixel, Bottom most pixel
}

void SpritePlayAnim(Sprite sprite, CP_Vector pos)
{
	if (!sprite.sprSingleFrame)
	{
		if (sprite.sprDir == RIGHT)
		{
			CP_Image_DrawSubImage(sprite.sprRightSpritesheet, pos.x, pos.y, sprite.sprHeight * sprite.sprScale.x, sprite.sprHeight * sprite.sprScale.y,
				sprite.sprHeight * sprite.sprAnimIndex, 0.f, sprite.sprHeight * (sprite.sprAnimIndex + 1), sprite.sprHeight, sprite.sprAlpha);
		}
		else
		{
			CP_Image_DrawSubImage(sprite.sprLeftSpriteSheet, pos.x, pos.y, sprite.sprHeight, sprite.sprHeight,
				sprite.sprHeight * sprite.sprAnimIndex, 0.f, sprite.sprHeight * (sprite.sprAnimIndex + 1), sprite.sprHeight, sprite.sprAlpha);
		}
	}
	else
	{
		if (sprite.sprDir == RIGHT)
		{
			CP_Image_Draw(sprite.sprRightSpritesheet, pos.x, pos.y, sprite.sprWidth, sprite.sprHeight, sprite.sprAlpha);
		}
		else
		{
			CP_Image_Draw(sprite.sprLeftSpriteSheet, pos.x, pos.y, sprite.sprWidth, sprite.sprHeight, sprite.sprAlpha);
		}
	}
}

// Ryan's index-specific sprite play (based off Jia Win's system)
void SpritePlayAnimIndex(Sprite sprite, CP_Vector pos, int index, float radius)
{
	CP_Image_DrawSubImage(
		sprite.sprRightSpritesheet, 
		pos.x, 
		pos.y, 
		radius, 
		radius,
		sprite.sprHeight * index, 0.f,
		sprite.sprHeight * (index + 1),
		sprite.sprHeight, 
		sprite.sprAlpha);
}

void SpriteParticlePlayAnim(SpriteParticle particle, int frameIndex, CP_Vector pos)
{
	CP_Image_DrawSubImage(particle.sprParticleSpritesheet, pos.x, pos.y, particle.sprHeight * particle.sprScale.x, particle.sprHeight * particle.sprScale.y,
		particle.sprHeight * frameIndex, 0.f, particle.sprHeight * (frameIndex + 1), particle.sprHeight, particle.sprAlpha);
}

void SpriteParticleUpdate(SpriteParticle* sprite, float* oldAnimTime, int* oldFrameIndex)
{
	//sprite->sprAnimTime += CP_System_GetDt() / sprite->sprAnimPlayTime;
	*oldAnimTime += CP_System_GetDt() / sprite->sprAnimPlayTime;

	if (*oldAnimTime > sprite->sprTimePerFrame)
	{
		*oldAnimTime = 0.f;
		*oldFrameIndex += 1;
		if (*oldFrameIndex > sprite->sprTotalFrames - 1)
		{
			*oldFrameIndex = 0;
		}
	}
}


void SpriteBetterInit(Sprite* sprite, char* spriteName, float animPlayTime, int alpha, int numRows, int numCols, int numFrames)
{
	sprite->sprAnimPlayTime = animPlayTime;
	sprite->sprAnimTime = 0;
	sprite->sprAnimIndex = 0;
	sprite->sprAlpha = alpha;
	sprite->sprDir = RIGHT;
	sprite->sprScale = CP_Vector_Set(1.f, 1.f);

	char imageString[100];
	memset(imageString, '\0', sizeof(imageString));

	strcat_s(imageString, sizeof(imageString), "Assets/");
	strcat_s(imageString, sizeof(imageString), spriteName);

	sprite->sprRightSpritesheet = CP_Image_Load(imageString);
	sprite->sprLeftSpriteSheet = CP_Image_Load(imageString);

	sprite->sprSingleFrame = false;

	sprite->sprHeight = (float)CP_Image_GetHeight(sprite->sprRightSpritesheet);
	sprite->sprWidth = (float)CP_Image_GetWidth(sprite->sprRightSpritesheet);
	sprite->sprNumRows = numRows;
	sprite->sprNumCols = numCols;
	sprite->sprTotalFrames = (float)numFrames;


	sprite->sprTimePerFrame = sprite->sprAnimPlayTime / sprite->sprTotalFrames;
}

void SpriteInit(Sprite* sprite, char* spriteName, bool singleFrame, float animPlayTime, int alpha)
{
	sprite->sprAnimPlayTime = animPlayTime;
	sprite->sprAnimTime = 0;
	sprite->sprAnimIndex = 0;
	sprite->sprAlpha = alpha;
	sprite->sprDir = RIGHT;
	sprite->sprScale = CP_Vector_Set(1.f, 1.f);

	char imageString[100];
	memset(imageString, '\0', sizeof(imageString));

	strcat_s(imageString, sizeof(imageString), "Assets/right_");
	strcat_s(imageString, sizeof(imageString), spriteName);

	sprite->sprRightSpritesheet = CP_Image_Load(imageString);

	memset(imageString, '\0', sizeof(imageString));

	strcat_s(imageString, sizeof(imageString), "Assets/left_");
	strcat_s(imageString, sizeof(imageString), spriteName);

	sprite->sprLeftSpriteSheet = CP_Image_Load(imageString);

	sprite->sprHeight = (float)CP_Image_GetHeight(sprite->sprRightSpritesheet);
	sprite->sprWidth = (float)CP_Image_GetWidth(sprite->sprRightSpritesheet);

	if (singleFrame)
	{
		sprite->sprSingleFrame = true;
		sprite->sprTotalFrames = 1;
	}
	else
	{
		sprite->sprSingleFrame = false;
		// frames = width / height BECAUSE each frame has the same width and height
		sprite->sprTotalFrames = sprite->sprWidth / sprite->sprHeight;
		sprite->sprTimePerFrame = sprite->sprAnimPlayTime / sprite->sprTotalFrames;
	}
}

// Ryan's directionless sprite (based off Marc's system)
void SpriteInitDirectionless(Sprite* sprite, char* spriteName, bool singleFrame, float animPlayTime, int alpha)
{
	sprite->sprAnimPlayTime = animPlayTime;
	sprite->sprAnimTime = 0;
	sprite->sprAnimIndex = 0;
	sprite->sprAlpha = alpha;
	sprite->sprDir = RIGHT;

	char imageString[100];
	memset(imageString, '\0', sizeof(imageString));

	strcat_s(imageString, sizeof(imageString), "Assets/");
	strcat_s(imageString, sizeof(imageString), spriteName);

	// Right and left sprite sheets are the same
	sprite->sprRightSpritesheet = CP_Image_Load(imageString);
	sprite->sprLeftSpriteSheet = CP_Image_Load(imageString);

	sprite->sprHeight = (float)CP_Image_GetHeight(sprite->sprRightSpritesheet);
	sprite->sprWidth = (float)CP_Image_GetWidth(sprite->sprRightSpritesheet);

	if (singleFrame)
	{
		sprite->sprSingleFrame = true;
		sprite->sprTotalFrames = 1;
	}
	else
	{
		sprite->sprSingleFrame = false;
		// frames = width / height BECAUSE each frame has the same width and height
		sprite->sprTotalFrames = sprite->sprWidth / sprite->sprHeight;
		sprite->sprTimePerFrame = sprite->sprAnimPlayTime / sprite->sprTotalFrames;
	}
}

void SpriteParticleInit(SpriteParticle* sprite, char* spriteName, float playTime, int alpha)
{

	sprite->sprAnimPlayTime = playTime;
	sprite->sprAlpha = alpha;
	sprite->sprScale = CP_Vector_Set(1.f, 1.f);

	char imageString[100];
	memset(imageString, '\0', sizeof(imageString));

	strcat_s(imageString, sizeof(imageString), "Assets/");
	strcat_s(imageString, sizeof(imageString), spriteName);

	sprite->sprParticleSpritesheet = CP_Image_Load(imageString);

	sprite->sprHeight = (float)CP_Image_GetHeight(sprite->sprParticleSpritesheet);
	sprite->sprWidth = (float)CP_Image_GetWidth(sprite->sprParticleSpritesheet);

	sprite->sprTotalFrames = sprite->sprWidth / sprite->sprHeight;
	sprite->sprTimePerFrame = sprite->sprAnimPlayTime / sprite->sprTotalFrames;
}


void SpriteReleaseVars(Sprite* sprite)
{
	CP_Image_Free(&sprite->sprLeftSpriteSheet);
	CP_Image_Free(&sprite->sprRightSpritesheet);
}

void SpriteParticleReleaseVars(SpriteParticle* sprite)
{
	CP_Image_Free(&sprite->sprParticleSpritesheet);
}
