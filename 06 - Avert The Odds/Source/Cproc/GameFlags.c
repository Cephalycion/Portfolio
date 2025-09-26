/*!
@copyright	DIGIPEN INSTITUTE OF TECHNOLOGY SINGAPORE
@file       GameFlags.c
@author     Marc Alviz Evangelista(marcalviz.e),
@course     CSD1401 Software Engineering Project 1
@section    A
@tutorial   Final Project
@date       18/11/2023
@brief      This file contains the definitions of functions and variables
			that are initialized in one scene and read/set to be used in
			other scenes(like display resolution being set in main menu and
			used in game)
*//*_________________________________________________________________________*/
#include "GameFlags.h"

void InitFlags(void)
{
	displayRes = RES_720;
	mainToGame = false;
	gameToMain = false;
	splashToMain = false;
	gameToGame = false;
}

DISPLAY GetDisplay(void)
{
	return displayRes;
}

void SetDisplay(DISPLAY res)
{
	displayRes = res;
}

bool GetMTG(void)
{
	return mainToGame;
}

void SetMTG(bool b)
{
	mainToGame = b;
}

bool GetGTM(void)
{
	return gameToMain;
}

void SetGTM(bool b)
{
	gameToMain = b;
}

bool GetSTM(void)
{
	return splashToMain;
}

void SetSTM(bool b)
{
	splashToMain = b;
}

bool GetGTG(void)
{
	return gameToGame;
}

void SetGTG(bool b)
{
	gameToGame = b;
}
