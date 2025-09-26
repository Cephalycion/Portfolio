/*!
@copyright	DIGIPEN INSTITUTE OF TECHNOLOGY SINGAPORE
@file       WeaponInfo.h
@author     Chng Kai Rong Leonard(k.chng)
@course     CSD1401 Software Engineering Project 1
@section    a
@tutorial   Final Project
@date       18/11/2023
@brief      This file contains the declaration of an enum for WEAPON_TYPE,
			and a struct of WeaponInfo containing some variables.
*//*_________________________________________________________________________*/

#pragma once
#include "cprocessing.h"
#include "Utils.h"


typedef enum WEAPON_TYPE
{
	PLASMA_BALL,
	ORBITERS,
	ELECTRO_DOME
}WEAPON_TYPE;

typedef struct WeaponInfo
{
	int			level;
	float		baseDamage;
	float		baseSize;
	CP_Vector	cirOffset;
	CP_Image	image;
}WeaponInfo;
