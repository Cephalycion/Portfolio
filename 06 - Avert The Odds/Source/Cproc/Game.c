/*!
@copyright	DIGIPEN INSTITUTE OF TECHNOLOGY SINGAPORE
@file       Game.c
@author     Chan Kuan Fu Ryan (c.kuanfuryan), 
			Marc Alviz Evangelista(marcalviz.e), 
			Chng Kai Rong Leonard(k.chng), 
			Gavin Ang Jun Liang(ang.g),
			Lau Jia Win(jiawin.lau)
@course     CSD1401 Software Engineering Project 1
@section    a
@tutorial   Final Project
@date       18/11/2023
@brief      This file contains the definitions of functions which execute game
			logic, as well as render UI and game objects to screen.
*//*_________________________________________________________________________*/

#include "Game.h"
#include "ReadFile.h"
#include "MainMenu.h"

void GameUpdateUIRes(int res)
{
	switch (res)
	{
	case RES_720:
	{
		for (int i = 0; i < sizeof(levelUpOptionButtons) / sizeof(levelUpOptionButtons[0]); ++i)
		{
			levelUpOptionButtons[i].btnPos = levelUpOptionButtons[i].btnPosRes720;
			levelUpOptionButtons[i].btnScale = levelUpOptionButtons[i].btnScaleRes720;
		}
		for (int i = 0; i < sizeof(pauseMenuButtons) / sizeof(pauseMenuButtons[0]); ++i)
		{
			pauseMenuButtons[i].btnPos = pauseMenuButtons[i].btnPosRes720;
			pauseMenuButtons[i].btnScale = pauseMenuButtons[i].btnScaleRes720;
		}
		for (int i = 0; i < sizeof(endScreenButtons) / sizeof(endScreenButtons[0]); ++i)
		{
			endScreenButtons[i].btnPos = endScreenButtons[i].btnPosRes720;
			endScreenButtons[i].btnScale = endScreenButtons[i].btnScaleRes720;
		}
		oribitersIcon.btnPos = oribitersIcon.btnPosRes720;
		oribitersIcon.btnScale = oribitersIcon.btnScaleRes720;
		electroIcon.btnPos = oribitersIcon.btnPosRes720;
		electroIcon.btnScale = oribitersIcon.btnScaleRes720;
		plasmaIcon.btnPos = oribitersIcon.btnPosRes720;
		plasmaIcon.btnScale = oribitersIcon.btnScaleRes720;
		levelUpBackdrop.btnPos = levelUpBackdrop.btnPosRes720;
		levelUpBackdrop.btnScale = levelUpBackdrop.btnScaleRes720;
		pauseBackdrop.btnPos = pauseBackdrop.btnPosRes720;
		pauseBackdrop.btnScale = pauseBackdrop.btnScaleRes720;
		pauseAudioBarActiveIcon.btnPos = pauseAudioBarActiveIcon.btnPosRes720;
		pauseAudioBarActiveIcon.btnScale = pauseAudioBarActiveIcon.btnScaleRes720;
		pauseAudioBarInactiveIcon.btnPos = pauseAudioBarInactiveIcon.btnPosRes720;
		pauseAudioBarInactiveIcon.btnScale = pauseAudioBarInactiveIcon.btnScaleRes720;
		endBackdrop.btnPos = endBackdrop.btnPosRes720;
		endBackdrop.btnScale = endBackdrop.btnScaleRes720;
		playerHealthIcon.btnPos = playerHealthIcon.btnPosRes720;
		playerHealthIcon.btnScale = playerHealthIcon.btnScaleRes720;

		break;
	}
	case RES_1080:
	{
		for (int i = 0; i < sizeof(levelUpOptionButtons) / sizeof(levelUpOptionButtons[0]); ++i)
		{
			levelUpOptionButtons[i].btnPos = levelUpOptionButtons[i].btnPosRes1080;
			levelUpOptionButtons[i].btnScale = levelUpOptionButtons[i].btnScaleRes1080;
		}
		for (int i = 0; i < sizeof(pauseMenuButtons) / sizeof(pauseMenuButtons[0]); ++i)
		{
			pauseMenuButtons[i].btnPos = pauseMenuButtons[i].btnPosRes1080;
			pauseMenuButtons[i].btnScale = pauseMenuButtons[i].btnScaleRes1080;
		}
		for (int i = 0; i < sizeof(endScreenButtons) / sizeof(endScreenButtons[0]); ++i)
		{
			endScreenButtons[i].btnPos = endScreenButtons[i].btnPosRes1080;
			endScreenButtons[i].btnScale = endScreenButtons[i].btnScaleRes1080;
		}
		oribitersIcon.btnPos = oribitersIcon.btnPosRes1080;
		oribitersIcon.btnScale = oribitersIcon.btnScaleRes1080;
		electroIcon.btnPos = oribitersIcon.btnPosRes1080;
		electroIcon.btnScale = oribitersIcon.btnScaleRes1080;
		plasmaIcon.btnPos = oribitersIcon.btnPosRes1080;
		plasmaIcon.btnScale = oribitersIcon.btnScaleRes1080;
		levelUpBackdrop.btnPos = levelUpBackdrop.btnPosRes1080;
		levelUpBackdrop.btnScale = levelUpBackdrop.btnScaleRes1080;
		pauseBackdrop.btnPos = pauseBackdrop.btnPosRes1080;
		pauseBackdrop.btnScale = pauseBackdrop.btnScaleRes1080;
		pauseAudioBarActiveIcon.btnPos = pauseAudioBarActiveIcon.btnPosRes1080;
		pauseAudioBarActiveIcon.btnScale = pauseAudioBarActiveIcon.btnScaleRes1080;
		pauseAudioBarInactiveIcon.btnPos = pauseAudioBarInactiveIcon.btnPosRes1080;
		pauseAudioBarInactiveIcon.btnScale = pauseAudioBarInactiveIcon.btnScaleRes1080;
		endBackdrop.btnPos = endBackdrop.btnPosRes1080;
		endBackdrop.btnScale = endBackdrop.btnScaleRes1080;
		playerHealthIcon.btnPos = playerHealthIcon.btnPosRes1080;
		playerHealthIcon.btnScale = playerHealthIcon.btnScaleRes1080;
		break;
	}
	case RES_FULLSCREEN:
	{
		for (int i = 0; i < sizeof(levelUpOptionButtons) / sizeof(levelUpOptionButtons[0]); ++i)
		{
			levelUpOptionButtons[i].btnPos = levelUpOptionButtons[i].btnPosResFullscreen;
			levelUpOptionButtons[i].btnScale = levelUpOptionButtons[i].btnScaleResFullscreen;
		}
		for (int i = 0; i < sizeof(pauseMenuButtons) / sizeof(pauseMenuButtons[0]); ++i)
		{
			pauseMenuButtons[i].btnPos = pauseMenuButtons[i].btnPosResFullscreen;
			pauseMenuButtons[i].btnScale = pauseMenuButtons[i].btnScaleResFullscreen;
		}
		for (int i = 0; i < sizeof(endScreenButtons) / sizeof(endScreenButtons[0]); ++i)
		{
			endScreenButtons[i].btnPos = endScreenButtons[i].btnPosResFullscreen;
			endScreenButtons[i].btnScale = endScreenButtons[i].btnScaleResFullscreen;
		}
		oribitersIcon.btnPos = oribitersIcon.btnPosResFullscreen;
		oribitersIcon.btnScale = oribitersIcon.btnScaleResFullscreen;
		electroIcon.btnPos = oribitersIcon.btnPosResFullscreen;
		electroIcon.btnScale = oribitersIcon.btnScaleResFullscreen;
		plasmaIcon.btnPos = oribitersIcon.btnPosResFullscreen;
		plasmaIcon.btnScale = oribitersIcon.btnScaleResFullscreen;
		levelUpBackdrop.btnPos = levelUpBackdrop.btnPosResFullscreen;
		levelUpBackdrop.btnScale = levelUpBackdrop.btnScaleResFullscreen;
		pauseBackdrop.btnPos = pauseBackdrop.btnPosResFullscreen;
		pauseBackdrop.btnScale = pauseBackdrop.btnScaleResFullscreen;
		pauseAudioBarActiveIcon.btnPos = pauseAudioBarActiveIcon.btnPosResFullscreen;
		pauseAudioBarActiveIcon.btnScale = pauseAudioBarActiveIcon.btnScaleResFullscreen;
		pauseAudioBarInactiveIcon.btnPos = pauseAudioBarInactiveIcon.btnPosResFullscreen;
		pauseAudioBarInactiveIcon.btnScale = pauseAudioBarInactiveIcon.btnScaleResFullscreen;
		endBackdrop.btnPos = endBackdrop.btnPosResFullscreen;
		endBackdrop.btnScale = endBackdrop.btnScaleResFullscreen;
		playerHealthIcon.btnPos = playerHealthIcon.btnPosResFullscreen;
		playerHealthIcon.btnScale = playerHealthIcon.btnScaleResFullscreen;
		break;
	}
	default:
	{
		break;
	}
	}
}

void GameDrawEndScreen(void)
{
	ButtonDraw(endBackdrop, 0.f);
	for (int i = 0; i < sizeof(endScreenButtons) / sizeof(endScreenButtons[0]); ++i)
	{
		ButtonDraw(endScreenButtons[i], 0.f);
	}
	// Draw Score 
	CP_Settings_TextSize(60.0f);
	CP_Settings_Fill(white);
	CP_Font_DrawText(
		playerScoreDisplay,
		((float)CP_System_GetWindowWidth() / 8.f) * 3.075f,
		((float)CP_System_GetWindowHeight() / 10.f) * 3.5f 
	);
}

void GameDrawPauseMenuUI(void)
{
	ButtonDraw(pauseBackdrop, 0.f);
	for (int i = 0; i < sizeof(pauseMenuButtons) / sizeof(pauseMenuButtons[0]); ++i)
	{
		ButtonDraw(pauseMenuButtons[i], 0.f);
	}
	// Draw the audio bars
	for (float i = 0, j = (float)(CP_System_GetWindowWidth() / 2.f) - ((float)CP_Image_GetWidth(pauseAudioBarActiveIcon.btnImage) * pauseAudioBarActiveIcon.btnScale.x) * 3.35f
		; i < 10.f; ++i, j += ((float)CP_Image_GetWidth(pauseAudioBarActiveIcon.btnImage) * pauseAudioBarActiveIcon.btnScale.x) * 0.75f)
	{
		if (i < pauseAudioBarActiveCount)
		{
			CP_Image_Draw(pauseAudioBarActiveIcon.btnImage, j, pauseAudioBarInactiveIcon.btnPos.y, (float)CP_Image_GetWidth(pauseAudioBarInactiveIcon.btnImage) * pauseAudioBarInactiveIcon.btnScale.x,
				(float)CP_Image_GetHeight(pauseAudioBarInactiveIcon.btnImage) * pauseAudioBarInactiveIcon.btnScale.y, 255);
		}
		else
		{
			CP_Image_Draw(pauseAudioBarInactiveIcon.btnImage, j, pauseAudioBarInactiveIcon.btnPos.y, (float)CP_Image_GetWidth(pauseAudioBarInactiveIcon.btnImage) * pauseAudioBarInactiveIcon.btnScale.x,
				(float)CP_Image_GetHeight(pauseAudioBarInactiveIcon.btnImage) * pauseAudioBarInactiveIcon.btnScale.y, 255);
		}
	}

}

void GameSpriteProgress(GameObject* object)
{
	object->spriteAnimTime += CP_System_GetDt();
	if (object->spriteAnimTime > object->spriteAnimTimeLimit)
	{
		object->spriteAnimTime = 0.0f;
		++object->spriteIndex;
	}
	if (object->spriteIndex >= object->spriteIndexLimit)
	{
		object->spriteIndex = 0;
	}
}

const char starting[3] = "Lv\0";
const char max[4] = "Max\0";

void GameGetWeaponLevel(WEAPON_TYPE weapon, char arr[])
{
	switch (weapon)
	{
	case PLASMA_BALL:
	{
		if (weaponInfo[PLASMA_BALL].level < maxWeaponLevel)
		{
			strcpy_s(arr, 3, starting);
			arr[2] = weaponInfo[PLASMA_BALL].level + '0';
			arr[3] = '\0';
		}
		else
		{
			strcpy_s(arr, 4, max);
		}

		break;
	}
	case ELECTRO_DOME:
	{
		if (weaponInfo[ELECTRO_DOME].level < maxWeaponLevel)
		{
			strcpy_s(arr, 3, starting);
			arr[2] = weaponInfo[ELECTRO_DOME].level + '0';
			arr[3] = '\0';
		}
		else
		{
			strcpy_s(arr, 4, max);
		}
		break;
	}
	case ORBITERS:
	{
		if (weaponInfo[ORBITERS].level < maxWeaponLevel)
		{
			strcpy_s(arr, 3, starting);
			arr[2] = weaponInfo[ORBITERS].level + '0';
			arr[3] = '\0';
		}
		else
		{
			strcpy_s(arr, 4, max);
		}
		break;
	}
	default:
		break;
	}
}

