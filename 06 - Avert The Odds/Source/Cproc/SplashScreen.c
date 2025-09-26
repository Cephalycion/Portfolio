/*!
@copyright	DIGIPEN INSTITUTE OF TECHNOLOGY SINGAPORE
@file       SplashScreen.c
@author     Marc Alviz Evangelista(marcalviz.e)
@course     CSD1401 Software Engineering Project 1
@section    a
@tutorial   Final Project
@date       18/11/2023
@brief      This file contains the definitions of functions which run the
			splash screen of the game, where it flashes the school and team
			logo then changes to the main menu scene
*//*_________________________________________________________________________*/
#include "SplashScreen.h"
#include "MainMenu.h"



void SplashScreenInit(void)
{
	CP_System_SetWindowTitle("AvertTheOdds");
	CP_Settings_ImageMode(CP_POSITION_CENTER);
	CP_Settings_ImageWrapMode(CP_IMAGE_WRAP_CLAMP);
	CP_System_SetWindowSize(1920, 1080);

	splashFadeInTimer = 2.f;
	splashFadeOutTimer = 2.f;
	digipenTimer = 0.f;
	anvilFoxTimer = 0.f;

	goMainMenu = false;
	onDigipenLogo = true;

	digipenFadeIn = true;
	anvilFoxFadeIn = true;

	//CP_System_ShowCursor(false);

	ButtonInit(&digipenLogo, CP_Vector_Set((float)CP_System_GetWindowWidth() / 2.f, (float)CP_System_GetWindowHeight() / 2.f), CP_Vector_Set(1.f, 1.f), 0,
		"DigiPen_Singapore_WEB_WHITE.png", false, "", false, "", false);
	ButtonInit(&anvilFoxLogo, CP_Vector_Set((float)CP_System_GetWindowWidth() / 2.f, (float)CP_System_GetWindowHeight() / 2.f), CP_Vector_Set(1.f, 1.f), 0,
		"AnvilFoxLogo.png", false, "", false, "", false);

	for (int j = RES_720; j < DISPLAY_END; ++j)
	{
		ButtonSetRes(&digipenLogo, j, CP_Vector_Set((float)CP_System_GetWindowWidth(), (float)CP_System_GetWindowHeight()));
		ButtonSetRes(&anvilFoxLogo, j, CP_Vector_Set((float)CP_System_GetWindowWidth(), (float)CP_System_GetWindowHeight()));
	}

	CP_System_SetWindowSize(1280, 720);

	digipenLogo.btnPos = digipenLogo.btnPosRes720;
	digipenLogo.btnScale = digipenLogo.btnScaleRes720;
	anvilFoxLogo.btnPos = anvilFoxLogo.btnPosRes720;
	anvilFoxLogo.btnScale = anvilFoxLogo.btnScaleRes720;
	InitFlags();
	SetSTM(true);

}

void SplashScreenUpdate(void)
{
	CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));

	if (!goMainMenu)
	{
		if (onDigipenLogo)
		{
			if (digipenFadeIn)
			{
				digipenLogo.btnAlpha += (int)((CP_System_GetDt() / splashFadeInTimer) * 255.f);
				if ((float)digipenLogo.btnAlpha >= 255.f)
				{
					digipenFadeIn = false;
				}
			}
			else
			{
				digipenLogo.btnAlpha -= (int)((CP_System_GetDt() / splashFadeOutTimer) * 255.f);
				if ((float)digipenLogo.btnAlpha < 0.f)
				{
					digipenLogo.btnAlpha = 0;
					onDigipenLogo = false;
				}
			}
			ButtonDraw(digipenLogo, 0.f);
		}
		else
		{
			if (anvilFoxFadeIn)
			{
				anvilFoxLogo.btnAlpha += (int)((CP_System_GetDt() / splashFadeInTimer) * 255.f);
				if ((float)anvilFoxLogo.btnAlpha > 255.f)
				{
					anvilFoxFadeIn = false;
				}
			}
			else
			{
				anvilFoxLogo.btnAlpha -= (int)((CP_System_GetDt() / splashFadeOutTimer) * 255.f);
				if ((float)anvilFoxLogo.btnAlpha < 0.f)
				{
					anvilFoxLogo.btnAlpha = 0;
					goMainMenu = true;
				}


			}
			ButtonDraw(anvilFoxLogo, 0.f);

		}
	}
	else
	{
		CP_Engine_SetNextGameState(MainMenuInit, MainMenuUpdate, MainMenuExit);
	}
}

void SplashScreenExit(void)
{
	ButtonReleaseVars(&digipenLogo);
	ButtonReleaseVars(&anvilFoxLogo);
}
