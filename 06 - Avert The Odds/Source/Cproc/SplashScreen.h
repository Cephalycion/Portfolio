/*!
@copyright	DIGIPEN INSTITUTE OF TECHNOLOGY SINGAPORE
@file       SplashScreen.h
@author     Marc Alviz Evangelista(marcalviz.e)
@course     CSD1401 Software Engineering Project 1
@section    a
@tutorial   Final Project
@date       18/11/2023
@brief      This file contains the declarations of functions which run the
			splash screen of the game, where it flashes the school and team
			logo then changes to the main menu scene
*//*_________________________________________________________________________*/
#pragma once
#include <stdbool.h>
#include "cprocessing.h"
#include "Button.h"
#include "GameFlags.h"


// Logos used for splash screen
Button digipenLogo;
Button anvilFoxLogo;

// Timers used for the logos fading
float splashFadeInTimer;
float splashFadeOutTimer;
float digipenTimer;
float anvilFoxTimer;

// Bools used for order of logos
bool digipenFadeIn;
bool anvilFoxFadeIn;

bool onDigipenLogo;
bool goMainMenu;

void SplashScreenInit(void);
void SplashScreenUpdate(void);
void SplashScreenExit(void);