void GameDrawLevelUpUI(void)
{
	// draw backdrop
	ButtonDraw(levelUpBackdrop, 0.f);
	// draw the buttons

	if (weaponInfo[PLASMA_BALL].level != maxWeaponLevel)
	{
		ButtonDraw(levelUpOptionButtons[0], 0.f);
	}
	if (weaponInfo[ORBITERS].level != maxWeaponLevel)
	{
		ButtonDraw(levelUpOptionButtons[2], 0.f);
	}	
	if (weaponInfo[ELECTRO_DOME].level != maxWeaponLevel)
	{
		ButtonDraw(levelUpOptionButtons[1], 0.f);
	}
	// Draw the button info
	char weaponLv[10];
	switch (levelUpOption1Type)
	{
	case PLASMA_BALL:
	{
		GameGetWeaponLevel(PLASMA_BALL, weaponLv);
		if (weaponInfo[PLASMA_BALL].level == maxWeaponLevel)
		{
			CP_Image_Draw(levelUpMaxedOption.btnImage, levelUpOptionButtons[0].btnPos.x, levelUpOptionButtons[0].btnPos.y,
				CP_Image_GetWidth(levelUpMaxedOption.btnImage) * levelUpOptionButtons[0].btnScale.x,
				CP_Image_GetHeight(levelUpMaxedOption.btnImage) * levelUpOptionButtons[0].btnScale.y,
				255);

			CP_Image_Draw(plasmaIcon.btnImage, (buttonLevelUpOption1.btnPos.x * plasmaIcon.btnScale.x) - CP_Image_GetWidth(plasmaIcon.btnImage) * plasmaIcon.btnScale.x * 2.f,
				(buttonLevelUpOption1.btnPos.y * plasmaIcon.btnScale.y),
				CP_Image_GetWidth(plasmaIcon.btnImage) * plasmaIcon.btnScale.x, CP_Image_GetHeight(plasmaIcon.btnImage) * plasmaIcon.btnScale.y, 255);
			CP_Settings_TextSize(45.f * plasmaIcon.btnScale.y);
			CP_Settings_Fill(white);
			CP_Font_DrawText(
				"Plasma Ball",
				(buttonLevelUpOption1.btnPos.x * plasmaIcon.btnScale.x) - 1.25f * CP_Image_GetWidth(plasmaIcon.btnImage) * plasmaIcon.btnScale.x,
				(buttonLevelUpOption1.btnPos.y * plasmaIcon.btnScale.y) + (CP_Image_GetWidth(plasmaIcon.btnImage) * plasmaIcon.btnScale.y) / 10.f
			);
			CP_Font_DrawText(
				weaponLv,
				(buttonLevelUpOption1.btnPos.x * plasmaIcon.btnScale.x) + 1.75f * CP_Image_GetWidth(plasmaIcon.btnImage) * plasmaIcon.btnScale.x,
				(buttonLevelUpOption1.btnPos.y * plasmaIcon.btnScale.y) + (CP_Image_GetWidth(plasmaIcon.btnImage) * plasmaIcon.btnScale.y) / 10.f
			);
		}
		else
		{
			if (levelUpOptionButtons[0].btnHovering)
			{
				// Draw Weapon Icon -> Weapon Name -> Weapon Level
				CP_Image_Draw(plasmaIcon.btnImage, (buttonLevelUpOption1.btnPos.x * plasmaIcon.btnScale.x) - CP_Image_GetWidth(plasmaIcon.btnImage) * plasmaIcon.btnScale.x * 2.f,
					(buttonLevelUpOption1.btnPos.y * plasmaIcon.btnScale.y),
					CP_Image_GetWidth(plasmaIcon.btnImage) * plasmaIcon.btnScale.x, CP_Image_GetHeight(plasmaIcon.btnImage) * plasmaIcon.btnScale.y, 255);
				CP_Settings_TextSize(45.f * plasmaIcon.btnScale.y);
				CP_Settings_Fill(white);
				CP_Font_DrawText(
					"Plasma Ball",
					(buttonLevelUpOption1.btnPos.x * plasmaIcon.btnScale.x) - 1.25f * CP_Image_GetWidth(plasmaIcon.btnImage) * plasmaIcon.btnScale.x,
					(buttonLevelUpOption1.btnPos.y * plasmaIcon.btnScale.y) + (CP_Image_GetWidth(plasmaIcon.btnImage) * plasmaIcon.btnScale.y) / 10.f
				);
				CP_Font_DrawText(
					weaponLv,
					(buttonLevelUpOption1.btnPos.x * plasmaIcon.btnScale.x) + 1.75f * CP_Image_GetWidth(plasmaIcon.btnImage) * plasmaIcon.btnScale.x,
					(buttonLevelUpOption1.btnPos.y * plasmaIcon.btnScale.y) + (CP_Image_GetWidth(plasmaIcon.btnImage) * plasmaIcon.btnScale.y) / 10.f
				);
			}
			else
			{
				CP_Image_Draw(plasmaIcon.btnImage, (buttonLevelUpOption1.btnPos.x * plasmaIcon.btnScale.x) - CP_Image_GetWidth(plasmaIcon.btnImage) * plasmaIcon.btnScale.x * 2.f,
					(buttonLevelUpOption1.btnPos.y * plasmaIcon.btnScale.y) - (CP_Image_GetWidth(plasmaIcon.btnImage) * plasmaIcon.btnScale.y) / 20.f,
					CP_Image_GetWidth(plasmaIcon.btnImage) * plasmaIcon.btnScale.x, CP_Image_GetHeight(plasmaIcon.btnImage) * plasmaIcon.btnScale.y, 255);
				CP_Settings_TextSize(45.f * plasmaIcon.btnScale.y);
				CP_Settings_Fill(white);
				CP_Font_DrawText(
					"Plasma Ball",
					(buttonLevelUpOption1.btnPos.x * plasmaIcon.btnScale.x) - 1.25f * CP_Image_GetWidth(plasmaIcon.btnImage) * plasmaIcon.btnScale.x,
					buttonLevelUpOption1.btnPos.y * plasmaIcon.btnScale.y
				);
				CP_Font_DrawText(
					weaponLv,
					(buttonLevelUpOption1.btnPos.x * plasmaIcon.btnScale.x) + 1.75f * CP_Image_GetWidth(plasmaIcon.btnImage) * plasmaIcon.btnScale.x,
					(buttonLevelUpOption1.btnPos.y * plasmaIcon.btnScale.y)
				);
			}
		}

	}
	default:
		break;
	}
	switch (levelUpOption2Type)
	{
	case ELECTRO_DOME:
	{

		GameGetWeaponLevel(ELECTRO_DOME, weaponLv);
		if (weaponInfo[ELECTRO_DOME].level == maxWeaponLevel)
		{
			CP_Image_Draw(levelUpMaxedOption.btnImage, levelUpOptionButtons[1].btnPos.x, levelUpOptionButtons[1].btnPos.y,
				CP_Image_GetWidth(levelUpMaxedOption.btnImage) * levelUpOptionButtons[1].btnScale.x,
				CP_Image_GetHeight(levelUpMaxedOption.btnImage) * levelUpOptionButtons[1].btnScale.y,
				255);

			CP_Image_Draw(electroIcon.btnImage, (buttonLevelUpOption2.btnPos.x * electroIcon.btnScale.x) - CP_Image_GetWidth(electroIcon.btnImage) * plasmaIcon.btnScale.x * 2.f,
				(buttonLevelUpOption2.btnPos.y * electroIcon.btnScale.y),
				CP_Image_GetWidth(electroIcon.btnImage) * electroIcon.btnScale.x, CP_Image_GetHeight(electroIcon.btnImage) * electroIcon.btnScale.y, 255);
			CP_Settings_TextSize(45.f * electroIcon.btnScale.y);
			CP_Settings_Fill(white);
			CP_Font_DrawText(
				"Electro Dome",
				(buttonLevelUpOption2.btnPos.x * electroIcon.btnScale.x) - 1.25f * CP_Image_GetWidth(electroIcon.btnImage) * electroIcon.btnScale.x,
				(buttonLevelUpOption2.btnPos.y * electroIcon.btnScale.y) + (CP_Image_GetWidth(electroIcon.btnImage) * electroIcon.btnScale.y) / 10.f
			);
			CP_Font_DrawText(
				weaponLv,
				(buttonLevelUpOption2.btnPos.x * electroIcon.btnScale.x) + 1.75f * CP_Image_GetWidth(electroIcon.btnImage) * electroIcon.btnScale.x,
				(buttonLevelUpOption2.btnPos.y * electroIcon.btnScale.y) + (CP_Image_GetWidth(electroIcon.btnImage) * electroIcon.btnScale.y) / 10.f
			);
		}
		else
		{
			if (levelUpOptionButtons[1].btnHovering)
			{
				CP_Image_Draw(electroIcon.btnImage, (buttonLevelUpOption2.btnPos.x * electroIcon.btnScale.x) - CP_Image_GetWidth(electroIcon.btnImage) * electroIcon.btnScale.x * 2.f,
					buttonLevelUpOption2.btnPos.y * electroIcon.btnScale.y,
					CP_Image_GetWidth(electroIcon.btnImage) * electroIcon.btnScale.x, CP_Image_GetHeight(electroIcon.btnImage) * electroIcon.btnScale.y, 255);
				CP_Settings_TextSize(45.f * electroIcon.btnScale.y);
				CP_Settings_Fill(white);
				CP_Font_DrawText(
					"Electro Dome",
					(buttonLevelUpOption2.btnPos.x * electroIcon.btnScale.x) - 1.25f * CP_Image_GetWidth(electroIcon.btnImage) * electroIcon.btnScale.x,
					buttonLevelUpOption2.btnPos.y * electroIcon.btnScale.y + (CP_Image_GetWidth(electroIcon.btnImage) * electroIcon.btnScale.y) / 10.f
				);
				CP_Font_DrawText(
					weaponLv,
					(buttonLevelUpOption2.btnPos.x * electroIcon.btnScale.x) + 1.75f * CP_Image_GetWidth(electroIcon.btnImage) * electroIcon.btnScale.x,
					(buttonLevelUpOption2.btnPos.y * electroIcon.btnScale.y) + (CP_Image_GetWidth(electroIcon.btnImage) * electroIcon.btnScale.y) / 10.f
				);
			}
			else
			{
				CP_Image_Draw(electroIcon.btnImage, (buttonLevelUpOption2.btnPos.x * electroIcon.btnScale.x) - CP_Image_GetWidth(electroIcon.btnImage) * electroIcon.btnScale.x * 2.f,
					buttonLevelUpOption2.btnPos.y * electroIcon.btnScale.y - (CP_Image_GetWidth(electroIcon.btnImage) * electroIcon.btnScale.y) / 20.f,
					CP_Image_GetWidth(electroIcon.btnImage) * electroIcon.btnScale.x, CP_Image_GetHeight(electroIcon.btnImage) * electroIcon.btnScale.y, 255);
				CP_Settings_TextSize(45.f * electroIcon.btnScale.y);
				CP_Settings_Fill(white);
				CP_Font_DrawText(
					"Electro Dome",
					(buttonLevelUpOption2.btnPos.x * electroIcon.btnScale.x) - 1.25f * CP_Image_GetWidth(electroIcon.btnImage) * electroIcon.btnScale.x,
					buttonLevelUpOption2.btnPos.y * electroIcon.btnScale.y
				);
				CP_Font_DrawText(
					weaponLv,
					(buttonLevelUpOption2.btnPos.x * electroIcon.btnScale.x) + 1.75f * CP_Image_GetWidth(electroIcon.btnImage) * electroIcon.btnScale.x,
					(buttonLevelUpOption2.btnPos.y * electroIcon.btnScale.y)
				);
			}
		}
	}
	default:
		break;
	}
	switch (levelUpOption3Type)
	{
	case ORBITERS:
	{
		GameGetWeaponLevel(ORBITERS, weaponLv);
		if (weaponInfo[ORBITERS].level == maxWeaponLevel)
		{
			CP_Image_Draw(levelUpMaxedOption.btnImage, levelUpOptionButtons[2].btnPos.x, levelUpOptionButtons[2].btnPos.y,
				CP_Image_GetWidth(levelUpMaxedOption.btnImage)* levelUpOptionButtons[2].btnScale.x,
				CP_Image_GetHeight(levelUpMaxedOption.btnImage)* levelUpOptionButtons[2].btnScale.y,
				255);

			CP_Image_Draw(oribitersIcon.btnImage, (buttonLevelUpOption3.btnPos.x* oribitersIcon.btnScale.x) - CP_Image_GetWidth(oribitersIcon.btnImage) * plasmaIcon.btnScale.x * 2.f,
				(buttonLevelUpOption3.btnPos.y* oribitersIcon.btnScale.y),
				CP_Image_GetWidth(oribitersIcon.btnImage)* oribitersIcon.btnScale.x, CP_Image_GetHeight(oribitersIcon.btnImage)* oribitersIcon.btnScale.y, 255);
			CP_Settings_TextSize(45.f * oribitersIcon.btnScale.y);
			CP_Settings_Fill(white);
			CP_Font_DrawText(
				"Orbiters",
				(buttonLevelUpOption3.btnPos.x* oribitersIcon.btnScale.x) - 1.25f * CP_Image_GetWidth(oribitersIcon.btnImage) * oribitersIcon.btnScale.x,
				(buttonLevelUpOption3.btnPos.y* oribitersIcon.btnScale.y) + (CP_Image_GetWidth(oribitersIcon.btnImage) * oribitersIcon.btnScale.y) / 10.f
			);
			CP_Font_DrawText(
				weaponLv,
				(buttonLevelUpOption3.btnPos.x* oribitersIcon.btnScale.x) + 1.75f * CP_Image_GetWidth(oribitersIcon.btnImage) * oribitersIcon.btnScale.x,
				(buttonLevelUpOption3.btnPos.y* oribitersIcon.btnScale.y) + (CP_Image_GetWidth(oribitersIcon.btnImage) * oribitersIcon.btnScale.y) / 10.f
			);
		}
		else
		{
			if (levelUpOptionButtons[2].btnHovering)
			{
				CP_Image_Draw(oribitersIcon.btnImage, (buttonLevelUpOption3.btnPos.x * oribitersIcon.btnScale.x) - CP_Image_GetWidth(oribitersIcon.btnImage) * oribitersIcon.btnScale.x * 2.f,
					buttonLevelUpOption3.btnPos.y * oribitersIcon.btnScale.y,
					CP_Image_GetWidth(oribitersIcon.btnImage) * oribitersIcon.btnScale.x, CP_Image_GetHeight(oribitersIcon.btnImage) * oribitersIcon.btnScale.y, 255);
				CP_Settings_TextSize(45.f * oribitersIcon.btnScale.y);
				CP_Settings_Fill(white);
				CP_Font_DrawText(
					"Orbiters",
					(buttonLevelUpOption3.btnPos.x * oribitersIcon.btnScale.x) - 1.25f * CP_Image_GetWidth(oribitersIcon.btnImage) * oribitersIcon.btnScale.x,
					buttonLevelUpOption3.btnPos.y * oribitersIcon.btnScale.y + (CP_Image_GetWidth(oribitersIcon.btnImage) * oribitersIcon.btnScale.y) / 10.f
				);
				CP_Font_DrawText(
					weaponLv,
					(buttonLevelUpOption3.btnPos.x* oribitersIcon.btnScale.x) + 1.75f * CP_Image_GetWidth(oribitersIcon.btnImage) * oribitersIcon.btnScale.x,
					(buttonLevelUpOption3.btnPos.y* oribitersIcon.btnScale.y) + (CP_Image_GetWidth(oribitersIcon.btnImage) * oribitersIcon.btnScale.y) / 10.f
				);
			}
			else
			{
				CP_Image_Draw(oribitersIcon.btnImage, (buttonLevelUpOption3.btnPos.x * oribitersIcon.btnScale.x) - CP_Image_GetWidth(oribitersIcon.btnImage) * oribitersIcon.btnScale.x * 2.f,
					buttonLevelUpOption3.btnPos.y * oribitersIcon.btnScale.y - (CP_Image_GetWidth(oribitersIcon.btnImage) * oribitersIcon.btnScale.y) / 20.f,
					CP_Image_GetWidth(oribitersIcon.btnImage) * oribitersIcon.btnScale.x, CP_Image_GetHeight(oribitersIcon.btnImage) * oribitersIcon.btnScale.y, 255);
				CP_Settings_TextSize(45.f * oribitersIcon.btnScale.y);
				CP_Settings_Fill(white);
				CP_Font_DrawText(
					"Orbiters",
					(buttonLevelUpOption3.btnPos.x * oribitersIcon.btnScale.x) - 1.25f * CP_Image_GetWidth(oribitersIcon.btnImage) * oribitersIcon.btnScale.x,
					buttonLevelUpOption3.btnPos.y * oribitersIcon.btnScale.y
				);
				CP_Font_DrawText(
					weaponLv,
					(buttonLevelUpOption3.btnPos.x* oribitersIcon.btnScale.x) + 1.75f * CP_Image_GetWidth(oribitersIcon.btnImage) * oribitersIcon.btnScale.x,
					(buttonLevelUpOption3.btnPos.y* oribitersIcon.btnScale.y)
				);
			}
		}
	}
	default:
		break;
	}
}

void GameSetupLevelUpOptions(void)
{
	levelUpOption1Type = PLASMA_BALL;
	levelUpOption2Type = ELECTRO_DOME;
	levelUpOption3Type = ORBITERS;
}

