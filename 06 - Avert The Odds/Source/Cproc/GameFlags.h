/*!
@copyright	DIGIPEN INSTITUTE OF TECHNOLOGY SINGAPORE
@file       GameFlags.h
@author     Marc Alviz Evangelista(marcalviz.e),
@course     CSD1401 Software Engineering Project 1
@section    A
@tutorial   Final Project
@date       18/11/2023
@brief      This file contains the declarations of functions and variables
			that are initialized in one scene and read/set to be used in
			other scenes(like display resolution being set in main menu and 
			used in game)
*//*_________________________________________________________________________*/
#pragma once
// This file is used to init, store, and read flags, used for scene transitions, and 
// variables used in multiple scenes(eg display resolution 
#include "cprocessing.h"
#include <stdbool.h>


// For Game and Main Menu Use, changing resolutions
typedef enum DISPLAY
{
	DISPLAY_DEFAULT = 0,
	RES_720,
	RES_1080,
	RES_FULLSCREEN,

	DISPLAY_END
}DISPLAY;

static DISPLAY displayRes;
static bool mainToGame;
static bool gameToMain;
static bool splashToMain;
static bool gameToGame;

// Called in splashscreen scene to set the default values for the flags
void InitFlags(void);

// Reading and Setting functions of the flags
DISPLAY GetDisplay(void);
void SetDisplay(DISPLAY);

bool GetMTG(void);
void SetMTG(bool);

bool GetGTM(void);
void SetGTM(bool);

bool GetSTM(void);
void SetSTM(bool);

bool GetGTG(void);
void SetGTG(bool);