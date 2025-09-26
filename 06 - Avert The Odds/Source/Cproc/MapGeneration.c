/*!
@copyright	DIGIPEN INSTITUTE OF TECHNOLOGY SINGAPORE
@file       MapGeneration.c
@author     Chng Kai Rong Leonard(k.chng)
@course     CSD1401 Software Engineering Project 1
@section    a
@tutorial   Final Project
@date       18/11/2023
@brief      This file contains the defination of a struct MAP_IMAGES,
			void GenerateMap, void LoadBackgroundImages, void DrawMap and
			void FreeMap.
*//*_________________________________________________________________________*/

#include "MapGeneration.h"

void GenerateMap(GameObject* player)
{
	LoadBackgroundImage();
	// Set Each Background Position
	float offset = 1000.f;
	float xPos = player->position.x;
	float yPos = player->position.y;
	map[4].position.x = xPos;
	map[4].position.y = yPos;

	map[1].position.x = xPos;
	map[7].position.x = xPos;

	map[3].position.y = yPos;
	map[5].position.y = yPos;

	// handle y position of the maps
	for (int i = 0; i < sizeof(map) / sizeof(map[0]); ++i)
	{
		
		if (i != 4)
		{
			switch (i)
			{
			case 0:
			case 1:
			case 2:
				map[i].position.y = yPos - offset;
				break;
			case 6:
			case 7:
			case 8:
				map[i].position.y = yPos + offset;
				break;
			default:
				break;
			}
		}
	}

	// handle x position of the maps
	for (int i = 0; i < sizeof(map) / sizeof(map[0]); ++i)
	{

		if (i != 4)
		{
			switch (i)
			{
			case 0:
			case 3:
			case 6:
				map[i].position.x = xPos - offset;
				break;
			case 2:
			case 5:
			case 8:
				map[i].position.x = xPos + offset;
				break;
			default:
				break;
			}
		}
	}
}

void LoadBackgroundImage(void)
{
	CP_Settings_ImageMode(CP_POSITION_CENTER);
	CP_Settings_ImageWrapMode(CP_IMAGE_WRAP_CLAMP);
	char str[50];
	char index[3];
	for (int i = 0; i < sizeof(map) / sizeof(map[0]); ++i)
	{
		// Load up images
		memset(str, '\0', sizeof(str));
		_itoa_s(i, index, sizeof(index), 10);
		strcat_s(str, sizeof(str), "Assets/background/");
		strcat_s(str, sizeof(str), index);
		strcat_s(str, sizeof(str), ".png");
		map[i].image = CP_Image_Load(str);
		
		// Set map size to images resolution 
		map[i].size = CP_Vector_Set((float)CP_Image_GetWidth(map[i].image),
								(float)CP_Image_GetHeight(map[i].image));
		// Set alpha
		map[i].alpha = 255;
	}

}

void DrawMap(CP_Vector cameraOffset)
{
	// CP_Draw
	for (int i = 0; i < sizeof(map) / sizeof(map[0]); ++i)
	{
		CP_Image_Draw(
			map[i].image,
			map[i].position.x + cameraOffset.x,
			map[i].position.y + cameraOffset.y,
			map[i].size.x,
			map[i].size.y,
			map[i].alpha);
	}
}

void FreeMap(void)
{
	for (int i = 0; i < sizeof(map) / sizeof(map[0]); ++i)
	{
		CP_Image_Free(&map[i].image);
	}
	
}