void GameCheckButtonClicks(void)
{
	if (gameLevelUp)
	{
		if (CP_Input_MouseClicked())
		{
			for (int i = 0; i < sizeof(levelUpOptionButtons) / sizeof(levelUpOptionButtons[0]); ++i)
			{
				if (levelUpOptionButtons[i].btnInteractable && (ButtonCheckClick(&levelUpOptionButtons[i], CP_Vector_Set(CP_Input_GetMouseWorldX(), CP_Input_GetMouseWorldY()))))
				{
					switch (i)
					{
					case 0: 
					{
						switch (levelUpOption1Type)
						{
							case PLASMA_BALL:
							{
								if (weaponInfo[PLASMA_BALL].level < maxWeaponLevel)
								{
									++weaponInfo[PLASMA_BALL].level;
									msInterval = bfInterval;
									msTimer = bfTimer + (bfInterval * 0.5f);
									if (weaponInfo[PLASMA_BALL].level == maxWeaponLevel)
									{
										levelUpOptionButtons[0].btnInteractable = false;
										levelUpOptionButtons[0].btnHovering = true;
									}
								}
								break;
							}
							case ELECTRO_DOME:
							{
								if (weaponInfo[ELECTRO_DOME].level < maxWeaponLevel)
								{
									++weaponInfo[ELECTRO_DOME].level;
									if (weaponInfo[ELECTRO_DOME].level == maxWeaponLevel)
									{
										levelUpOptionButtons[1].btnInteractable = false;
										levelUpOptionButtons[1].btnHovering = true;
									}
								}
								break;
							}
							case ORBITERS:
							{
								if (weaponInfo[ORBITERS].level < maxWeaponLevel)
								{
									++weaponInfo[ORBITERS].level;
									cirTimer = bfTimer + (bfInterval * 0.5f);
									if (weaponInfo[ORBITERS].level == maxWeaponLevel)
									{
										levelUpOptionButtons[2].btnInteractable = false;
										levelUpOptionButtons[2].btnHovering = true;
									}
								}
								break;
							}
							default:
							{
								break;
							}
						}
						break;
					}
					case 1:
					{
						switch (levelUpOption2Type)
						{
							case PLASMA_BALL:
							{
								if (weaponInfo[PLASMA_BALL].level < maxWeaponLevel)
								{
									++weaponInfo[PLASMA_BALL].level;
									msInterval = bfInterval;
									msTimer = bfTimer + (bfInterval * 0.5f);
									if (weaponInfo[PLASMA_BALL].level == maxWeaponLevel)
									{
										levelUpOptionButtons[0].btnInteractable = false;
										levelUpOptionButtons[0].btnHovering = true;
									}
								}
								break;
							}
							case ELECTRO_DOME:
							{
								if (weaponInfo[ELECTRO_DOME].level < maxWeaponLevel)
								{
									++weaponInfo[ELECTRO_DOME].level;
									if (weaponInfo[ELECTRO_DOME].level == maxWeaponLevel)
									{
										levelUpOptionButtons[1].btnInteractable = false;
										levelUpOptionButtons[1].btnHovering = true;
									}
								}
								break;
							}
							case ORBITERS:
							{
								if (weaponInfo[ORBITERS].level < maxWeaponLevel)
								{
									++weaponInfo[ORBITERS].level;
									cirTimer = bfTimer + (bfInterval * 0.5f);
									if (weaponInfo[ORBITERS].level == maxWeaponLevel)
									{
										levelUpOptionButtons[2].btnInteractable = false;
										levelUpOptionButtons[2].btnHovering = true;
									}
								}
								break;
							}
							default:
							{
								break;
							}
						}
						break;
					}
					case 2:
					{
						switch (levelUpOption3Type)
						{
							case PLASMA_BALL:
							{
								if (weaponInfo[PLASMA_BALL].level < maxWeaponLevel)
								{
									++weaponInfo[PLASMA_BALL].level;
									msInterval = bfInterval;
									msTimer = bfTimer + (bfInterval * 0.5f);
									if (weaponInfo[PLASMA_BALL].level == maxWeaponLevel)
									{
										levelUpOptionButtons[0].btnInteractable = false;
										levelUpOptionButtons[0].btnHovering = true;
									}

								}
								break;
							}
							case ELECTRO_DOME:
							{
								if (weaponInfo[ELECTRO_DOME].level < maxWeaponLevel)
								{
									++weaponInfo[ELECTRO_DOME].level;
									if (weaponInfo[ELECTRO_DOME].level == maxWeaponLevel)
									{
										levelUpOptionButtons[1].btnInteractable = false;
										levelUpOptionButtons[1].btnHovering = true;
									}
								}
								break;
							}
							case ORBITERS:
							{
								if (weaponInfo[ORBITERS].level < maxWeaponLevel)
								{
									++weaponInfo[ORBITERS].level;
									cirTimer = bfTimer + (bfInterval * 0.5f);
									if (weaponInfo[ORBITERS].level == maxWeaponLevel)
									{
										levelUpOptionButtons[2].btnInteractable = false;
										levelUpOptionButtons[2].btnHovering = true;
									}
								}
								break;
							}
							default:
							{
								break;
							}
						}
						break;
					}
					default:
					{
						break;
					}
					}
					weaponMenuOpen = 0;
					AudioExitMenu();
					AudioMenuAnyButtonClicked();
				}
			}
		}

		for (int i = 0; i < sizeof(levelUpOptionButtons) / sizeof(levelUpOptionButtons[0]); ++i)
		{
			if (levelUpOptionButtons[i].btnInteractable)
			{
				if (ButtonCheckHover(&levelUpOptionButtons[i], CP_Vector_Set(CP_Input_GetMouseWorldX(), CP_Input_GetMouseWorldY())))
				{
					levelUpOptionButtons[i].btnHovering = true;
				}
				else
				{
					levelUpOptionButtons[i].btnHovering = false;
				}
			}
		}
	}
	if (gamePause)
	{
		if (CP_Input_MouseClicked())
		{
			bool breakOut = false;
			for (int i = 0; i < sizeof(pauseMenuButtons) / sizeof(pauseMenuButtons[0]); ++i)
			{
				if (breakOut)
				{
					break;
				}
				if (ButtonCheckClick(&pauseMenuButtons[i], CP_Vector_Set(CP_Input_GetMouseWorldX(), CP_Input_GetMouseWorldY())))
				{
					switch (i)
					{
					case 0:
					{
						if (pauseAudioBarActiveCount > 0)
						{
							--pauseAudioBarActiveCount;
							AudioDecreaseVolume(AUDIO_GROUP_MASTER);
							WriteAudioTextFile(AudioGetVolume(AUDIO_GROUP_MASTER));
							breakOut = true;
						}
						break;
					}
					case 1:
					{
						if (pauseAudioBarActiveCount < 10)
						{
							++pauseAudioBarActiveCount;
							AudioIncreaseVolume(AUDIO_GROUP_MASTER);
							WriteAudioTextFile(AudioGetVolume(AUDIO_GROUP_MASTER));
							breakOut = true;
						}
						break;
					}
					case 2: // 720p
					{
						if (GetDisplay() != RES_720)
						{
							SetDisplay(RES_720);
							GameUpdateUIRes(GetDisplay());

							CP_System_SetWindowSize(1280, 720);
							breakOut = true;
						}
						break;
					}
					case 3: // 1080p
					{
						if (GetDisplay() != RES_1080)
						{
							SetDisplay(RES_1080);
							GameUpdateUIRes(GetDisplay());

							CP_System_SetWindowSize(1920, 1080);
							breakOut = true;
						}
						break;
					}
					case 4: // fullscreen
					{
						if (GetDisplay() != RES_FULLSCREEN)
						{
							SetDisplay(RES_FULLSCREEN);
							GameUpdateUIRes(GetDisplay());

							CP_System_Fullscreen();
							breakOut = true;
						}
						break;
					}
					case 5:
					{
						isQuittingToMainMenu = true;
						SetGTM(true);
						//CP_Engine_SetNextGameState(MainMenuInit, MainMenuUpdate, MainMenuExit);
						// go back to main menu
						// draw the are you sure thing with yes and no
						breakOut = true;
						break;
					}
					case 6:
					{
						gamePause = false;
						AudioExitMenu();
						breakOut = true;
						break;
					}
					default:
					{
						break;
					}
					}
					AudioMenuAnyButtonClicked();						
				}
			}
		}
		for (int i = 0; i < sizeof(pauseMenuButtons) / sizeof(pauseMenuButtons[0]); ++i)
		{

			if (pauseMenuButtons[i].btnInteractable)
			{
				if (ButtonCheckHover(&pauseMenuButtons[i], CP_Vector_Set(CP_Input_GetMouseWorldX(), CP_Input_GetMouseWorldY())))
				{
					pauseMenuButtons[i].btnHovering = true;

				}
				else
				{
					pauseMenuButtons[i].btnHovering = false;
				}
			}
		}
	}
	if (gameEnd)
	{
		if (CP_Input_MouseClicked())
		{
			bool breakOut = false;
			for (int i = 0; i < sizeof(endScreenButtons) / sizeof(endScreenButtons[0]); ++i)
			{
				if (breakOut)
					break;
				if (ButtonCheckClick(&endScreenButtons[i], CP_Vector_Set(CP_Input_GetMouseWorldX(), CP_Input_GetMouseWorldY())))
				{
					switch (i)
					{
					case 0: // Quit to main Menu
					{
						isQuittingToMainMenu = true;
						SetGTM(true);
						//CP_Engine_SetNextGameState(MainMenuInit, MainMenuUpdate, MainMenuExit);
						breakOut = true;
						break;
					}
					case 1: // Restart Run
					{
						isQuittingToMainMenu = true;
						SetGTG(true);
						//CP_Engine_SetNextGameStateForced(GameInit, GameUpdate, GameExit);
						breakOut = true;
						break;
					}
					default:
					{
						break;
					}
					}
				}
				AudioMenuAnyButtonClicked();
			}
		}
		for (int i = 0; i < sizeof(endScreenButtons) / sizeof(endScreenButtons[0]); ++i)
		{
			if (endScreenButtons[i].btnInteractable)
			{
				if (ButtonCheckHover(&endScreenButtons[i], CP_Vector_Set(CP_Input_GetMouseWorldX(), CP_Input_GetMouseWorldY())))
				{
					endScreenButtons[i].btnHovering = true;
				}
				else
				{
					endScreenButtons[i].btnHovering = false;
				}
			}
		}
	}
}

// Decrements GameObject->healthCurrent and returns the final dealt value
float GameDealDamage(GameObject* object, float damage)
{
	if (!object->active)
		return -1.0f;

	float damageDealt = damage;
	if (object->healthCurrent > 0.0f)
	{
		if (object == player)
		{
			object->healthCurrent -= damage;
			GameUpdateHealthDisplay();
		}
		else
		{
			damageDealt = damage * damageModifer;
			object->healthCurrent -= damageDealt;
		}
	}
	return damageDealt;
}

void GameInitButtons(void)
{
	// Level Up UI
	ButtonInit(&levelUpBackdrop, CP_Vector_Set(1920.f / 2.f, 1080.f / 2.f), CP_Vector_Set(1.f, 1.f), 255,
		"LevelUpBackdrop.png", false, "", false, "", false);
	ButtonInit(&buttonLevelUpOption1, CP_Vector_Set(1920.f / 2.f, (1080.f / 8) * 3.f), CP_Vector_Set(1.f, 1.f), 255,
		"LevelUpOptionButton.png", true, "", false, "", false);
	ButtonInit(&buttonLevelUpOption2, CP_Vector_Set(1920.f / 2.f, (1080.f / 8) * 4.5f), CP_Vector_Set(1.f, 1.f), 255,
		"LevelUpOptionButton.png", true, "", false, "", false);
	ButtonInit(&buttonLevelUpOption3, CP_Vector_Set(1920.f / 2.f, (1080.f / 8) * 6.f), CP_Vector_Set(1.f, 1.f), 255,
		"LevelUpOptionButton.png", true, "", false, "", false);
	ButtonInit(&levelUpMaxedOption, CP_Vector_Set(1.f, 1.f), CP_Vector_Set(1.f, 1.f), 255,
		"maxed_LevelUpOptionButton.png", false, "", false, "", false);

	ButtonInit(&oribitersIcon, CP_Vector_Set(1.f, 1.f), CP_Vector_Set(1.f, 1.f), 255,
		"WeaponIcon_Circle.png", false, "", false, "", false);
	ButtonInit(&plasmaIcon, CP_Vector_Set(1.f, 1.f), CP_Vector_Set(1.f, 1.f), 255,
		"WeaponIcon_Plasma.png", false, "", false, "", false);
	ButtonInit(&electroIcon, CP_Vector_Set(1.f, 1.f), CP_Vector_Set(1.f, 1.f), 255,
		"WeaponIcon_Electro.png", false, "", false, "", false);

	levelUpOptionButtons[0] = buttonLevelUpOption1;
	levelUpOptionButtons[1] = buttonLevelUpOption2;
	levelUpOptionButtons[2] = buttonLevelUpOption3;

	// Pause Menu UI
	ButtonInit(&pauseBackdrop, CP_Vector_Set(1920.f / 2.f, 1080.f / 2.f), CP_Vector_Set(1.f, 1.f), 255,
		"PauseMenuBackdrop.png", false, "", false, "", false);
	ButtonInit(&pauseAudioBarInactiveIcon, CP_Vector_Set(1.f, (1080.f / 10.f) * 4.5f), CP_Vector_Set(1.f, 1.f), 255,
		"PauseAudioBarInactive.png", false, "", false, "", false);
	ButtonInit(&pauseAudioBarActiveIcon, pauseAudioBarInactiveIcon.btnPos, CP_Vector_Set(1.f, 1.f), 255,
		"PauseAudioBarActive.png", false, "", false, "", false);

	ButtonInit(&pauseAudioDecButton, CP_Vector_Set((1920.f / 2.f) - (CP_Image_GetWidth(pauseAudioBarActiveIcon.btnImage) * 5.5f), (1080.f / 10) * 4.5f), CP_Vector_Set(1.f, 1.f),
		255, "PauseAudioDec.png", true, "", false, "", false);
	ButtonInit(&pauseAudioIncButton, CP_Vector_Set((1920.f / 2.f) + (CP_Image_GetWidth(pauseAudioBarActiveIcon.btnImage) * 5.5f), (1080.f / 10) * 4.5f), CP_Vector_Set(1.f, 1.f),
		255, "PauseAudioInc.png", true, "", false, "", false);

	// ButtonInit(&pause720ResolutionButton, CP_Vector_Set((1920.f / 8.f) * 3.f, (1080.f / 10.f) * 5.f), CP_Vector_Set(1.f, 1.f), 255,
	// 	"Pause720Reso.png", true, "", false, "", false);
	// ButtonInit(&pause1080ResolutionButton, CP_Vector_Set((1920.f / 2.f), (1080.f / 10.f) * 5.f), CP_Vector_Set(1.f, 1.f), 255,
	// 	"Pause1080Reso.png", true, "", false, "", false);
	// ButtonInit(&pauseFullResolutionButton, CP_Vector_Set((1920.f / 8.f) * 5.f, (1080.f / 10.f) * 5.f), CP_Vector_Set(1.f, 1.f), 255,
	// 	"PauseFullReso.png", true, "", false, "", false);

	ButtonInit(&pauseExitButton, CP_Vector_Set((1920.f / 8.f) * 3.f, (1080.f / 10.f) * 6.5f), CP_Vector_Set(1.f, 1.f), 255,
		"PauseMenuExit.png", true, "", false, "", false);
	ButtonInit(&pauseResumeButton, CP_Vector_Set((1920.f / 8.f) * 5.f, (1080.f / 10.f) * 6.5f), CP_Vector_Set(1.f, 1.f), 255,
		"PauseMenuResume.png", true, "", false, "", false);

	pauseMenuButtons[0] = pauseAudioDecButton;
	pauseMenuButtons[1] = pauseAudioIncButton;
	pauseMenuButtons[2] = pause720ResolutionButton;
	pauseMenuButtons[3] = pause1080ResolutionButton;
	pauseMenuButtons[4] = pauseFullResolutionButton;
	pauseMenuButtons[5] = pauseExitButton;
	pauseMenuButtons[6] = pauseResumeButton;

	// End Screen UI

	ButtonInit(&endBackdrop, CP_Vector_Set(1920.f / 2.f, 1080.f / 2.f), CP_Vector_Set(1.f, 1.f), 255,
		"EndScreenBackdrop.png", false, "", false, "", false);

	ButtonInit(&endQuitToMainButton, CP_Vector_Set((1920.f / 8.f) * 3.4f, (1080.f / 10.f) * 5.5f), CP_Vector_Set(1.f, 1.f), 255,
		"EndScreenExit.png", true, "", false, "", false);
	ButtonInit(&endRestartButton, CP_Vector_Set((1920.f / 8.f) * 4.6f, (1080.f / 10.f) * 5.5f), CP_Vector_Set(1.f, 1.f), 255,
		"EndScreenRestart.png", true, "", false, "", false);


	endScreenButtons[0] = endQuitToMainButton;
	endScreenButtons[1] = endRestartButton;

	// Other UI
	ButtonInit(&playerHealthIcon, CP_Vector_Set(85.f,75.f), CP_Vector_Set(1.f, 1.f), 255, "PlayerHeart.png", false, "", false, "", false);

	for (int j = RES_720; j < DISPLAY_END; ++j)
	{
		for (int i = 0; i < sizeof(levelUpOptionButtons) / sizeof(levelUpOptionButtons[0]); ++i)
		{
			ButtonSetRes(&levelUpOptionButtons[i], j, CP_Vector_Set(1920.f, 1080.f));
		}
		for (int i = 0; i < sizeof(pauseMenuButtons) / sizeof(pauseMenuButtons[0]); ++i)
		{
			ButtonSetRes(&pauseMenuButtons[i], j, CP_Vector_Set(1920.f, 1080.f));
		}
		for (int i = 0; i < sizeof(endScreenButtons) / sizeof(endScreenButtons[0]); ++i)
		{
			ButtonSetRes(&endScreenButtons[i], j, CP_Vector_Set(1920.f, 1080.f));
		}
		ButtonSetRes(&pauseAudioBarInactiveIcon, j, CP_Vector_Set(1920.f, 1080.f));
		ButtonSetRes(&pauseAudioBarActiveIcon, j, CP_Vector_Set(1920.f, 1080.f));
		ButtonSetRes(&pauseBackdrop, j, CP_Vector_Set(1920.f, 1080.f));
		ButtonSetRes(&levelUpBackdrop, j, CP_Vector_Set(1920.f, 1080.f));
		ButtonSetRes(&endBackdrop, j, CP_Vector_Set(1920.f, 1080.f));
		ButtonSetRes(&oribitersIcon, j, CP_Vector_Set(1920.f, 1080.f));
		ButtonSetRes(&electroIcon, j, CP_Vector_Set(1920.f, 1080.f));
		ButtonSetRes(&plasmaIcon, j, CP_Vector_Set(1920.f, 1080.f));
		ButtonSetRes(&playerHealthIcon, j, CP_Vector_Set(1920.f, 1080.f));
	}
	GameUpdateUIRes(GetDisplay());

	GameSetupLevelUpOptions();
}

