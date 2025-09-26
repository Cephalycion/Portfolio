/*!
@copyright	DIGIPEN INSTITUTE OF TECHNOLOGY SINGAPORE
@file       Obstacle.h
@author     Chng Kai Rong Leonard(k.chng)
@course     CSD1401 Software Engineering Project 1
@section    a
@tutorial   Final Project
@date       18/11/2023
@brief      This file contains the declaration of void LoadObstacle,
			void GenerateObstacle, void RelocateObstacle, void DrawObstacle
			and void FreeObstacle
*//*_________________________________________________________________________*/

#pragma once
#include "cprocessing.h"
#include "Utils.h"
#include "Game.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define MAX_OBSTACLE 40

CP_Image image;
int alpha;
float sizeRand;
int obstacleCount;

void LoadObstacle(void);

void GenerateObstacle(GameObject* obstacle, CP_Vector center);

int RelocateObstacle(GameObject* obstacle, CP_Vector center);

void DrawObstacle(GameObject* obstacle);

void FreeObstacle(void);