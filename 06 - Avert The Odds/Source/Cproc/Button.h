/*!
@copyright	DIGIPEN INSTITUTE OF TECHNOLOGY SINGAPORE
@file       Button.h
@author     Marc Alviz Evangelista(marcalviz.e),
@course     CSD1401 Software Engineering Project 1
@section    a
@tutorial   Final Project
@date       18/11/2023
@brief      This file contains the declarations of functions which pertains
			to buttons in game
*//*_________________________________________________________________________*/

#pragma once
#include "cprocessing.h"
#include "Utils.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

//typedef void (*Draw)(struct Button*, float);
//typedef bool (*CheckClick)(struct Button*, CP_Vector);
//typedef void (*Release)(struct Button*);

typedef struct Button
{
	// Drawing variables
	CP_Image btnImage;
	CP_Image btnImageHover;

	CP_Vector btnPos;
	CP_Vector btnScale;

	// Store these set values in here after init
	CP_Vector btnPosRes720;
	CP_Vector btnPosRes1080;
	CP_Vector btnPosResFullscreen;

	CP_Vector btnScaleRes720;
	CP_Vector btnScaleRes1080;
	CP_Vector btnScaleResFullscreen;

	int btnAlpha;
	bool btnHovering;
	bool btnInteractable;

	// Other button features
	CP_Sound btnSoundEffect;
	CP_Font btnFont;

	// Functions for the button
	//Draw btn_draw;
	//CheckClick btn_isClicked;
	//Release btn_release;
} Button;

// Init Buttons with its reference, position, scale, image name, interactability, sound effect and font(last 2 optional)
void ButtonInit(Button* button, CP_Vector pos, CP_Vector scale, int alpha, char* imageName, bool interactable, char* soundName, bool hasSound, char* fontName, bool hasFont);

// Draws a button by reading the info stored in the struct object
void ButtonDraw(Button button, float);

// Area Checking for Button and Mouse
bool ButtonCheckClick(Button* button, CP_Vector click);

bool ButtonCheckHover(Button* button, CP_Vector mouse);

// Sets the various display scales and positions for UI buttons for 1 time math for future reading
void ButtonSetRes(Button* button, int newRes, CP_Vector oldRes);

// Frees all the cprocessing variables inside the b utton
void ButtonReleaseVars(Button* button);