void GameClearObject(GameObject* object)
{
	/*if (object->type == OBJ_ALLY_PROJECTILE || object->type == OBJ_ALLY_MULTI_SHOT)
	{
		if (object->lifetime <= 0.0f)
		{
			printf("Projectile died due to lifetime\n");
		}
		else
		{
			printf("Projectile collided at (%f, %f)\n", object->position.x, object->position.y);
		}
	}*/
	object->type			= OBJ_CLEAR;
	object->positionBase	= CP_Vector_Zero();
	object->position		= CP_Vector_Zero();
	object->velocity		= CP_Vector_Zero();
	object->active			= 0;
	object->healthMaximum	= 0;
	object->healthCurrent	= 0;
	object->speed			= 0;
	object->radius			= 0;
	object->lifetime		= 0;
	object->tweening		= 0;
}

void GameClearTween(Tween* tween)
{
	tween->active = 0;
	tween->objectIndex = 0;
	tween->direction = CP_Vector_Zero();
	tween->speed = 0.0f;
	tween->decay = 0.0f;
}

void GameInit(void)
{
	srand((unsigned int)time(0));

	// Clear all objects at the start of the game
	for (int i = sizeof(objects) / sizeof(objects[0]) - 1; i >= 0; --i)
	{
		GameClearObject(&objects[i]);
	}

	// Clear all tweens at the start of the game
	for (int i = sizeof(tweens) / sizeof(tweens[0]) - 1; i >= 0; --i)
	{
		GameClearTween(&tweens[i]);
	}

	// Initialise game loop variables
	waveTimer = 0.0f;
	waveInterval = 5.0f;
	enemiesPerWave = 6;
	waveCounter = 0;
	bossSpawnCounter = 0;
	gameCenter = CP_Vector_Set(CP_System_GetWindowWidth() * 0.5f, CP_System_GetWindowHeight() * 0.5f);
	SpriteInit(&playerSpriteWalking, "playerCharacter.png", false, 1.f, 255);
	SpriteInit(&playerSpriteStanding, "playerIdle.png", false, 1.f, 255);
	SpriteInitDirectionless(&enemySprite, "enemy.png", false, 0.5f, 255);
	SpriteInitDirectionless(&enemyEliteSprite, "enemyElite.png", false, 0.5f, 255);
	SpriteInitDirectionless(&enemyBossSprite, "enemyBoss.png", false, 0.5f, 255);
	
	// Game UI Flags
	gamePause = false;
	gameEnd = false;

	// Cursor Sprites
	SpriteInit(&cursorAim, "AimCursor.png", false, 1.f, 255);
	SpriteInit(&cursorUI, "PlayerCursor.png", false, 1.f, 255);

	// Fade 
	SpriteBetterInit(&transitionClose720Game, "transition_closing720.png", 1.f, 255, 4, 4, 14);
	SpriteBetterInit(&transitionClose1080Game, "transition_closing1080.png", 1.f, 255, 4, 4, 14);
	SpriteBetterInit(&transitionOpen720Game, "transition_opening720.png", 1.f, 255, 3, 3, 8);
	SpriteBetterInit(&transitionOpen1080Game, "transition_opening1080.png", 1.f, 255, 3, 3, 8);

	// EXP SYSTEM INIT
	gameLevelUp = false;
	playerLevel = 0;
	playerExp = 0;
	playerExpLimit = 30;
	playerScore = 0;


	// Initialise colours
	white	= CP_Color_Create(255, 255, 255, 255);
	red		= CP_Color_Create(255, 0, 0, 255);
	yellow	= CP_Color_Create(255, 255, 0, 255);
	blue	= CP_Color_Create(0, 0, 255, 255);
	cyan	= CP_Color_Create(0, 255, 255, 255);
	purple	= CP_Color_Create(255, 0, 255, 255);
	orange	= CP_Color_Create(255, 165, 0, 255);
	green	= CP_Color_Create(0, 255, 0, 255);

	//read player stats first
	//Reading player data from text file
	ReadPlayerTextFile();

	// Initialise player
	if (player = GameGetObject(OBJ_PLAYER))
	{
		*player = CreateGameObject(
			/*	type			*/	OBJ_PLAYER,
			/*	positionBase	*/	CP_Vector_Zero(),
			/*	position		*/	gameCenter,
			/*	healthMaximum	*/	100.0f + (healthTempCounter * 20.0f),
			/*	speed			*/	150.0f + (speedTempCounter * 10.0f),
			/*	radius			*/	15.0f,
			/*	lifetime		*/	0.0f
		);
	}
	playerIsMoving = 0;
	playerIsFacingLeft = 0;
	damageModifer = 1.0f + (attackTempCounter * 0.2f);


	// Ryan's UI (MUST INIT AFTER PLAYER)
	GameUpdateScoreDisplay();
	GameUpdateHealthDisplay();

	// Weapons & Projectile
	for (int i = 0; i < WEAPONINFO_SIZE; ++i)
	{
		weaponInfo[i].level = 0;
	}
	weaponInfo[PLASMA_BALL].baseDamage = 6.0f;
	weaponInfo[PLASMA_BALL].baseSize = 15.0f;
	weaponInfo[PLASMA_BALL].image = CP_Image_Load("Assets/plasmaball.png");
	weaponInfo[ORBITERS].baseDamage = 2.0f;
	weaponInfo[ORBITERS].image = CP_Image_Load("Assets/orbiters.png");
	weaponInfo[ELECTRO_DOME].baseDamage = 12.0f;
	weaponInfo[ELECTRO_DOME].baseSize = 100.0f;
	weaponInfo[ELECTRO_DOME].image = CP_Image_Load("Assets/electrodome.png");
	maxWeaponLevel = 3;
	weaponMenuOpen = 0;

	// Projectiles
	// Basic Fire
	bfTimer			= 1.0f;		// current wave time to spawn basic fire Projectile
	bfInterval		= 1.0f;		// Interval between each spawn
	// Multi_Shot
	msTimer			= 0.0f;		// current wave time to spawn multi_shot Projectile
	msInterval		= 0.0f;		// Interval between each spawn
	// Circling
	cirTimer		= 0.0f;		// current wave time to spawn circling Projectile
	cirInterval		= 2.5f;		// Interval between each spawn
	cirHitInterval	= 0.5f;
	// ELECTRO_DOME
	aoeTimer		= 3.0f;		// current wave time to spawn ELECTRO_DOME
	aoeInterval		= 3.0f;		// Interval between each appearance
	aoeActive		= 0;		// if its not active, then spawn ELECTRO_DOME
	aoeAlpha		= 0.0f;
	aoe				= CP_Color_Create(0, 0, 255, (int)aoeAlpha);
	aoeAlphaChanger = 1.0f;		// change by how much

	enemyBulletCounter = 5;
	enemyFireTimer = 5.0f;
	enemyFireInterval = 5.0f;
	enemyBurstTimer = 0.0f;
	enemyBurstInterval = 0.2f;
	enemyBossBurst = false;

	cameraOffset = CP_Vector_Zero();
	
	// Map
	GenerateMap(player);

	// Obstacles
	LoadObstacle();
	for (int i = 0; i < MAX_OBSTACLE; ++i)
	{
		GameObject* obstacle;
		if (obstacle = GameGetObject(OBJ_OBSTACLE))
			GenerateObstacle(obstacle, gameCenter);
	}

	// Initialize ELECTRO_DOME 
	if (allyAOE = GameGetObject(OBJ_ALLY_AOE))
	{
		*allyAOE = CreateGameObject(
			/*	type			*/	OBJ_ALLY_AOE,
			/*	positionBase	*/	CP_Vector_Set(gameCenter.x - cameraOffset.x, gameCenter.y - cameraOffset.y),
			/*	position		*/	CP_Vector_Zero(),
			/*	healthMaximum	*/	0.0f,
			/*	speed			*/	0.0f,
			/*	radius			*/	weaponInfo[ELECTRO_DOME].baseSize,
			/*	lifetime		*/	0.0f
		);
	}

	// UI
	GameInitButtons();

	
	ParticleInitGame(&cameraOffset);
	//AudioInit() - this should be in main menu
	AudioPlayMusic(AUDIO_TYPE_SICKBEATS_MUSIC);
	
	

	// Set Pause menu's bars
	pauseAudioBarActiveCount = AudioGetVolume(AUDIO_GROUP_MASTER);

	// Check if quit exe (altf4) or exit to main menu (from pause/end game)
	isQuittingToMainMenu = false;
}

void GameSetTween(int objectIndex, CP_Vector direction, float speed, float decay)
{
	for (int i = 0; i < sizeof(tweens) / sizeof(tweens[0]); ++i)
	{
		if (tweens[i].active == 0)
		{
			tweens[i].active = 1;
			tweens[i].objectIndex = objectIndex;
			tweens[i].direction = direction;
			tweens[i].speed = speed;
			tweens[i].decay = decay;
			break;
		}
	}
}

void GameProjectileExplode(GameObject* projectile, bool isCollideObstacle)
{
	if (isCollideObstacle)
		GamePlayParticleAndSound(GE_PROJ_HIT, projectile, 0);
	//ParticleSpawn(PARTICLE_TYPE_EXPLOSION, projectile->position.x - cameraOffset.x, projectile->position.y - cameraOffset.y, PARTICLEDEF_LIFETIME, 0);
	//AudioPlaySpatial(AUDIO_TYPE_EXPLOSION_SFX, gameCenter.x, gameCenter.y, projectile->position.x, projectile->position.y);
	GameClearObject(projectile);
}

