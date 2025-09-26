/*!
@copyright	DIGIPEN INSTITUTE OF TECHNOLOGY SINGAPORE
@file       Utils.h
@author     Chan Kuan Fu Ryan (c.kuanfuryan),
			Chng Kai Rong Leonard(k.chng),
			Lau Jia Win(jiawin.lau)
@course     CSD1401 Software Engineering Project 1
@section    a
@tutorial   Final Project
@date       18/11/2023
@brief      This file contains the declarations of functions which can check 
			for intersection or collision of objects, as well as 
			CreateGameObject() which is used for the initialisation of all new
			objects in Game.c. Also includes structs for Tweens and GameEvents.
*//*_________________________________________________________________________*/

#pragma once
#include "cprocessing.h"

typedef enum GAMEOBJ_TYPE
{
	OBJ_CLEAR,
	OBJ_PLAYER,
	OBJ_ALLY_PROJECTILE,
	OBJ_ALLY_MULTI_SHOT,
	OBJ_ALLY_CIRCLING,
	OBJ_ALLY_AOE,
	OBJ_ENEMY,
	OBJ_ENEMY_PROJECTILE,
	OBJ_ENEMY_ELITE,
	OBJ_ENEMY_BOSS,
	OBJ_OBSTACLE
} GAMEOBJ_TYPE;

typedef struct GameObject
{
	GAMEOBJ_TYPE	type;
	CP_Vector		positionBase;
	CP_Vector		position;
	CP_Vector		velocity;				// Not set by creator function, remember to set if you need it!
	int				active;					// Set automatically by creator function to 1
	float			healthMaximum;
	float			healthCurrent;
	float			speed;
	float			radius;
	float			lifetime;
	float			tweening;				// Not set by creator function, remember to set if you need it!
	int				spriteIndex;			// Set automatically by creator function to 0
	int				spriteIndexLimit;		// Not set by creator function, remember to set if you need it!
	float			spriteAnimTime;			// Set automatically by creator function to 0.0f
	float			spriteAnimTimeLimit;	// Set automatically by creator function to 1.0f
	CP_Vector		cirOffset;
	float			range;
	float			cirHitTimer;
} GameObject;

//typedef struct Weapon
//{
//	float attack;
//} Weapon;

typedef struct Tween
{
	int active;
	int objectIndex;
	CP_Vector direction;
	float speed;
	float decay;
} Tween;

int IsAreaClicked(float areaCenterX, float areaCenterY, float areaWidth, float areaHeight, float clickX, float clickY);
int AreCirclesIntersecting(GameObject* object1, GameObject* object2);
int AreObstacleIntersecting(GameObject* object1, GameObject* object2);
int IsMovingCloser(GameObject* object1, GameObject* object2);
CP_Vector VectorProjection(CP_Vector u, CP_Vector v);

GameObject CreateGameObject(
	GAMEOBJ_TYPE	type,
	CP_Vector		positionBase,
	CP_Vector		position,
	float			healthMaximum,
	float			speed,
	float			radius,
	float			lifetime
);

typedef enum GameEvent {
	GE_PROJ_HIT,
	GE_PLAYER_SHOOT,
	GE_PLAYER_MULTI,
	GE_PLAYER_HURT,
	GE_ENEMY_HURT,
	GE_ENEMY_DIE,

	GE_MAX
} GameEvent;