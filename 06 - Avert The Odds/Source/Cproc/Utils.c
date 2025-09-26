/*!
@copyright	DIGIPEN INSTITUTE OF TECHNOLOGY SINGAPORE
@file       Utils.c
@author     Chan Kuan Fu Ryan (c.kuanfuryan)
@course     CSD1401 Software Engineering Project 1
@section    a
@tutorial   Final Project
@date       18/11/2023
@brief      This file contains the definitions of functions which can check for
            intersection or collision of objects, as well as CreateGameObject()
            which is used for the initialisation of all new objects in Game.c
*//*_________________________________________________________________________*/

#include "Utils.h"

int IsAreaClicked(float areaCenterX, float areaCenterY, float areaWidth, float areaHeight, float clickX, float clickY)
{
    if (clickX < areaCenterX + (areaWidth / 2) &&
        clickX > areaCenterX - (areaWidth / 2) &&
        clickY > areaCenterY - (areaHeight / 2) &&
        clickY < areaCenterY + (areaHeight / 2))
        return 1;
    else
        return 0;
}

int AreCirclesIntersecting(GameObject *object1, GameObject *object2)
{
    float distance = CP_Vector_Distance(
        CP_Vector_Set(object1->position.x, object1->position.y), 
        CP_Vector_Set(object2->position.x, object2->position.y)
    );

    if (distance <= (object1->radius + object2->radius))
        return 1;
    else
        return 0;
}

int AreObstacleIntersecting(GameObject* object1, GameObject* object2)
{
    float distance = CP_Vector_Distance(
        CP_Vector_Set(object1->position.x, object1->position.y),
        CP_Vector_Set(object2->position.x, object2->position.y)
    );

    if (distance <= (object1->radius + object2->radius  + 100.0f))
        return 1;
    else
        return 0;
}

int IsMovingCloser(GameObject* object1, GameObject* object2)
{
    float distance1 = CP_Vector_Distance(
        CP_Vector_Set(object1->position.x, object1->position.y),
        CP_Vector_Set(object2->position.x, object2->position.y)
    );

    float distance2 = CP_Vector_Distance(
        CP_Vector_Set(object1->position.x + object1->velocity.x, object1->position.y + object1->velocity.y),
        // (object2->position.x + object2->velocity.x, object2->position.y + object2->velocity.y)
        CP_Vector_Set(object2->position.x, object2->position.y)
    );

    return (distance2 < distance1);
}

CP_Vector VectorProjection(CP_Vector u, CP_Vector v)
{
    return CP_Vector_Scale(v, CP_Vector_DotProduct(u, v) / CP_Vector_DotProduct(v, v));
}

GameObject CreateGameObject(
	GAMEOBJ_TYPE	type,
	CP_Vector		positionBase,
	CP_Vector		position,
	float			healthMaximum,
	float			speed,
	float			radius,
	float			lifetime)
{
	GameObject object;

	object.active = 1;
    object.spriteIndex = 0;
    object.spriteAnimTime = 0.0f;
    object.spriteAnimTimeLimit = 1.0f;
	object.type = type;
	object.positionBase = positionBase;
	object.position = position;
	object.healthMaximum = healthMaximum;
    object.healthCurrent = healthMaximum;
	object.speed = speed;
	object.radius = radius;
	object.lifetime = lifetime;

	return object;
}