void GameUpdate(void)
{
	CP_Graphics_ClearBackground(CP_Color_Create(100, 100, 100, 255));
	AudioUpdate();
	DrawMap(cameraOffset);

	if (weaponMenuOpen /* || add other condition that will pause the game || condition 3*/)
	{
		gameLevelUp = true;
	}
	else
	{
		gameLevelUp = false;
	}

	if (!gameLevelUp && !gameEnd && !gamePause) // NOT PAUSE, RUN GAME NORMALLY
	{
		ParticleUpdate();

		// Tweens
		for (int i = 0; i < sizeof(tweens) / sizeof(tweens[0]); ++i)
		{
			if (tweens[i].active == 0)
				continue;

			if (objects[tweens[i].objectIndex].active == 0)
			{
				GameClearTween(&tweens[i]);
			}

			objects[tweens[i].objectIndex].velocity.x = tweens[i].direction.x * tweens[i].speed * CP_System_GetDt();
			objects[tweens[i].objectIndex].velocity.y = tweens[i].direction.y * tweens[i].speed * CP_System_GetDt();
			tweens[i].speed -= tweens[i].decay * CP_System_GetDt();

			if (tweens[i].speed <= 0.0f)
			{
				objects[tweens[i].objectIndex].tweening = 0;
				GameClearTween(&tweens[i]);
			}
		}

		// Player controller
		if (player != NULL && player->active)
		{
			// Check facing direction
			if (CP_Input_GetMouseX() < gameCenter.x)
				playerIsFacingLeft = 1;
			else
				playerIsFacingLeft = 0;

			// Reset velocity every frame
			player->velocity = CP_Vector_Zero();
			playerIsMoving = 0;

			// Input alters direction
			if (CP_Input_KeyDown(KEY_W) && cameraOffset.y + player->radius < MAP_BOUNDS_Y)
			{
				player->velocity.y = -1.0f;
			}
			if (CP_Input_KeyDown(KEY_A) && cameraOffset.x + player->radius < MAP_BOUNDS_X)
			{
				player->velocity.x = -1.0f;
			}
			if (CP_Input_KeyDown(KEY_S) && cameraOffset.y - player->radius > -MAP_BOUNDS_Y)
			{
				player->velocity.y = 1.0f;
			}
			if (CP_Input_KeyDown(KEY_D) && cameraOffset.x - player->radius > -MAP_BOUNDS_X)
			{
				player->velocity.x = 1.0f;
			}

			// Direction gives velocity for this frame
			CP_Vector moveVector = CP_Vector_Normalize(player->velocity);
			player->velocity.x = moveVector.x * player->speed * CP_System_GetDt();
			player->velocity.y = moveVector.y * player->speed * CP_System_GetDt();

			// Set playerIsMoving
			if (CP_Vector_Length(player->velocity))
				playerIsMoving = 1;

			// Basic shooting
			CreateBasicFireProjectile();

			// Auto Spawn Weapon's Projectile
			if (weaponInfo[PLASMA_BALL].level)
			{
				CreateMultiShotProjectile();
			}
			
			// ELECTRO_DOME
			if (aoeActive == 1)
			{
				if (aoeAlpha >= 255.0f)
					aoeAlphaChanger = -1.0f;
				else if (aoeAlpha <= 0.0f)
					aoeAlphaChanger = 1.0f;

				aoeAlpha += aoeAlphaChanger * 100.f * CP_System_GetDt();
				aoe = CP_Color_Create(0, 0, 255, (int)aoeAlpha);
			}
			else if (weaponInfo[ELECTRO_DOME].level > 0)
			{
				aoeActive = 1;
			}

			// Circling
			cirTimer -= CP_System_GetDt();
			if (weaponInfo[ORBITERS].level && cirTimer <= 0.0f)
			{
				CreateCirclingProjectile();
			}

			// Death condition check
			if (player->healthCurrent <= 0.0f && player->active)
			{
				gameEnd = true;
				player->active = 0;
				// printf("Game Over!\n");
				ReadPlayerCurrencyTextFile();
				WritePlayerCurrencyTextFile(playerScore);
			}
		}

		// Set the position before executing logic
		for (int i = 0; i < sizeof(objects) / sizeof(objects[0]); ++i)
		{
			// CAMERA OFFSET
			if (&objects[i] != player)
			{
				objects[i].position.x = objects[i].positionBase.x + cameraOffset.x;
				objects[i].position.y = objects[i].positionBase.y + cameraOffset.y;
			}
		}

		// GameObject loop
		for (int i = sizeof(objects) / sizeof(objects[0]) - 1; i >= 0; --i)
		{
			// If not active, don't execute logic
			if (!objects[i].active)
				continue;

			// Object behaviours
			switch (objects[i].type)
			{
			case OBJ_CLEAR:
			{
				break;
			}
			case OBJ_PLAYER:
			{
				// PLAYER TO OBSTACLE COLLISION
				for (int j = 0; j < sizeof(objects) / sizeof(objects[0]); ++j)
				{
					// If not active, don't execute logic
					if (!objects[j].active)
						continue;

					// Only consider obstacle types
					if (objects[j].type == OBJ_OBSTACLE)
					{
						// If collide
						if (AreCirclesIntersecting(&objects[i], &objects[j]) && IsMovingCloser(&objects[i], &objects[j]))
						{
							CP_Vector toObstacle = CP_Vector_Set(
								objects[j].position.x - objects[i].position.x,
								objects[j].position.y - objects[i].position.y
							);

							CP_Vector reactionVelocity = VectorProjection(objects[i].velocity, toObstacle);
							objects[i].velocity.x -= reactionVelocity.x;
							objects[i].velocity.y -= reactionVelocity.y;
						}
					}
				}
				break;
			}
			case OBJ_ALLY_PROJECTILE:
			{
				objects[i].positionBase.x += objects[i].velocity.x * CP_System_GetDt();
				objects[i].positionBase.y += objects[i].velocity.y * CP_System_GetDt();

				// LIFETIME
				objects[i].lifetime -= CP_System_GetDt();
				if (objects[i].lifetime <= 0.0f)
				{
					GameClearObject(&objects[i]);
				}

				// SECONDARY LOOP
				for (int j = 0; j < sizeof(objects) / sizeof(objects[0]); ++j)
				{
					// If not active, don't execute logic
					if (!objects[j].active)
						continue;

					// PROJECTILE TO ENEMY COLLISION
					if (objects[j].type == OBJ_ENEMY ||
						objects[j].type == OBJ_ENEMY_ELITE ||
						objects[j].type == OBJ_ENEMY_BOSS)
					{
						// DEAL DAMAGE TO ENEMY
						if (AreCirclesIntersecting(&objects[i], &objects[j]))
						{
							GameProjectileExplode(&objects[i], false);

							GamePlayParticleAndSound(GE_ENEMY_HURT, &objects[j], (int)GameDealDamage(&objects[j], 10.0f));
							/*ParticleSpawn(
								PARTICLE_TYPE_ENEMY_HIT,
								objects[j].position.x - cameraOffset.x,
								objects[j].position.y - cameraOffset.y,
								PARTICLEDEF_LIFETIME,
								(int)GameDealDamage(&objects[j], 10.0f)
							);*/
						}
					}

					// PROJECTILE TO OBSTACLE COLLISION
					if (objects[j].type == OBJ_OBSTACLE)
					{
						// IF COLLIDE
						if (AreCirclesIntersecting(&objects[i], &objects[j]))
						{
							GameProjectileExplode(&objects[i], true);
						}
					}
				}
				break;
			}
			case OBJ_ALLY_MULTI_SHOT:
			{
				objects[i].positionBase.x += objects[i].velocity.x * CP_System_GetDt();
				objects[i].positionBase.y += objects[i].velocity.y * CP_System_GetDt();

				// LIFETIME
				objects[i].lifetime -= CP_System_GetDt();
				if (objects[i].lifetime <= 0.0f)
				{
					GameClearObject(&objects[i]);
				}

				// SECONDARY LOOP
				for (int j = 0; j < sizeof(objects) / sizeof(objects[0]); ++j)
				{
					// If not active, don't execute logic
					if (!objects[j].active)
						continue;

					// PROJECTILE TO ENEMY COLLISION
					if (objects[j].type == OBJ_ENEMY ||
						objects[j].type == OBJ_ENEMY_ELITE ||
						objects[j].type == OBJ_ENEMY_BOSS)
					{
						// DEAL DAMAGE TO ENEMY
						if (AreCirclesIntersecting(&objects[i], &objects[j]))
						{
							GameProjectileExplode(&objects[i], false);

							GamePlayParticleAndSound(GE_ENEMY_HURT, &objects[j], (int)GameDealDamage(&objects[j], weaponInfo[PLASMA_BALL].baseDamage));
							/*ParticleSpawn(
								PARTICLE_TYPE_ENEMY_HIT,
								objects[j].position.x - cameraOffset.x,
								objects[j].position.y - cameraOffset.y,
								PARTICLEDEF_LIFETIME,
								(int)GameDealDamage(&objects[j], weaponInfo[PLASMA_BALL].baseDamage * 1.0f + ((float)(weaponInfo[PLASMA_BALL].level - 1.0f) * 0.5f))
							);*/
						}
					}

					// PROJECTILE TO OBSTACLE COLLISION
					if (objects[j].type == OBJ_OBSTACLE)
					{
						// IF COLLIDE
						if (AreCirclesIntersecting(&objects[i], &objects[j]))
						{
							GameProjectileExplode(&objects[i], true);
						}
					}
				}
				break;
			}
			case OBJ_ALLY_CIRCLING:
			{
				if (objects[i].range > 0.0f)
					objects[i].range -= 10.0f * CP_System_GetDt();
				else
					objects[i].range = 0.0f;
				objects[i].cirOffset = CP_Vector_MatrixMultiply(CP_Matrix_Rotate(90.0f * CP_System_GetDt()), objects[i].cirOffset);

				float speed = 1.5f * CP_System_GetDt();

				CP_Vector dir = CP_Vector_Normalize(objects[i].cirOffset);
				objects[i].cirOffset.x += dir.x * objects[i].range * speed;
				objects[i].cirOffset.y += dir.y * objects[i].range * speed;


				objects[i].positionBase.x = gameCenter.x + objects[i].cirOffset.x - cameraOffset.x;
				objects[i].positionBase.y = gameCenter.y + objects[i].cirOffset.y - cameraOffset.y;

				// LIFETIME
				objects[i].lifetime -= CP_System_GetDt();
				if (objects[i].lifetime <= 0.0f)
				{
					GameClearObject(&objects[i]);
				}

				// SECONDARY LOOP
				for (int j = 0; j < sizeof(objects) / sizeof(objects[0]); ++j)
				{
					// If not active, don't execute logic
					if (!objects[j].active)
						continue;

					// PROJECTILE TO ENEMY COLLISION
					if (objects[j].type == OBJ_ENEMY ||
						objects[j].type == OBJ_ENEMY_ELITE ||
						objects[j].type == OBJ_ENEMY_BOSS)
					{
						if (objects[j].cirHitTimer <= 0.0f)
						{
							// DEAL DAMAGE TO ENEMY
							if (AreCirclesIntersecting(&objects[i], &objects[j]))
							{
								//GameProjectileExplode(&objects[i], false);

								GamePlayParticleAndSound(GE_ENEMY_HURT, &objects[j], (int)GameDealDamage(&objects[j], weaponInfo[ORBITERS].baseDamage));
								/*ParticleSpawn(
									PARTICLE_TYPE_ENEMY_HIT,
									objects[j].position.x - cameraOffset.x,
									objects[j].position.y - cameraOffset.y,
									PARTICLEDEF_LIFETIME,
									(int)GameDealDamage(&objects[j], weaponInfo[PLASMA_BALL].baseDamage * 1.0f + ((float)(weaponInfo[PLASMA_BALL].level - 1.0f) * 0.5f))
								);*/
								objects[j].cirHitTimer = cirHitInterval;
							}
						}
					}
				}
				break;
			}
			case OBJ_ALLY_AOE:
			{
				objects[i].positionBase = CP_Vector_Set(gameCenter.x - cameraOffset.x, gameCenter.y - cameraOffset.y);

				float offset = 0.5f;
				objects[i].radius = weaponInfo[ELECTRO_DOME].baseSize * (1.0f + ((float)(weaponInfo[ELECTRO_DOME].level - 1.0f) * offset));
				if (aoeAlpha >= 255.0f)
				{
					// SECONDARY LOOP
					for (int j = 0; j < sizeof(objects) / sizeof(objects[0]); ++j)
					{
						// If not active, don't execute logic
						if (!objects[j].active)
							continue;
						
						// PROJECTILE TO ENEMY COLLISION
						if (objects[j].type == OBJ_ENEMY ||
							objects[j].type == OBJ_ENEMY_ELITE ||
							objects[j].type == OBJ_ENEMY_BOSS)
						{
							// DEAL DAMAGE TO ENEMY
							if (AreCirclesIntersecting(&objects[i], &objects[j]))
							{
								// GameProjectileExplode(&objects[i]);
								// float damage = weaponInfo[ELECTRO_DOME].baseDamage * 1.0f + ((float)(weaponInfo[ELECTRO_DOME].level - 1.0f) * 0.2f);

								GamePlayParticleAndSound(GE_ENEMY_HURT, &objects[j], (int)GameDealDamage(&objects[j], weaponInfo[ELECTRO_DOME].baseDamage));
								/*ParticleSpawn(
									PARTICLE_TYPE_ENEMY_HIT,
									objects[j].position.x - cameraOffset.x,
									objects[j].position.y - cameraOffset.y,
									PARTICLEDEF_LIFETIME,
									(int)GameDealDamage(&objects[j], damage)
								);*/
							}
						}
					}
				}
				break;
			}
			case OBJ_ENEMY:
			{
				if (objects[i].cirHitTimer >= 0.0f)
					objects[i].cirHitTimer -= CP_System_GetDt();
				// DEATH CONDITION
				if (objects[i].healthCurrent <= 0.0f)
				{
					GamePlayParticleAndSound(GE_ENEMY_DIE, &objects[i], 0);
					GameGainExp(10);
					GameClearObject(&objects[i]);
				}

				if (objects[i].tweening == 0 && player->active)
				{
					CP_Vector dir = CP_Vector_Set(
						gameCenter.x - objects[i].position.x,
						gameCenter.y - objects[i].position.y
					);
					dir = CP_Vector_Normalize(dir);

					objects[i].velocity.x = dir.x * objects[i].speed * CP_System_GetDt();
					objects[i].velocity.y = dir.y * objects[i].speed * CP_System_GetDt();
				}

				// ENEMY TO OBSTACLE + ENEMY COLLISION
				for (int j = 0; j < sizeof(objects) / sizeof(objects[0]); ++j)
				{
					// If not active, don't execute logic
					if (!objects[j].active)
						continue;

					// Only consider obstacle types + enemy types
					if (objects[j].type == OBJ_OBSTACLE || objects[j].type == OBJ_ENEMY)
					{
						// If collide
						if (AreCirclesIntersecting(&objects[i], &objects[j]) && IsMovingCloser(&objects[i], &objects[j]))
						{
							CP_Vector toObstacle = CP_Vector_Set(
								objects[j].position.x - objects[i].position.x,
								objects[j].position.y - objects[i].position.y
							);
							CP_Vector reactionVelocity = VectorProjection(objects[i].velocity, toObstacle);
							objects[i].velocity.x -= reactionVelocity.x;
							objects[i].velocity.y -= reactionVelocity.y;
						}
					}
				}

				// ENEMY TO PLAYER COLLISION
				if (AreCirclesIntersecting(&objects[i], player) && IsMovingCloser(&objects[i], player) && player->active)
				{
					objects[i].tweening = 1;

					CP_Vector toPlayer = CP_Vector_Set(
						gameCenter.x - objects[i].position.x,
						gameCenter.y - objects[i].position.y
					);

					CP_Vector reactionVelocity = VectorProjection(
						CP_Vector_Set(
							objects[i].velocity.x,
							objects[i].velocity.y
						),
						toPlayer
					);

					reactionVelocity.x = -reactionVelocity.x;
					reactionVelocity.y = -reactionVelocity.y;

					objects[i].velocity.x = 0.0f;
					objects[i].velocity.y = 0.0f;

					GameSetTween(i, CP_Vector_Normalize(reactionVelocity), 500.0f, 2000.0f);

					// PLAYER TAKE DAMAGE
					GamePlayParticleAndSound(GE_PLAYER_HURT, NULL, (int)GameDealDamage(player, 10.0f));
					//ParticleSpawn(
					//	PARTICLE_TYPE_PLAYER_HIT,
					//	player->position.x - cameraOffset.x,
					//	player->position.y - cameraOffset.y,
					//	PARTICLEDEF_LIFETIME,
					//	(int)GameDealDamage(player, 10.0f)
					//);
				}

				objects[i].positionBase.x += objects[i].velocity.x;
				objects[i].positionBase.y += objects[i].velocity.y;

				break;
			}
			case OBJ_ENEMY_PROJECTILE:
			{
				objects[i].positionBase.x += objects[i].velocity.x * CP_System_GetDt();
				objects[i].positionBase.y += objects[i].velocity.y * CP_System_GetDt();

				// LIFETIME
				objects[i].lifetime -= CP_System_GetDt();
				if (objects[i].lifetime <= 0.0f)
				{
					GameClearObject(&objects[i]);
				}

				// SECONDARY LOOP
				for (int j = 0; j < sizeof(objects) / sizeof(objects[0]); ++j)
				{
					// If not active, don't execute logic
					if (!objects[j].active)
						continue;

					// PROJECTILE TO ENEMY COLLISION
					if (objects[j].type == OBJ_PLAYER)
					{
						// DEAL DAMAGE TO ENEMY
						if (AreCirclesIntersecting(&objects[i], &objects[j]))
						{
							GameProjectileExplode(&objects[i], false);

							GamePlayParticleAndSound(GE_PLAYER_HURT, &objects[j], (int)GameDealDamage(&objects[j], 10.0f));
							/*ParticleSpawn(
								PARTICLE_TYPE_ENEMY_HIT,
								objects[j].position.x - cameraOffset.x,
								objects[j].position.y - cameraOffset.y,
								PARTICLEDEF_LIFETIME,
								(int)GameDealDamage(&objects[j], 10.0f)
							);*/
						}
					}

					// PROJECTILE TO OBSTACLE COLLISION
					if (objects[j].type == OBJ_OBSTACLE)
					{
						// IF COLLIDE
						if (AreCirclesIntersecting(&objects[i], &objects[j]))
						{
							GameProjectileExplode(&objects[i], true);
						}
					}
				}
				break;
			}
			case OBJ_ENEMY_ELITE:
			{
				if (objects[i].cirHitTimer >= 0.0f)
					objects[i].cirHitTimer -= CP_System_GetDt();
				// DEATH CONDITION
				if (objects[i].healthCurrent <= 0.0f)
				{
					GamePlayParticleAndSound(GE_ENEMY_DIE, &objects[i], 0);
					GameGainExp(30);
					GameClearObject(&objects[i]);
				}

				if (objects[i].tweening == 0 && player->active)
				{
					CP_Vector dir = CP_Vector_Set(
						gameCenter.x - objects[i].position.x,
						gameCenter.y - objects[i].position.y
					);
					dir = CP_Vector_Normalize(dir);
					dir = CP_Vector_MatrixMultiply(CP_Matrix_Rotate(30.0f), dir);

					objects[i].velocity.x = dir.x * objects[i].speed * CP_System_GetDt();
					objects[i].velocity.y = dir.y * objects[i].speed * CP_System_GetDt();
				}

				// ENEMY_ELITE TO OBSTACLE + ENEMY_ELITE COLLISION
				for (int j = 0; j < sizeof(objects) / sizeof(objects[0]); ++j)
				{
					// If not active, don't execute logic
					if (!objects[j].active)
						continue;

					// Only consider obstacle types + enemy elite types
					if (objects[j].type == OBJ_OBSTACLE || objects[j].type == OBJ_ENEMY_ELITE)
					{
						// If collide
						if (AreCirclesIntersecting(&objects[i], &objects[j]) && IsMovingCloser(&objects[i], &objects[j]))
						{
							CP_Vector toObstacle = CP_Vector_Set(
								objects[j].position.x - objects[i].position.x,
								objects[j].position.y - objects[i].position.y
							);
							CP_Vector reactionVelocity = VectorProjection(objects[i].velocity, toObstacle);
							objects[i].velocity.x -= reactionVelocity.x;
							objects[i].velocity.y -= reactionVelocity.y;
						}
					}


				}

				// ENEMY_ELITE TO PLAYER COLLISION
				if (AreCirclesIntersecting(&objects[i], player) && IsMovingCloser(&objects[i], player) && player->active)
				{
					objects[i].tweening = 1;

					CP_Vector toPlayer = CP_Vector_Set(
						gameCenter.x - objects[i].position.x,
						gameCenter.y - objects[i].position.y
					);

					CP_Vector reactionVelocity = VectorProjection(
						CP_Vector_Set(
							objects[i].velocity.x,
							objects[i].velocity.y
						),
						toPlayer
					);

					reactionVelocity.x = -reactionVelocity.x;
					reactionVelocity.y = -reactionVelocity.y;

					objects[i].velocity.x = 0.0f;
					objects[i].velocity.y = 0.0f;

					GameSetTween(i, CP_Vector_Normalize(reactionVelocity), 500.0f, 2000.0f);

					// PLAYER TAKE DAMAGE
					GamePlayParticleAndSound(GE_PLAYER_HURT, NULL, (int)GameDealDamage(player, 30.0f));
					//ParticleSpawn(
					//	PARTICLE_TYPE_PLAYER_HIT,
					//	player->position.x - cameraOffset.x,
					//	player->position.y - cameraOffset.y,
					//	PARTICLEDEF_LIFETIME,
					//	(int)GameDealDamage(player, 10.0f)
					//);
				}

				objects[i].positionBase.x += objects[i].velocity.x;
				objects[i].positionBase.y += objects[i].velocity.y;

				break;
				break;
			}
			case OBJ_ENEMY_BOSS:
			{
				if (objects[i].cirHitTimer >= 0.0f)
					objects[i].cirHitTimer -= CP_System_GetDt();
				// DEATH CONDITION
				if (objects[i].healthCurrent <= 0.0f)
				{
					GameGainExp(100);
					GameClearObject(&objects[i]);
				}

				if (objects[i].tweening == 0 && player->active)
				{
					CP_Vector dir = CP_Vector_Set(
						gameCenter.x - objects[i].position.x,
						gameCenter.y - objects[i].position.y
					);
					dir = CP_Vector_Normalize(dir);

					objects[i].velocity.x = dir.x * objects[i].speed * CP_System_GetDt();
					objects[i].velocity.y = dir.y * objects[i].speed * CP_System_GetDt();
				}
				CreateBossEnemyFireProjectile(&objects[i]);

				// ENEMY TO OBSTACLE COLLISION
				for (int j = 0; j < sizeof(objects) / sizeof(objects[0]); ++j)
				{
					// If not active, don't execute logic
					if (!objects[j].active)
						continue;

					// Only consider obstacle types
					if (objects[j].type == OBJ_OBSTACLE || objects[j].type == OBJ_ENEMY)
					{
						// If collide
						if (AreCirclesIntersecting(&objects[i], &objects[j]) && IsMovingCloser(&objects[i], &objects[j]))
						{
							CP_Vector toObstacle = CP_Vector_Set(
								objects[j].position.x - objects[i].position.x,
								objects[j].position.y - objects[i].position.y
							);
							CP_Vector reactionVelocity = VectorProjection(objects[i].velocity, toObstacle);
							objects[i].velocity.x -= reactionVelocity.x;
							objects[i].velocity.y -= reactionVelocity.y;
						}
					}
				}

				// ENEMY TO PLAYER COLLISION
				if (AreCirclesIntersecting(&objects[i], player) && IsMovingCloser(&objects[i], player) && player->active)
				{
					objects[i].tweening = 1;

					CP_Vector toPlayer = CP_Vector_Set(
						gameCenter.x - objects[i].position.x,
						gameCenter.y - objects[i].position.y
					);

					CP_Vector reactionVelocity = VectorProjection(
						CP_Vector_Set(
							objects[i].velocity.x,
							objects[i].velocity.y
						),
						toPlayer
					);

					reactionVelocity.x = -reactionVelocity.x;
					reactionVelocity.y = -reactionVelocity.y;

					objects[i].velocity.x = 0.0f;
					objects[i].velocity.y = 0.0f;

					GameSetTween(i, CP_Vector_Normalize(reactionVelocity), 500.0f, 2000.0f);


					// PLAYER TAKE DAMAGE
					GamePlayParticleAndSound(GE_PLAYER_HURT, NULL, (int)GameDealDamage(player, 50.0f));
					/*ParticleSpawn(
						PARTICLE_TYPE_PLAYER_HIT,
						player->position.x - cameraOffset.x,
						player->position.y - cameraOffset.y,
						PARTICLEDEF_ALPHA_MAX,
						(int)GameDealDamage(player, 10.0f)
					);*/
				}

				objects[i].positionBase.x += objects[i].velocity.x;
				objects[i].positionBase.y += objects[i].velocity.y;

				break;
			}
			case OBJ_OBSTACLE:
			{
				break;
			}
			default:
			{
				break;
			}
			}

			// CAMERA OFFSET
			if (&objects[i] != player)
			{
				objects[i].position.x = objects[i].positionBase.x + cameraOffset.x;
				objects[i].position.y = objects[i].positionBase.y + cameraOffset.y;
			}
		}

		// PLAYER MOVEMENT -> CAMERA OFFSET
		// Do this after object loop for collision
		if (player != NULL && player->active)
		{
			cameraOffset.x -= player->velocity.x;
			cameraOffset.y -= player->velocity.y;
		}

		// ENEMY WAVE SPAWNING
		waveTimer -= CP_System_GetDt();

		if (waveTimer <= 0.0f)
		{
			waveTimer = waveInterval;
			waveCounter += 1;
			++bossSpawnCounter;
			CP_Vector spawnPosition = CP_Vector_Zero();
			spawnPosition.x = gameCenter.x - cameraOffset.x;
			spawnPosition.y = gameCenter.y - cameraOffset.y;

			if (bossSpawnCounter != 12) 
			{
				CP_Vector spawnOffset = CP_Vector_Set(/*(float)CP_System_GetWindowWidth() / 2.0f*/ 1000.0f, 0.0f);
				float deg = 360.0f / (float)enemiesPerWave;

				if (waveCounter != 3)
				{
					for (int enemiesSpawned = 0; enemiesSpawned < enemiesPerWave; ++enemiesSpawned)
					{
						GameObject* enemy;

						if (enemy = GameGetObject(OBJ_ENEMY))
						{
							*enemy = CreateGameObject(
								/*	type			*/	OBJ_ENEMY,
								/*	positionBase	*/	CP_Vector_Set(spawnPosition.x + spawnOffset.x, spawnPosition.y + spawnOffset.y),
								/*	position		*/	CP_Vector_Zero(),
								/*	healthMaximum	*/	20.0f,
								/*	speed			*/	100.0f,
								/*	radius			*/	15.0f,
								/*	lifetime		*/	0.0f
							);
							enemy->tweening = 0;
							enemy->spriteAnimTimeLimit = enemySprite.sprTimePerFrame;
							enemy->spriteIndexLimit = (int)enemySprite.sprTotalFrames;
							enemy->cirHitTimer = -0.1f;
						}
						spawnOffset = CP_Vector_MatrixMultiply(CP_Matrix_Rotate(deg), spawnOffset);
					}
				}
				else
				{

					waveCounter = 0;
					int enemiesSpawned = 0;
					for (enemiesSpawned; enemiesSpawned < 4; ++enemiesSpawned)
					{
						GameObject* enemy;

						if (enemy = GameGetObject(OBJ_ENEMY))
						{
							*enemy = CreateGameObject(
								/*	type			*/	OBJ_ENEMY,
								/*	positionBase	*/	CP_Vector_Set(spawnPosition.x + spawnOffset.x, spawnPosition.y + spawnOffset.y),
								/*	position		*/	CP_Vector_Zero(),
								/*	healthMaximum	*/	20.0f,
								/*	speed			*/	100.0f,
								/*	radius			*/	15.0f,
								/*	lifetime		*/	0.0f
							);
							enemy->tweening = 0;
							enemy->spriteAnimTimeLimit = enemySprite.sprTimePerFrame;
							enemy->spriteIndexLimit = (int)enemySprite.sprTotalFrames;
							enemy->cirHitTimer = -0.1f;
						}
						spawnOffset = CP_Vector_MatrixMultiply(CP_Matrix_Rotate(deg), spawnOffset);
					}
					for (enemiesSpawned; enemiesSpawned < enemiesPerWave; ++enemiesSpawned)
					{
						GameObject* enemy;

						if (enemy = GameGetObject(OBJ_ENEMY_ELITE))
						{
							*enemy = CreateGameObject(
								/*	type			*/	OBJ_ENEMY_ELITE,
								/*	positionBase	*/	CP_Vector_Set(spawnPosition.x + spawnOffset.x, spawnPosition.y + spawnOffset.y),
								/*	position		*/	CP_Vector_Zero(),
								/*	healthMaximum	*/	40.0f,
								/*	speed			*/	100.0f,
								/*	radius			*/	25.0f,
								/*	lifetime		*/	0.0f
							);
							enemy->tweening = 0;
							enemy->spriteAnimTimeLimit = enemyEliteSprite.sprTimePerFrame;
							enemy->spriteIndexLimit = (int)enemyEliteSprite.sprTotalFrames;
							enemy->cirHitTimer = -0.1f;
						}
						spawnOffset = CP_Vector_MatrixMultiply(CP_Matrix_Rotate(deg), spawnOffset);
					}
				}
			}
			else
			{
				waveCounter = 0;
				bossSpawnCounter = 0;
				CP_Vector spawnOffset = CP_Vector_Set(/*(float)CP_System_GetWindowWidth() / 2.0f*/ 1000.0f, 0.0f);
				float deg = 360.0f / 7.0f;
				int enemiesSpawned = 0;
				for (enemiesSpawned; enemiesSpawned < 4; ++enemiesSpawned)
				{
					GameObject* enemy;

					if (enemy = GameGetObject(OBJ_ENEMY))
					{
						*enemy = CreateGameObject(
							/*	type			*/	OBJ_ENEMY,
							/*	positionBase	*/	CP_Vector_Set(spawnPosition.x + spawnOffset.x, spawnPosition.y + spawnOffset.y),
							/*	position		*/	CP_Vector_Zero(),
							/*	healthMaximum	*/	20.0f,
							/*	speed			*/	100.0f,
							/*	radius			*/	15.0f,
							/*	lifetime		*/	0.0f
						);
						enemy->tweening = 0;
						enemy->spriteAnimTimeLimit = enemySprite.sprTimePerFrame;
						enemy->spriteIndexLimit = (int)enemySprite.sprTotalFrames;
						enemy->cirHitTimer = -0.1f;
					}
					spawnOffset = CP_Vector_MatrixMultiply(CP_Matrix_Rotate(deg), spawnOffset);
				}
				for (enemiesSpawned; enemiesSpawned < enemiesPerWave; ++enemiesSpawned)
				{
					GameObject* enemy;

					if (enemy = GameGetObject(OBJ_ENEMY_ELITE))
					{
						*enemy = CreateGameObject(
							/*	type			*/	OBJ_ENEMY_ELITE,
							/*	positionBase	*/	CP_Vector_Set(spawnPosition.x + spawnOffset.x, spawnPosition.y + spawnOffset.y),
							/*	position		*/	CP_Vector_Zero(),
							/*	healthMaximum	*/	50.0f,
							/*	speed			*/	100.0f,
							/*	radius			*/	25.0f,
							/*	lifetime		*/	0.0f
						);
						enemy->tweening = 0;
						enemy->spriteAnimTimeLimit = enemyEliteSprite.sprTimePerFrame;
						enemy->spriteIndexLimit = (int)enemyEliteSprite.sprTotalFrames;
						enemy->cirHitTimer = -0.1f;
					}
					spawnOffset = CP_Vector_MatrixMultiply(CP_Matrix_Rotate(deg), spawnOffset);
				}
				for (enemiesSpawned; enemiesSpawned < 7; ++enemiesSpawned)
				{
					GameObject* enemy;

					if (enemy = GameGetObject(OBJ_ENEMY_BOSS))
					{
						*enemy = CreateGameObject(
							/*	type			*/	OBJ_ENEMY_BOSS,
							/*	positionBase	*/	CP_Vector_Set(spawnPosition.x + spawnOffset.x, spawnPosition.y + spawnOffset.y),
							/*	position		*/	CP_Vector_Zero(),
							/*	healthMaximum	*/	75.0f,
							/*	speed			*/	100.0f,
							/*	radius			*/	45.0f,
							/*	lifetime		*/	0.0f
						);
						enemy->tweening = 0;
						enemy->spriteAnimTimeLimit = enemyBossSprite.sprTimePerFrame;
						enemy->spriteIndexLimit = (int)enemyBossSprite.sprTotalFrames;
						enemy->cirHitTimer = -0.1f;
					}
					spawnOffset = CP_Vector_MatrixMultiply(CP_Matrix_Rotate(deg), spawnOffset);
				}
			}

		}
		ParticleRender();
	} // GAME IS NOT PAUSE
	
	// GAME IS PAUSE HERE
	// GameObject loop;
	// RENDER OBJECTS EVERY FRAME; DOES NOT MATTER IF ITS PAUSE OR NOT PAUSE
	for (int i = sizeof(objects) / sizeof(objects[0]) - 1; i >= 0; --i)
	{
		// CONTINUE RENDER GAMEOBJECT EVEN ITS PAUSE
		// If not active, don't render
		if (!objects[i].active)
			continue;

		switch (objects[i].type)
		{
		case OBJ_CLEAR:
		{
			break;
		}
		case OBJ_PLAYER:
		{
			/*
			CP_Settings_Fill(purple);
			CP_Graphics_DrawCircle(
				objects[i].position.x,
				objects[i].position.y,
				objects[i].radius * 2
			);
			*/
			// Walking
			if (playerIsMoving)
			{
				if (playerIsFacingLeft)
					playerSpriteWalking.sprDir = LEFT;
				else
					playerSpriteWalking.sprDir = RIGHT;

				SpriteUpdate(&playerSpriteWalking);
				SpritePlayAnim(playerSpriteWalking, objects[i].position);
			}
			// Standing still
			else
			{
				if (playerIsFacingLeft)
					playerSpriteStanding.sprDir = LEFT;
				else
					playerSpriteStanding.sprDir = RIGHT;

				SpriteUpdate(&playerSpriteStanding);
				SpritePlayAnim(playerSpriteStanding, objects[i].position);
			}
			break;
		}
		case OBJ_ALLY_PROJECTILE:
		{
			CP_Settings_Fill(cyan);
			CP_Graphics_DrawCircle(
				objects[i].position.x,
				objects[i].position.y,
				objects[i].radius * 2
			);
			break;
		}
		case OBJ_ALLY_MULTI_SHOT:
		{
			/*CP_Settings_Fill(blue);
			CP_Graphics_DrawCircle(
				objects[i].position.x,
				objects[i].position.y,
				objects[i].radius * 2
			);*/
			CP_Image_Draw(
				weaponInfo[PLASMA_BALL].image,
				objects[i].position.x,
				objects[i].position.y,
				objects[i].radius * 2.f,
				objects[i].radius * 2.f,
				255);
			break;
		}
		case OBJ_ALLY_CIRCLING:
		{
			/*CP_Settings_Fill(green);
			CP_Graphics_DrawCircle(
				objects[i].position.x,
				objects[i].position.y,
				objects[i].radius * 2
			);*/
			CP_Image_Draw(
				weaponInfo[ORBITERS].image,
				objects[i].position.x,
				objects[i].position.y,
				objects[i].radius * 2.f,
				objects[i].radius * 2.f,
				255);
			break;
		}
		case OBJ_ALLY_AOE:
		{
			if (aoeActive)
			{
				/*CP_Settings_Fill(aoe);
				CP_Graphics_DrawCircle(
					objects[i].position.x,
					objects[i].position.y,
					objects[i].radius * 2
				);*/
				CP_Image_Draw(
					weaponInfo[ELECTRO_DOME].image,
					objects[i].position.x,
					objects[i].position.y,
					objects[i].radius * 2.f,
					objects[i].radius * 2.f,
					(int)aoeAlpha);
			}
			break;
		}
		case OBJ_ENEMY:
		{
			/*CP_Settings_Fill(red);
			CP_Graphics_DrawCircle(
				objects[i].position.x,
				objects[i].position.y,
				objects[i].radius * 2
			);*/
			GameSpriteProgress(&objects[i]);
			SpritePlayAnimIndex(enemySprite, objects[i].position, objects[i].spriteIndex, objects[i].radius * 3.5f);
			break;
		}
		case OBJ_ENEMY_PROJECTILE:
		{
			CP_Settings_Fill(purple);
			CP_Graphics_DrawCircle(
				objects[i].position.x,
				objects[i].position.y,
				objects[i].radius * 2
			);
			break;
		}
		case OBJ_ENEMY_ELITE:
		{
			/*CP_Settings_Fill(orange);
			CP_Graphics_DrawCircle(
				objects[i].position.x,
				objects[i].position.y,
				objects[i].radius * 2
			);*/
			GameSpriteProgress(&objects[i]);
			SpritePlayAnimIndex(enemyEliteSprite, objects[i].position, objects[i].spriteIndex, objects[i].radius * 3.5f);
			break;
		}
		case OBJ_ENEMY_BOSS:
		{
			/*CP_Settings_Fill(purple);
			CP_Graphics_DrawCircle(
				objects[i].position.x,
				objects[i].position.y,
				objects[i].radius * 2
			);*/
			GameSpriteProgress(&objects[i]);
			SpritePlayAnimIndex(enemyBossSprite, objects[i].position, objects[i].spriteIndex, objects[i].radius * 3.5f);
			break;
		}
		case OBJ_OBSTACLE:
		{
			DrawObstacle(&objects[i]);
			break;
		}
		default:
		{
			break;
		}
		}
	}

	// Ryan's UI
	if (playerExp > 0)
	{
		CP_Settings_Fill(white);
		CP_Graphics_DrawRect(
			0.0f,
			gameCenter.y - ((float)CP_System_GetWindowHeight() / 2.0f),
			(float)CP_System_GetWindowWidth() * ((float)playerExp / (float)playerExpLimit),
			20.0f
		);
	}
	// Render Score
	CP_Settings_TextSize(60.0f * playerHealthIcon.btnScale.x);
	CP_Settings_Fill(white);
	CP_Font_DrawText(
		playerScoreDisplay,
		(float)CP_System_GetWindowWidth() - (350.0f * playerHealthIcon.btnScale.x),
		(gameCenter.y - ((float)CP_System_GetWindowHeight() / 2.0f) + 90.0f) * playerHealthIcon.btnScale.y
	);
	// Render Health
	ButtonDraw(playerHealthIcon, 0.f);
	CP_Settings_Fill(red);
	CP_Settings_TextSize(60.0f * playerHealthIcon.btnScale.x);
	CP_Font_DrawText(
		playerHealthDisplay, 
		playerHealthIcon.btnPos.x +  0.5f * (CP_Image_GetWidth(playerHealthIcon.btnImage) * playerHealthIcon.btnScale.x),
		(gameCenter.y - ((float)CP_System_GetWindowHeight() / 2.0f) + 90.f) * playerHealthIcon.btnScale.y
	);

	// PARTICLE RENDER
	ParticleRender();
	if (CP_Input_KeyTriggered(KEY_ESCAPE))
	{
		gamePause = !gamePause;
		if (gamePause)
			AudioEnterMenu();
		else
			AudioExitMenu();
	}

	// WEAPON MENU SELECTION
	if (weaponMenuOpen) // IF ITS GAME PAUSE AND WEAPON MENU OPEN
	{
		// RENDER UI
		GameDrawLevelUpUI();
		GameCheckButtonClicks();
	}
	if (gamePause && !gameEnd)
	{
		GameDrawPauseMenuUI();
		GameCheckButtonClicks();
	}
	if (gameEnd)
	{
		GameDrawEndScreen();
		GameCheckButtonClicks();
	}

	// Main Menu > Game Fade
	if (GetMTG())
	{
		if (GetDisplay() == RES_720)
		{
			SpriteUpdate(&transitionOpen720Game);
			if (transitionOpen720Game.sprAnimIndex >= transitionOpen720Game.sprTotalFrames - 1)
			{
				SetMTG(false);
			}

			SpriteBetterPlayAnim(transitionOpen720Game, CP_Vector_Set((float)CP_System_GetWindowWidth() / 2.f, (float)CP_System_GetWindowHeight() / 2.f));
		}
		else
		{
			SpriteUpdate(&transitionOpen1080Game);
			if (transitionOpen1080Game.sprAnimIndex >= transitionOpen1080Game.sprTotalFrames - 1)
			{
				SetMTG(false);
			}

			SpriteBetterPlayAnim(transitionOpen1080Game, CP_Vector_Set((float)CP_System_GetWindowWidth() / 2.f, (float)CP_System_GetWindowHeight() / 2.f));
		}
	}
	// Game > Main Menu Fade || Game Restart
	if (GetGTM() || GetGTG())
	{
		if (GetDisplay() == RES_720)
		{
			SpriteUpdate(&transitionClose720Game);
			if (transitionClose720Game.sprAnimIndex >= transitionClose720Game.sprTotalFrames - 1)
			{
				if (GetGTG())
				{
					SetMTG(true);
					SetGTG(false);
					CP_Engine_SetNextGameStateForced(GameInit, GameUpdate, GameExit);
				}
				if (GetGTM())
				{
					CP_Engine_SetNextGameState(MainMenuInit, MainMenuUpdate, MainMenuExit);	
				}
			}

			SpriteBetterPlayAnim(transitionClose720Game, CP_Vector_Set((float)CP_System_GetWindowWidth() / 2.f, (float)CP_System_GetWindowHeight() / 2.f));
		}
		else
		{
			SpriteUpdate(&transitionClose1080Game);
			if (transitionClose1080Game.sprAnimIndex >= transitionClose1080Game.sprTotalFrames - 1)
			{
				if (GetGTG())
				{
					SetMTG(true);
					SetGTG(false);
					CP_Engine_SetNextGameStateForced(GameInit, GameUpdate, GameExit);
				}
				if (GetGTM())
				{
					CP_Engine_SetNextGameState(MainMenuInit, MainMenuUpdate, MainMenuExit);
				}
			}

			SpriteBetterPlayAnim(transitionClose1080Game, CP_Vector_Set((float)CP_System_GetWindowWidth() / 2.f, (float)CP_System_GetWindowHeight() / 2.f));
		}
	}

	if (gamePause || gameEnd || gameLevelUp)
	{
		// draw normal cursor
		SpriteUpdate(&cursorUI);
		SpritePlayAnim(cursorUI,
			CP_Vector_Set(CP_Input_GetMouseX() + 0.5f * CP_Image_GetHeight(cursorUI.sprLeftSpriteSheet),
				CP_Input_GetMouseY() + 0.5f * CP_Image_GetHeight(cursorUI.sprLeftSpriteSheet)));
	}
	else
	{
		// draw aim cursor
		SpriteUpdate(&cursorAim);
		SpritePlayAnim(cursorAim,
			CP_Vector_Set(CP_Input_GetMouseX(),
				CP_Input_GetMouseY()));
	}
}

