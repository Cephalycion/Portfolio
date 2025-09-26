/*!
@copyright	DIGIPEN INSTITUTE OF TECHNOLOGY SINGAPORE
@file       Obstacle.c
@author     Chng Kai Rong Leonard(k.chng)
@course     CSD1401 Software Engineering Project 1
@section    a
@tutorial   Final Project
@date       18/11/2023
@brief      This file contains the defination of void LoadObstacle,
			void GenerateObstacle, void RelocateObstacle, void DrawObstacle
			and void FreeObstacle
*//*_________________________________________________________________________*/
#include "Obstacle.h"

void LoadObstacle(void)
{
	// Load Imagesddddddd
	CP_Settings_ImageMode(CP_POSITION_CENTER);
	CP_Settings_ImageWrapMode(CP_IMAGE_WRAP_CLAMP);
	image = CP_Image_Load("Assets/obstacles.png");
	alpha = 255;
}

void GenerateObstacle(GameObject* obstacle, CP_Vector center)
{
	sizeRand = ((float)(rand()) / (float)(RAND_MAX)) * (200.0f - 50.0f) + 50.0f;
	float offset = sizeRand + 100.0f;
	float xRand = ((float)(rand()) / (float)(RAND_MAX)) * ((center.x + MAP_BOUNDS_X - offset) - (-MAP_BOUNDS_X + center.x + offset)) + (-MAP_BOUNDS_X + center.x + offset);
	float yRand = ((float)(rand()) / (float)(RAND_MAX)) * ((center.y + MAP_BOUNDS_Y - offset) - (-MAP_BOUNDS_Y + center.y + offset)) + (-MAP_BOUNDS_Y + center.y + offset);

	*obstacle = CreateGameObject(
		/*	type			*/	OBJ_OBSTACLE, 
		/*	positionBase	*/	CP_Vector_Zero(), 
		/*	position		*/	CP_Vector_Zero(), 
		/*	healthMaximum	*/	0.0f, 
		/*	speed			*/	0.0f, 
		/*	radius			*/	sizeRand,
		/*	lifetime		*/	0.0f
	);
	obstacleCount = 0;
	if (!RelocateObstacle(obstacle, gameCenter))
	{
		GameClearObject(obstacle);
	}
}


int RelocateObstacle(GameObject* obstacle, CP_Vector center)
{
	sizeRand = ((float)(rand()) / (float)(RAND_MAX)) * (200.0f - 50.0f) + 50.0f;
	float offset = sizeRand + 50.0f;
	float xRand = ((float)(rand()) / (float)(RAND_MAX)) * ((center.x + MAP_BOUNDS_X - offset) - (-MAP_BOUNDS_X + center.x + offset)) + (-MAP_BOUNDS_X + center.x + offset);
	float yRand = ((float)(rand()) / (float)(RAND_MAX)) * ((center.y + MAP_BOUNDS_Y - offset) - (-MAP_BOUNDS_Y + center.y + offset)) + (-MAP_BOUNDS_Y + center.y + offset);
	obstacle->positionBase = CP_Vector_Set(xRand, yRand);
	obstacle->position = obstacle->positionBase;

	for (int j = 0; j < sizeof(objects) / sizeof(objects[0]); ++j)
	{
		if (&objects[j] != obstacle)
		{
			while ((objects[j].type == OBJ_PLAYER || objects[j].type == OBJ_OBSTACLE) && AreObstacleIntersecting(obstacle, &objects[j]))
			{
				if (obstacleCount < 5)
				{
					RelocateObstacle(obstacle, gameCenter);
					++obstacleCount;
				}
			}
		}
	}
	if (obstacleCount < 5)
		return 1;
	else
		return 0;
}

void DrawObstacle(GameObject* obstacle)
{
	/*CP_Settings_Fill(yellow);
	CP_Graphics_DrawCircle(
		obstacle->position.x,
		obstacle->position.y,
		obstacle->radius * 2.f
	);*/

	CP_Image_Draw(
		image,
		obstacle->position.x,
		obstacle->position.y,
		obstacle->radius * 2.f,
		obstacle->radius * 2.f,
		alpha);
}

void FreeObstacle(void)
{
	// Free Images
	CP_Image_Free(&image);
}