/*!
@copyright	DIGIPEN INSTITUTE OF TECHNOLOGY SINGAPORE
@file       MapGeneration.h
@author     Chng Kai Rong Leonard(k.chng)
@course     CSD1401 Software Engineering Project 1
@section    a
@tutorial   Final Project
@date       18/11/2023
@brief      This file contains the declaration of a struct MAP_IMAGES,
			void GenerateMap, void LoadBackgroundImages, void DrawMap and
			void FreeMap.
*//*_________________________________________________________________________*/

#pragma once
#include "cprocessing.h"
#include "Utils.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct MAP_IMAGES
{
	CP_Image image;
	CP_Vector position;
	CP_Vector size;
	int alpha;
}MAP_IMAGES;

MAP_IMAGES map[9];


void GenerateMap(GameObject *player);

void LoadBackgroundImage(void);

void DrawMap(CP_Vector cameraOffset);

void FreeMap(void);