void CreateBasicFireProjectile(void)
{
	bfTimer -= CP_System_GetDt();
	if (bfTimer <= 0.0f)
	{
		bfTimer = bfInterval;
		GameObject* allyBullet;

		if (allyBullet = GameGetObject(OBJ_ALLY_PROJECTILE))
		{
			CP_Vector dir = CP_Vector_Zero();
			dir.x = CP_Input_GetMouseX() - gameCenter.x;
			dir.y = CP_Input_GetMouseY() - gameCenter.y;
			dir = CP_Vector_Normalize(dir);

			*allyBullet = CreateGameObject(
				/*	type			*/	OBJ_ALLY_PROJECTILE,
				/*	positionBase	*/	CP_Vector_Set(gameCenter.x - cameraOffset.x, gameCenter.y - cameraOffset.y),
				/*	position		*/	CP_Vector_Zero(),
				/*	healthMaximum	*/	0.0f,
				/*	speed			*/	500.0f,
				/*	radius			*/	5.0f,
				/*	lifetime		*/	3.0f
			);

			allyBullet->velocity.x = /* player->velocity.x + */ dir.x * allyBullet->speed;
			allyBullet->velocity.y = /* player->velocity.y + */ dir.y * allyBullet->speed;

			// Play Shoot audio
			//AudioPlay(AUDIO_TYPE_PLAYER_SHOOT_SFX);
			GamePlayParticleAndSound(GE_PLAYER_SHOOT, NULL, 0);
		}
	}
}

