/*!
@copyright	DIGIPEN INSTITUTE OF TECHNOLOGY SINGAPORE
@file       Game.c
@author     Chan Kuan Fu Ryan (c.kuanfuryan),
			Marc Alviz Evangelista(),
			Chng Kai Rong Leonard(k.chng),
			Gavin Ang Jun Liang(ang.g),
			Lau Jia Win(jiawin.lau)
@course     CSD1401 Software Engineering Project 1
@section    a
@tutorial   Final Project
@date       18/11/2023
@brief      This file contains the definitions of int main(void)
*//*_________________________________________________________________________*/
#include "cprocessing.h"
#include "MainMenu.h"
#include "Game.h"
#include "SplashScreen.h"

int main(void)
{	
	CP_System_SetWindowSize(1280, 720);
	//CP_Engine_SetNextGameState(MainMenuInit, MainMenuUpdate, MainMenuExit);
	//CP_Engine_SetNextGameState(GameInit, GameUpdate, GameExit);
	CP_Engine_SetNextGameState(SplashScreenInit, SplashScreenUpdate, SplashScreenExit);
	CP_Engine_Run();
	return 0;
}