void CreateMultiShotProjectile(void)
{
	msTimer -= CP_System_GetDt();
	if (msTimer <= 0.0f)
	{
		msTimer = msInterval;
		GameObject* projectileA;
		GameObject* projectileB;
		GameObject* projectileC;
		CP_Vector dir = CP_Vector_Zero();
		dir = CP_Vector_Set(CP_Input_GetMouseX() - gameCenter.x, CP_Input_GetMouseY() - gameCenter.y);
		dir = CP_Vector_Normalize(dir);

		float angleOffset = 15.0f;
		CP_Vector dirOffset = CP_Vector_MatrixMultiply(CP_Matrix_Rotate(angleOffset), dir);
		CP_Vector dirOffset2 = CP_Vector_MatrixMultiply(CP_Matrix_Rotate(-angleOffset), dir);
		CP_Vector dirOffset3;

		switch (weaponInfo[PLASMA_BALL].level)
		{
		case 3:
			if (projectileC = GameGetObject(OBJ_ALLY_MULTI_SHOT))
			{
				*projectileC = CreateGameObject(
					/*	type			*/	OBJ_ALLY_MULTI_SHOT,
					/*	positionBase	*/	CP_Vector_Set(gameCenter.x - cameraOffset.x, gameCenter.y - cameraOffset.y),
					/*	position		*/	CP_Vector_Zero(),
					/*	healthMaximum	*/	0.0f,
					/*	speed			*/	500.0f,
					/*	radius			*/	weaponInfo[PLASMA_BALL].baseSize,
					/*	lifetime		*/	3.0f
				);
				projectileC->velocity = CP_Vector_Set(dirOffset2.x * projectileC->speed, dirOffset2.y * projectileC->speed);
			}
		case 2:
			if (projectileB = GameGetObject(OBJ_ALLY_MULTI_SHOT))
			{
				*projectileB = CreateGameObject(
					/*	type			*/	OBJ_ALLY_MULTI_SHOT,
					/*	positionBase	*/	CP_Vector_Set(gameCenter.x - cameraOffset.x, gameCenter.y - cameraOffset.y),
					/*	position		*/	CP_Vector_Zero(),
					/*	healthMaximum	*/	0.0f,
					/*	speed			*/	500.0f,
					/*	radius			*/	weaponInfo[PLASMA_BALL].baseSize,
					/*	lifetime		*/	3.0f
				);
				angleOffset = 10.0f;
				dirOffset3 = CP_Vector_MatrixMultiply(CP_Matrix_Rotate(-angleOffset), dir);
				if (weaponInfo[PLASMA_BALL].level != 2)
					projectileB->velocity = CP_Vector_Set(dirOffset.x * projectileB->speed, dirOffset.y * projectileB->speed);
				else
					projectileB->velocity = CP_Vector_Set(dirOffset3.x * projectileB->speed, dirOffset3.y * projectileB->speed);

			}
		case 1:
			if (projectileA = GameGetObject(OBJ_ALLY_MULTI_SHOT))
			{
				*projectileA = CreateGameObject(
					/*	type			*/	OBJ_ALLY_MULTI_SHOT,
					/*	positionBase	*/	CP_Vector_Set(gameCenter.x - cameraOffset.x, gameCenter.y - cameraOffset.y),
					/*	position		*/	CP_Vector_Zero(),
					/*	healthMaximum	*/	0.0f,
					/*	speed			*/	500.0f,
					/*	radius			*/	weaponInfo[PLASMA_BALL].baseSize,
					/*	lifetime		*/	3.0f
				);

				dirOffset3 = CP_Vector_MatrixMultiply(CP_Matrix_Rotate(angleOffset), dir);
				if (weaponInfo[PLASMA_BALL].level != 2)
					projectileA->velocity = CP_Vector_Set(dir.x * projectileA->speed, dir.y * projectileA->speed);
				else
					projectileA->velocity = CP_Vector_Set(dirOffset3.x * projectileA->speed, dirOffset3.y * projectileA->speed);
			}
			break;
		default:
			break;
		}

		GamePlayParticleAndSound(GE_PLAYER_MULTI, NULL, 0);
	}
}

void CreateCirclingProjectile(void)
{
	cirTimer = cirInterval;
	CP_Vector spawnPosition = CP_Vector_Set(gameCenter.x - cameraOffset.x, gameCenter.y - cameraOffset.y);

	if (weaponInfo[ORBITERS].level == 1)
	{
		for (int i = 0; i < 2; ++i)
		{
			GameObject* projectile;

			if (projectile = GameGetObject(OBJ_ALLY_CIRCLING))
			{
				*projectile = CreateGameObject(
					/*	type			*/	OBJ_ALLY_CIRCLING,
					/*	positionBase	*/	CP_Vector_Set(spawnPosition.x, spawnPosition.y),
					/*	position		*/	CP_Vector_Zero(),
					/*	healthMaximum	*/	0.0f,
					/*	speed			*/	5.0f,
					/*	radius			*/	20.0f,
					/*	lifetime		*/	6.0f
				);
				projectile->range = 60.0f;

				switch (i)
				{
				case 0:
					projectile->cirOffset = CP_Vector_Set(1.0f, 0.0f);
					break;
				case 1:
					projectile->cirOffset = CP_Vector_Set(-1.0f, 0.0f);
					break;
				default:
					break;
				}
			}
		}
	}
	else if (weaponInfo[ORBITERS].level == 2)
	{
		for (int i = 0; i < 3; ++i)
		{
			GameObject* projectile;

			if (projectile = GameGetObject(OBJ_ALLY_CIRCLING))
			{
				*projectile = CreateGameObject(
					/*	type			*/	OBJ_ALLY_CIRCLING,
					/*	positionBase	*/	CP_Vector_Set(spawnPosition.x, spawnPosition.y),
					/*	position		*/	CP_Vector_Zero(),
					/*	healthMaximum	*/	0.0f,
					/*	speed			*/	5.0f,
					/*	radius			*/	20.0f,
					/*	lifetime		*/	6.0f
				);
				projectile->range = 60.0f;

				switch (i)
				{
				case 0:
					projectile->cirOffset = CP_Vector_Set(1.0f, -0.5f);
					break;
				case 1:
					projectile->cirOffset = CP_Vector_Set(-1.0f, -0.5f);
					break;
				case 2:
					projectile->cirOffset = CP_Vector_Set(0.0f, 1.0f);
					break;
				default:
					break;
				}
			}
		}
	}
	else if (weaponInfo[ORBITERS].level == 3)
	{
		for (int i = 0; i < 4; ++i)
		{
			GameObject* projectile;

			if (projectile = GameGetObject(OBJ_ALLY_CIRCLING))
			{
				*projectile = CreateGameObject(
					/*	type			*/	OBJ_ALLY_CIRCLING,
					/*	positionBase	*/	CP_Vector_Set(spawnPosition.x, spawnPosition.y),
					/*	position		*/	CP_Vector_Zero(),
					/*	healthMaximum	*/	0.0f,
					/*	speed			*/	5.0f,
					/*	radius			*/	20.0f,
					/*	lifetime		*/	6.0f
				);
				projectile->range = 60.0f;

				switch (i)
				{
				case 0:
					projectile->cirOffset = CP_Vector_Set(1.0f, 0.0f);
					break;
				case 1:
					projectile->cirOffset = CP_Vector_Set(-1.0f, 0.0f);
					break;
				case 2:
					projectile->cirOffset = CP_Vector_Set(0.0f, 1.0f);
					break;
				case 3:
					projectile->cirOffset = CP_Vector_Set(0.0f, -1.0f);
					break;
				default:
					break;
				}
			}
		}
	}
}

void GameUpdateScoreDisplay()
{
	sprintf_s(playerScoreDisplay, DISPLAY_ARR, "%010ld", playerScore);

	/*
	printf("SCORE	: ");
	for (int i = 0; i < DISPLAY_ARR; ++i)
	{
		if (playerScoreDisplay[i] == '\0')
			break;

		printf("%c", playerScoreDisplay[i]);
	}
	printf("\n");
	*/
}

void CreateBossEnemyFireProjectile(GameObject* object)
{
	enemyFireTimer -= CP_System_GetDt();
	if (enemyFireTimer <= 0.0f)
	{
		enemyBossBurst = true;
		if (enemyBossBurst && enemyBulletCounter > 0)
		{
			enemyBurstTimer -= CP_System_GetDt();
			if (enemyBurstTimer <= 0.0f)
			{
				GameObject* enemyBullet;
				if (enemyBullet = GameGetObject(OBJ_ENEMY_PROJECTILE))
				{
					CP_Vector dir = CP_Vector_Zero();
					dir.x = gameCenter.x - object->position.x;
					dir.y = gameCenter.y - object->position.y;
					dir = CP_Vector_Normalize(dir);

					*enemyBullet = CreateGameObject(
						/*	type			*/	OBJ_ENEMY_PROJECTILE,
						/*	positionBase	*/	CP_Vector_Set(object->position.x - cameraOffset.x, object->position.y - cameraOffset.y),
						/*	position		*/	CP_Vector_Zero(),
						/*	healthMaximum	*/	0.0f,
						/*	speed			*/	500.0f,
						/*	radius			*/	5.0f,
						/*	lifetime		*/	3.0f
					);

					enemyBullet->velocity.x = /* player->velocity.x + */ dir.x * enemyBullet->speed;
					enemyBullet->velocity.y = /* player->velocity.y + */ dir.y * enemyBullet->speed;

					// Play Shoot audio
					//AudioPlay(AUDIO_TYPE_PLAYER_SHOOT_SFX);
					GamePlayParticleAndSound(GE_PLAYER_SHOOT, NULL, 0);
					--enemyBulletCounter;
					enemyBurstTimer = enemyBurstInterval;
				}
			}
		}
		else if (enemyBulletCounter <= 0)
		{

			enemyBossBurst = false;
			enemyFireTimer = enemyFireInterval;
			enemyBulletCounter = 5;
		}
		
		
	}
}

void GameExit(void)
{
	player = NULL;
	FreeMap();
	FreeObstacle();

	// Free Weapon Images
	CP_Image_Free(&weaponInfo[PLASMA_BALL].image);
	CP_Image_Free(&weaponInfo[ORBITERS].image);
	CP_Image_Free(&weaponInfo[ELECTRO_DOME].image);
	// Stops rendering particle and stops audio
	ParticleExit();
	AudioExit();
	if (isQuittingToMainMenu == false)
	{
		// Game was altf4'd
		ParticleQuitGame();
		AudioQuitGame();
		// printf("Quitting exe...\n");
	}
	else
	{
		// printf("Quitting to main menu...\n");
	}
		
	// Free UI
	for (int i = 0; i < sizeof(levelUpOptionButtons) / sizeof(levelUpOptionButtons[0]); ++i)
	{
		ButtonReleaseVars(&levelUpOptionButtons[i]);
	}
	for (int i = 0; i < sizeof(pauseMenuButtons) / sizeof(pauseMenuButtons[0]); ++i)
	{
		ButtonReleaseVars(&pauseMenuButtons[i]);
	}
	for (int i = 0; i < sizeof(endScreenButtons) / sizeof(endScreenButtons[0]); ++i)
	{
		ButtonReleaseVars(&endScreenButtons[i]);
	}
	ButtonReleaseVars(&levelUpBackdrop);
	ButtonReleaseVars(&pauseBackdrop);
	ButtonReleaseVars(&pauseAudioBarActiveIcon);
	ButtonReleaseVars(&pauseAudioBarInactiveIcon);
	ButtonReleaseVars(&endBackdrop);
	ButtonReleaseVars(&oribitersIcon);
	ButtonReleaseVars(&electroIcon);
	ButtonReleaseVars(&plasmaIcon);
	ButtonReleaseVars(&playerHealthIcon);
	ButtonReleaseVars(&levelUpMaxedOption);
	SpriteReleaseVars(&playerSpriteStanding);
	SpriteReleaseVars(&playerSpriteWalking);
	SpriteReleaseVars(&enemySprite);
	SpriteReleaseVars(&enemyEliteSprite);
	SpriteReleaseVars(&enemyBossSprite);
	SpriteReleaseVars(&cursorAim);
	SpriteReleaseVars(&cursorUI);
	SpriteReleaseVars(&transitionClose1080Game);
	SpriteReleaseVars(&transitionClose720Game);
	SpriteReleaseVars(&transitionOpen1080Game);
	SpriteReleaseVars(&transitionOpen720Game);
}

void GameGainExp(int exp)
{
	if (player->active)
	{
		playerExp += exp;
		playerScore += exp;

		if (playerExp > playerExpLimit)
			playerExp = playerExpLimit;

		// printf("EXP	: %d / %d\n", playerExp, playerExpLimit);
		GameUpdateScoreDisplay();

		if (playerExp >= playerExpLimit)
		{
			// printf("Level Up!\n");
			++playerLevel;
			playerExp = 0;
			playerExpLimit += playerLevel * 20;
			if (weaponInfo[PLASMA_BALL].level < maxWeaponLevel || weaponInfo[ELECTRO_DOME].level < maxWeaponLevel ||
				weaponInfo[ORBITERS].level < maxWeaponLevel)
			{
				weaponMenuOpen = true;
				AudioEnterMenu();
			}

		}
	}
}

// GameObject* objectSoundFrom AND int value ARE OPTIONAL (SET TO NULL, 0 IF UNUSED)
void GamePlayParticleAndSound(GameEvent eventType, GameObject* objectSoundFrom, int value)
{
	switch (eventType)
	{
	case GE_PROJ_HIT:
	{
		AudioPlaySpatial(AUDIO_TYPE_EXPLOSION_SFX,
			gameCenter.x,
			gameCenter.y,
			objectSoundFrom->position.x,
			objectSoundFrom->position.y);
		ParticleSpawn(PARTICLE_TYPE_EXPLOSION,
			objectSoundFrom->position.x - cameraOffset.x,
			objectSoundFrom->position.y - cameraOffset.y,
			PARTICLEDEF_LIFETIME, value);
		break;
	}
	case GE_PLAYER_SHOOT:
	{
		AudioPlay(AUDIO_TYPE_PLAYER_SHOOT_SFX);
		break;
	}
	case GE_PLAYER_MULTI:
	{
		AudioPlay(AUDIO_TYPE_PLAYER_MULTI_SFX);
		break;
	}
	case GE_PLAYER_HURT:
	{
		// PLAYER TAKE DAMAGE
		AudioPlay(AUDIO_TYPE_PLAYER_HIT_SFX);
		// Seems only particle has cameraoffset required problem
		ParticleSpawn(PARTICLE_TYPE_PLAYER_HIT,
			gameCenter.x - cameraOffset.x,
			gameCenter.y - cameraOffset.y,
			PARTICLEDEF_LIFETIME, value);
		break;
	}
	case GE_ENEMY_HURT:
	{
		// ENEMY TAKE DAMAGE
		AudioPlaySpatial(AUDIO_TYPE_ENEMY_HIT_SFX,
			gameCenter.x,
			gameCenter.y,
			objectSoundFrom->position.x,
			objectSoundFrom->position.y);
		ParticleSpawn(PARTICLE_TYPE_ENEMY_HIT,
			objectSoundFrom->position.x - cameraOffset.x,
			objectSoundFrom->position.y - cameraOffset.y,
			PARTICLEDEF_LIFETIME, value);
		break;
	}
	case GE_ENEMY_DIE:
	{
		// ENEMY DIE
		//AudioPlaySpatial(AUDIO_TYPE_ENEMY_DIE_SFX,
		//	gameCenter.x,
		//	gameCenter.y,
		//	objectSoundFrom->position.x,
		//	objectSoundFrom->position.y);
		ParticleSpawn(PARTICLE_TYPE_EXPLOSION,
			objectSoundFrom->position.x - cameraOffset.x,
			objectSoundFrom->position.y - cameraOffset.y,
			PARTICLEDEF_LIFETIME, value);
		break;
	}
	default:
		break;
	}
}

void GameUpdateHealthDisplay()
{
	int display = 0;

	if (player && player->active)
		display = (int)player->healthCurrent;

	sprintf_s(playerHealthDisplay, DISPLAY_ARR, "%ld", display);
}

GameObject* GameGetObject(GAMEOBJ_TYPE type)
{
	// GameObject array starts from 0 and goes to 200
	switch (type)
	{
	case OBJ_CLEAR:
	{
		break;
	}
	case OBJ_PLAYER:
	{
		if (player != &objects[0])
			return &objects[0];
	}
	case OBJ_ENEMY_PROJECTILE:
	case OBJ_ALLY_PROJECTILE:
	case OBJ_ALLY_MULTI_SHOT:
	case OBJ_ALLY_CIRCLING:
	{
		for (int i = 100; i < 159; ++i)
		{
			if (!objects[i].active)
				return &objects[i];
		}
		break;
	}
	case OBJ_ALLY_AOE:
	{
		if (!objects[159].active)
			return &objects[159];
		break;
	}
	case OBJ_ENEMY:
	case OBJ_ENEMY_ELITE:
	case OBJ_ENEMY_BOSS:
	{
		for (int i = 1; i < 100; ++i)
		{
			if (!objects[i].active)
				return &objects[i];
		}
		break;
	}
	case OBJ_OBSTACLE:
	{
		for (int i = 160; i < 200; ++i)
		{
			if (!objects[i].active)
				return &objects[i];
		}
		break;
	}
	default:
	{
		// printf("Tried to get an object with no type!\n");
		break;
	}
	}
	// printf("Not enough objects! %d\n", type);
	return NULL;
}
