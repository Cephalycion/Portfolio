/*!
@copyright	DIGIPEN INSTITUTE OF TECHNOLOGY SINGAPORE
@file       MainMenu.c
@author     Marc Alviz Evangelista(marcalviz.e),

			Gavin Ang Jun Liang(ang.g),
			Lau Jia Win (jiawin.lau)
@course     CSD1401 Software Engineering Project 1
@section    A
@tutorial   Final Project
@date       18/11/2023
@brief      This file contains the definitions of functions which renders the MainMenu UI
			to screen and also has the settings and persistant upgrades for the game.
*//*_________________________________________________________________________*/
#include <stdlib.h>
#include "MainMenu.h"

void MainMenuUpdateUIRes(int displayres)
{
	switch (displayres)
	{
	case RES_720:
	{
		for (int i = 0; i < sizeof(settingsBtns) / sizeof(settingsBtns[0]); ++i)
		{
			settingsBtns[i].btnScale = settingsBtns[i].btnScaleRes720;
			settingsBtns[i].btnPos = settingsBtns[i].btnPosRes720;
		}
		for (int i = 0; i < sizeof(mainMenuBtns) / sizeof(mainMenuBtns[0]); ++i)
		{
			mainMenuBtns[i].btnScale = mainMenuBtns[i].btnScaleRes720;
			mainMenuBtns[i].btnPos = mainMenuBtns[i].btnPosRes720;
		}
		for (int i = 0; i < sizeof(upgradesBtns) / sizeof(upgradesBtns[0]); ++i)
		{
			upgradesBtns[i].btnScale = upgradesBtns[i].btnScaleRes720;
			upgradesBtns[i].btnPos = upgradesBtns[i].btnPosRes720;
		}
		for (int i = 0; i < sizeof(creditsButtons) / sizeof(creditsButtons[0]); ++i)
		{
			creditsButtons[i].btnScale = creditsButtons[i].btnScaleRes720;
			creditsButtons[i].btnPos = creditsButtons[i].btnPosRes720;
		}

		backBtn.btnScale = backBtn.btnScaleRes720;
		backBtn.btnPos = backBtn.btnPosRes720;

		audioBarActiveIcon.btnScale = audioBarActiveIcon.btnScaleRes720;
		audioBarActiveIcon.btnPos = audioBarActiveIcon.btnPosRes720;
		audioBarInactiveIcon.btnScale = audioBarActiveIcon.btnScaleRes720;
		audioBarInactiveIcon.btnPos = audioBarActiveIcon.btnPosRes720;

		settingsBackdrop.btnScale = settingsBackdrop.btnScaleRes720;
		settingsBackdrop.btnPos = settingsBackdrop.btnPosRes720;

		upgradesBarActiveIcon.btnScale = upgradesBarActiveIcon.btnScaleRes720;
		upgradesBarActiveIcon.btnPos = upgradesBarActiveIcon.btnPosRes720;
		upgradesBarInactiveIcon.btnScale = upgradesBarInactiveIcon.btnScaleRes720;
		upgradesBarInactiveIcon.btnPos = upgradesBarInactiveIcon.btnPosRes720;
		upgradesBarPendingIcon.btnScale = upgradesBarPendingIcon.btnScaleRes720;
		upgradesBarPendingIcon.btnPos = upgradesBarPendingIcon.btnPosRes720;

		upgradesBackdrop.btnScale = upgradesBackdrop.btnScaleRes720;
		upgradesBackdrop.btnPos = upgradesBackdrop.btnPosRes720;

		mainMenuPanel.btnScale = mainMenuPanel.btnScaleRes720;
		mainMenuPanel.btnPos = mainMenuPanel.btnPosRes720;

		mainMenuGameTitle.btnScale = mainMenuGameTitle.btnScaleRes720;
		mainMenuGameTitle.btnPos = mainMenuGameTitle.btnPosRes720;

		mainMenuBackdrop.btnScale = mainMenuBackdrop.btnScaleRes720;
		mainMenuBackdrop.btnPos = mainMenuBackdrop.btnPosRes720;

		creditsBackdrop.btnScale = creditsBackdrop.btnScaleRes720;
		creditsBackdrop.btnPos = creditsBackdrop.btnPosRes720;

		nextPageBtn.btnScale = nextPageBtn.btnScaleRes720;
		nextPageBtn.btnPos = nextPageBtn.btnPosRes720;

		prevPageBtn.btnScale = prevPageBtn.btnScaleRes720;
		prevPageBtn.btnPos = prevPageBtn.btnPosRes720;

		creditsPage2.btnScale = creditsPage2.btnScaleRes720;
		creditsPage2.btnPos = creditsPage2.btnPosRes720;
		creditsPage3.btnScale = creditsPage3.btnScaleRes720;
		creditsPage3.btnPos = creditsPage3.btnPosRes720;

		MenuStarParticleSetBorder(1280, 720);
		break;
	}
	case RES_1080:
	{
		for (int i = 0; i < sizeof(settingsBtns) / sizeof(settingsBtns[0]); ++i)
		{
			settingsBtns[i].btnScale = settingsBtns[i].btnScaleRes1080;
			settingsBtns[i].btnPos = settingsBtns[i].btnPosRes1080;
		}
		for (int i = 0; i < sizeof(mainMenuBtns) / sizeof(mainMenuBtns[0]); ++i)
		{
			mainMenuBtns[i].btnScale = mainMenuBtns[i].btnScaleRes1080;
			mainMenuBtns[i].btnPos = mainMenuBtns[i].btnPosRes1080;
		}
		for (int i = 0; i < sizeof(upgradesBtns) / sizeof(upgradesBtns[0]); ++i)
		{
			upgradesBtns[i].btnScale = upgradesBtns[i].btnScaleRes1080;
			upgradesBtns[i].btnPos = upgradesBtns[i].btnPosRes1080;
		}
		for (int i = 0; i < sizeof(creditsButtons) / sizeof(creditsButtons[0]); ++i)
		{
			creditsButtons[i].btnScale = creditsButtons[i].btnScaleRes1080;
			creditsButtons[i].btnPos = creditsButtons[i].btnPosRes1080;
		}

		backBtn.btnScale = backBtn.btnScaleRes1080;
		backBtn.btnPos = backBtn.btnPosRes1080;

		settingsBackdrop.btnScale = settingsBackdrop.btnScaleRes1080;
		settingsBackdrop.btnPos = settingsBackdrop.btnPosRes1080;

		audioBarActiveIcon.btnScale = audioBarActiveIcon.btnScaleRes1080;
		audioBarActiveIcon.btnPos = audioBarActiveIcon.btnPosRes1080;
		audioBarInactiveIcon.btnScale = audioBarActiveIcon.btnScaleRes1080;
		audioBarInactiveIcon.btnPos = audioBarActiveIcon.btnPosRes1080;

		upgradesBarActiveIcon.btnScale = upgradesBarActiveIcon.btnScaleRes1080;
		upgradesBarActiveIcon.btnPos = upgradesBarActiveIcon.btnPosRes1080;
		upgradesBarInactiveIcon.btnScale = upgradesBarInactiveIcon.btnScaleRes1080;
		upgradesBarInactiveIcon.btnPos = upgradesBarInactiveIcon.btnPosRes1080;
		upgradesBarPendingIcon.btnScale = upgradesBarPendingIcon.btnScaleRes1080;
		upgradesBarPendingIcon.btnPos = upgradesBarPendingIcon.btnPosRes1080;

		upgradesBackdrop.btnScale = upgradesBackdrop.btnScaleRes1080;
		upgradesBackdrop.btnPos = upgradesBackdrop.btnPosRes1080;

		mainMenuPanel.btnScale = mainMenuPanel.btnScaleRes1080;
		mainMenuPanel.btnPos = mainMenuPanel.btnPosRes1080;

		mainMenuGameTitle.btnScale = mainMenuGameTitle.btnScaleRes1080;
		mainMenuGameTitle.btnPos = mainMenuGameTitle.btnPosRes1080;

		mainMenuBackdrop.btnScale = mainMenuBackdrop.btnScaleRes1080;
		mainMenuBackdrop.btnPos = mainMenuBackdrop.btnPosRes1080;

		creditsBackdrop.btnScale = creditsBackdrop.btnScaleRes1080;
		creditsBackdrop.btnPos = creditsBackdrop.btnPosRes1080;

		nextPageBtn.btnScale = nextPageBtn.btnScaleRes1080;
		nextPageBtn.btnPos = nextPageBtn.btnPosRes1080;

		prevPageBtn.btnScale = prevPageBtn.btnScaleRes1080;
		prevPageBtn.btnPos = prevPageBtn.btnPosRes1080;

		creditsPage2.btnScale = creditsPage2.btnScaleRes1080;
		creditsPage2.btnPos = creditsPage2.btnPosRes1080;
		creditsPage3.btnScale = creditsPage3.btnScaleRes1080;
		creditsPage3.btnPos = creditsPage3.btnPosRes1080;

		MenuStarParticleSetBorder(1920, 1080);
		break;
	}
	case RES_FULLSCREEN:
	{
		for (int i = 0; i < sizeof(settingsBtns) / sizeof(settingsBtns[0]); ++i)
		{
			settingsBtns[i].btnScale = settingsBtns[i].btnScaleResFullscreen;
			settingsBtns[i].btnPos = settingsBtns[i].btnPosResFullscreen;
		}
		for (int i = 0; i < sizeof(mainMenuBtns) / sizeof(mainMenuBtns[0]); ++i)
		{
			mainMenuBtns[i].btnScale = mainMenuBtns[i].btnScaleResFullscreen;
			mainMenuBtns[i].btnPos = mainMenuBtns[i].btnPosResFullscreen;
		}
		for (int i = 0; i < sizeof(upgradesBtns) / sizeof(upgradesBtns[0]); ++i)
		{
			upgradesBtns[i].btnScale = upgradesBtns[i].btnScaleResFullscreen;
			upgradesBtns[i].btnPos = upgradesBtns[i].btnPosResFullscreen;
		}
		for (int i = 0; i < sizeof(creditsButtons) / sizeof(creditsButtons[0]); ++i)
		{
			creditsButtons[i].btnScale = creditsButtons[i].btnScaleResFullscreen;
			creditsButtons[i].btnPos = creditsButtons[i].btnPosResFullscreen;
		}

		backBtn.btnScale = backBtn.btnScaleResFullscreen;
		backBtn.btnPos = backBtn.btnPosResFullscreen;

		settingsBackdrop.btnScale = settingsBackdrop.btnScaleResFullscreen;
		settingsBackdrop.btnPos = settingsBackdrop.btnPosResFullscreen;

		audioBarActiveIcon.btnScale = audioBarActiveIcon.btnScaleResFullscreen;
		audioBarActiveIcon.btnPos = audioBarActiveIcon.btnPosResFullscreen;
		audioBarInactiveIcon.btnScale = audioBarActiveIcon.btnScaleResFullscreen;
		audioBarInactiveIcon.btnPos = audioBarActiveIcon.btnPosResFullscreen;

		upgradesBarActiveIcon.btnScale = upgradesBarActiveIcon.btnScaleResFullscreen;
		upgradesBarActiveIcon.btnPos = upgradesBarActiveIcon.btnPosResFullscreen;
		upgradesBarInactiveIcon.btnScale = upgradesBarInactiveIcon.btnScaleResFullscreen;
		upgradesBarInactiveIcon.btnPos = upgradesBarInactiveIcon.btnPosResFullscreen;
		upgradesBarPendingIcon.btnScale = upgradesBarPendingIcon.btnScaleResFullscreen;
		upgradesBarPendingIcon.btnPos = upgradesBarPendingIcon.btnPosResFullscreen;

		upgradesBackdrop.btnScale = upgradesBackdrop.btnScaleResFullscreen;
		upgradesBackdrop.btnPos = upgradesBackdrop.btnPosResFullscreen;

		mainMenuPanel.btnScale = mainMenuPanel.btnScaleResFullscreen;
		mainMenuPanel.btnPos = mainMenuPanel.btnPosResFullscreen;

		mainMenuGameTitle.btnScale = mainMenuGameTitle.btnScaleResFullscreen;
		mainMenuGameTitle.btnPos = mainMenuGameTitle.btnPosResFullscreen;
		
		mainMenuBackdrop.btnScale = mainMenuBackdrop.btnScaleResFullscreen;
		mainMenuBackdrop.btnPos = mainMenuBackdrop.btnPosResFullscreen;

		creditsBackdrop.btnScale = creditsBackdrop.btnScaleResFullscreen;
		creditsBackdrop.btnPos = creditsBackdrop.btnPosResFullscreen;

		nextPageBtn.btnScale = nextPageBtn.btnScaleResFullscreen;
		nextPageBtn.btnPos = nextPageBtn.btnPosResFullscreen;

		prevPageBtn.btnScale = prevPageBtn.btnScaleResFullscreen;
		prevPageBtn.btnPos = prevPageBtn.btnPosResFullscreen;

		creditsPage2.btnScale = creditsPage2.btnScaleResFullscreen;
		creditsPage2.btnPos = creditsPage2.btnPosResFullscreen;
		creditsPage3.btnScale = creditsPage3.btnScaleResFullscreen;
		creditsPage3.btnPos = creditsPage3.btnPosResFullscreen;

		MenuStarParticleSetBorder(CP_System_GetDisplayWidth(), CP_System_GetDisplayHeight());
		break;
	}
	default:
		break;
	}

}

void MainMenuDraw(void)
{
	ButtonDraw(mainMenuGameTitle, 0.f);
	ButtonDraw(mainMenuPanel, 0.f);
	for (int i = 0; i < sizeof(mainMenuBtns) / sizeof(mainMenuBtns[0]); ++i)
	{
		ButtonDraw(mainMenuBtns[i], 0.f);
	}
}

void MainMenuCheckClicks(void)
{
	if (CP_Input_MouseClicked())
	{
		for (int i = 0; i < sizeof(mainMenuBtns) / sizeof(mainMenuBtns[0]); ++i)
		{
			if (ButtonCheckClick(&mainMenuBtns[i], CP_Vector_Set(CP_Input_GetMouseWorldX(), CP_Input_GetMouseWorldY())))
			{
				switch (i)
				{
				case 0:
				{
					// Go to game scene
					isQuittingToGame = true;

					SetMTG(true);
					//CP_Engine_SetNextGameState(GameInit, GameUpdate, GameExit);
					break;
				}
				case 1:
				{
					state = UPGRADES;
					GameUpdateCurrencyDisplay();
					GameUpdateCurrencySpentDisplay();

					break;
				}
				case 2:
				{
					state = SETTINGS;
					break;
				}
				case 3:
				{
					state = CREDITS;
					break;
				}
				case 4:
				{
					// Freeing audio and particle done at MainMenuExit()
					// as it takes into account closing the window using X button
					//AudioQuitGame();	// Free audio files
					//ParticleQuitGame();		// Free particle files
					CP_Engine_Terminate();
					break;
				}
				default:
					break;
				}
				AudioMenuAnyButtonClicked();
			}
		}

	}

	for (int i = 0; i < sizeof(mainMenuBtns) / sizeof(mainMenuBtns[0]); ++i)
	{
		if (ButtonCheckHover(&mainMenuBtns[i], CP_Vector_Set(CP_Input_GetMouseWorldX(), CP_Input_GetMouseWorldY())))
		{
			mainMenuBtns[i].btnHovering = true;
		}
		else
		{
			mainMenuBtns[i].btnHovering = false;
		}
	}

}


void MainMenuDrawSettings(void)
{
	ButtonDraw(settingsBackdrop, 0.f);
	for (int i = 0; i < (sizeof(settingsBtns) / sizeof(settingsBtns[0])); ++i)
	{
		ButtonDraw(settingsBtns[i], 0.f);
	}

	for (float i = (float)(CP_System_GetWindowWidth() / 2.f) - ((float)CP_Image_GetWidth(audioBarActiveIcon.btnImage) * audioBarActiveIcon.btnScale.x) * 3.35f, j = 0;
		j < 10; i += ((float)CP_Image_GetWidth(audioBarActiveIcon.btnImage) * audioBarActiveIcon.btnScale.x) * 0.75f, ++j)
	{
		if (j < audioMasterVolumeActiveBars)
		{
			CP_Image_Draw(audioBarActiveIcon.btnImage, i, audioBarActiveIcon.btnPos.y, (float)CP_Image_GetWidth(audioBarActiveIcon.btnImage) * audioBarActiveIcon.btnScale.x, (float)CP_Image_GetHeight(audioBarActiveIcon.btnImage) * audioBarActiveIcon.btnScale.y, 255);
		}
		else
		{
			CP_Image_Draw(audioBarInactiveIcon.btnImage, i, audioBarInactiveIcon.btnPos.y, (float)CP_Image_GetWidth(audioBarInactiveIcon.btnImage) * audioBarInactiveIcon.btnScale.x, (float)CP_Image_GetHeight(audioBarInactiveIcon.btnImage) * audioBarInactiveIcon.btnScale.y, 255);
		}
	}


	ButtonDraw(backBtn, 0.f);
}

void MainMenuCheckSettingsClicks(void)
{
	if (CP_Input_MouseClicked())
	{
		bool breakOut = false;
		for (int i = 0; i < sizeof(settingsBtns) / sizeof(settingsBtns[0]); ++i)
		{
			if (breakOut)
			{
				break;
			}
			if (settingsBtns[i].btnInteractable)
			{
				if (ButtonCheckClick(&settingsBtns[i], CP_Vector_Set(CP_Input_GetMouseWorldX(), CP_Input_GetMouseWorldY())))
				{
					switch (i)
					{
					case 0: // decrease audio
					{
						if (audioMasterVolumeActiveBars > 0)
						{
							--audioMasterVolumeActiveBars;
							// Set audio to lower
							AudioDecreaseVolume(AUDIO_GROUP_MASTER);
							// Play some audio
							//AudioPlay(AUDIO_TYPE_MENU_POP_UIFX);
							// Save to save file here or when exit settings menu
							WriteAudioTextFile(AudioGetVolume(AUDIO_GROUP_MASTER));
							breakOut = true;
						}
						break;
					}
					case 1:
					{
						if (audioMasterVolumeActiveBars < 10)
						{
							++audioMasterVolumeActiveBars;

							AudioIncreaseVolume(AUDIO_GROUP_MASTER);
							//AudioPlay(AUDIO_TYPE_MENU_POP_UIFX);
							// Save to save file here or when exit settings menu
							WriteAudioTextFile(AudioGetVolume(AUDIO_GROUP_MASTER));
							breakOut = true;
						}
						break;
					}
					case 2:
					{
						// Update buttons here
						if (GetDisplay() != RES_720)
						{
							SetDisplay(RES_720);
							MainMenuUpdateUIRes(GetDisplay());

							CP_System_SetWindowSize(1280, 720);
							breakOut = true;
						}
						break;
					}
					case 3:
					{
						if (GetDisplay() != RES_1080)
						{
							SetDisplay(RES_1080);
							MainMenuUpdateUIRes(GetDisplay());

							CP_System_SetWindowSize(1920, 1080);
							breakOut = true;
						}
					}
						break;
					case 4:
					{
						if (GetDisplay() != RES_FULLSCREEN)
						{
							SetDisplay(RES_FULLSCREEN);
							MainMenuUpdateUIRes(GetDisplay());

							CP_System_Fullscreen();
							breakOut = true;
						}
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

		if (ButtonCheckClick(&backBtn, CP_Vector_Set(CP_Input_GetMouseWorldX(), CP_Input_GetMouseWorldY())))
		{
			AudioMenuBackButtonClicked();
			state = MAIN;
		}
	}


	for (int i = 0; i < sizeof(settingsBtns) / sizeof(settingsBtns[0]); ++i)
	{
		if (settingsBtns[i].btnInteractable)
		{
			if (ButtonCheckHover(&settingsBtns[i], CP_Vector_Set(CP_Input_GetMouseWorldX(), CP_Input_GetMouseWorldY())))
			{
				settingsBtns[i].btnHovering = true;
			}
			else
			{
				settingsBtns[i].btnHovering = false;
			}
		}
	}

	if (ButtonCheckHover(&backBtn, CP_Vector_Set(CP_Input_GetMouseWorldX(), CP_Input_GetMouseWorldY())))
	{
		backBtn.btnHovering = true;
	}
	else
	{
		backBtn.btnHovering = false;
	}
}

void MainMenuDrawUpgrades(void)
{
	ButtonDraw(upgradesBackdrop, 0.f);
	for (int i = 0; i < sizeof(upgradesBtns) / sizeof(upgradesBtns[0]); ++i)
	{
		ButtonDraw(upgradesBtns[i], 0.f);
	}

	float scaledBarActiveWidth = (float)CP_Image_GetWidth(upgradesBarActiveIcon.btnImage) * upgradesBarActiveIcon.btnScale.x;
	float scaledBarInactiveWidth = (float)CP_Image_GetWidth(upgradesBarInactiveIcon.btnImage) * upgradesBarInactiveIcon.btnScale.x;

	float scaledBarActiveHeight = (float)CP_Image_GetHeight(upgradesBarActiveIcon.btnImage) * upgradesBarActiveIcon.btnScale.y;
	float scaledBarInactiveHeight = (float)CP_Image_GetHeight(upgradesBarInactiveIcon.btnImage) * upgradesBarInactiveIcon.btnScale.y;

	for (float i = ((float)CP_System_GetWindowWidth() / 2.f) - (2.f * scaledBarActiveWidth),
		j = 0; j < 5; i += scaledBarActiveWidth, ++j)
	{
		if (j < upgradesCurrentAttack)
		{
			CP_Image_Draw(upgradesBarActiveIcon.btnImage, i, upgradesBarActiveIcon.btnPos.y,
				scaledBarActiveWidth, scaledBarActiveHeight,
				255);
		}
		else
		{
			CP_Image_Draw(upgradesBarInactiveIcon.btnImage, i, upgradesBarActiveIcon.btnPos.y,
				scaledBarInactiveWidth, scaledBarInactiveHeight,
				255);
		}
	}

	for (float i = ((float)CP_System_GetWindowWidth() / 2.f) - (14.f * scaledBarActiveWidth),
		j = 0; j < 5; i += scaledBarActiveWidth, ++j)
	{
		if (j < upgradesCurrentHP)
		{
			CP_Image_Draw(upgradesBarActiveIcon.btnImage, i, upgradesBarActiveIcon.btnPos.y,
				scaledBarActiveWidth, scaledBarActiveHeight,
				255);
		}
		else
		{
			CP_Image_Draw(upgradesBarInactiveIcon.btnImage, i, upgradesBarActiveIcon.btnPos.y,
				scaledBarInactiveWidth, scaledBarInactiveHeight,
				255);
		}
	}
	
	for (float i = ((float)CP_System_GetWindowWidth() / 2.f) + (10.f * scaledBarActiveWidth),
		j = 0; j < 5; i += scaledBarActiveWidth, ++j)
	{
		if (j < upgradesCurrentSpeed)
		{
			CP_Image_Draw(upgradesBarActiveIcon.btnImage, i, upgradesBarActiveIcon.btnPos.y,
				scaledBarActiveWidth, scaledBarActiveHeight,
				255);
		}
		else
		{
			CP_Image_Draw(upgradesBarInactiveIcon.btnImage, i, upgradesBarActiveIcon.btnPos.y,
				scaledBarInactiveWidth, scaledBarInactiveHeight,
				255);
		}
	}

	if (upgradesCurrentAttack < 5)
	{
		for (float i = ((float)CP_System_GetWindowWidth() / 2.f) - (2.f * scaledBarActiveWidth),
			j = 0; j < 5; i += scaledBarActiveWidth, ++j)
		{
			if (j < upgradesCurrentAttack)
			{
				continue;
			}
			if (j < upgradesAttackToAdd + upgradesCurrentAttack)
			{
				CP_Image_Draw(upgradesBarPendingIcon.btnImage, i, upgradesBarActiveIcon.btnPos.y,
					scaledBarActiveWidth, scaledBarActiveHeight,
					255);
			}
		
		}
	}
	if (upgradesCurrentAttack < 5)
	{
		for (float i = ((float)CP_System_GetWindowWidth() / 2.f) - (2.f * scaledBarActiveWidth),
			j = 0; j < 5; i += scaledBarActiveWidth, ++j)
		{
			if (j < upgradesCurrentAttack)
			{
				continue;
			}
			if (j < upgradesAttackToAdd + upgradesCurrentAttack)
			{
				CP_Image_Draw(upgradesBarPendingIcon.btnImage, i, upgradesBarActiveIcon.btnPos.y,
					scaledBarActiveWidth, scaledBarActiveHeight,
					255);
			}

		}
	}
	if (upgradesCurrentHP < 5)
	{
		for (float i = ((float)CP_System_GetWindowWidth() / 2.f) - (14.f * scaledBarActiveWidth),
			j = 0; j < 5; i += scaledBarActiveWidth, ++j)
		{
			if (j < upgradesCurrentHP)
			{
				continue;
			}
			if (j < upgradesHPToAdd + upgradesCurrentHP)
			{
				CP_Image_Draw(upgradesBarPendingIcon.btnImage, i, upgradesBarActiveIcon.btnPos.y,
					scaledBarActiveWidth, scaledBarActiveHeight,
					255);
			}
		}
	}
	if (upgradesCurrentSpeed < 5)
	{
		for (float i = ((float)CP_System_GetWindowWidth() / 2.f) + (10.f * scaledBarActiveWidth),
			j = 0; j < 5; i += scaledBarActiveWidth, ++j)
		{
			if (j < upgradesCurrentSpeed)
			{
				continue;
			}
			if (j < upgradesSpeedToAdd + upgradesCurrentSpeed)
			{
				CP_Image_Draw(upgradesBarPendingIcon.btnImage, i, upgradesBarActiveIcon.btnPos.y,
					scaledBarActiveWidth, scaledBarActiveHeight,
					255);
			}
		}
	}

	ButtonDraw(backBtn, 0.f);

}

void MainMenuCheckUpgradesClicks(void)
{
	if (CP_Input_MouseClicked())
	{
		for (int i = 0; i < sizeof(upgradesBtns) / sizeof(upgradesBtns[0]); ++i)
		{
			if (upgradesBtns[i].btnInteractable)
			{
				// 3/4 == atk--/atk-- | 5/6 == hp--/hp++ | 7/8 == spd--/spd++
				if (ButtonCheckClick(&upgradesBtns[i], CP_Vector_Set(CP_Input_GetMouseWorldX(), CP_Input_GetMouseWorldY())))
				{
					switch (i)
					{
					case 3:
					{
						if (upgradesAttackToAdd > 0)
						{
							--upgradesAttackToAdd;
							//decrease the total count
							--upgradeCount;
							//recalculate the cost
							CalculateCost(upgradeCount);
							//update the currency used display
							GameUpdateCurrencySpentDisplay();
						}
						break;
					}
					case 4:
					{
						if (upgradesCurrentAttack < 5)
						{
							if (upgradesAttackToAdd < 5 - upgradesCurrentAttack)
							{
								//increase the total count
								++upgradeCount;
								//recalculate the cost
								CalculateCost(upgradeCount);
								if (upgradeCost <= currencyTemp)
								{
									++upgradesAttackToAdd;
									//recalculate the cost
									CalculateCost(upgradeCount);
									//update the currency used display
									GameUpdateCurrencySpentDisplay();
								}
								else
								{
									//decrease the total count
									--upgradeCount;
									//recalculate the cost
									CalculateCost(upgradeCount);
									//update the currency used display
									GameUpdateCurrencySpentDisplay();
								}
							}
						}
						break;
					}
					case 5:
					{
						if (upgradesHPToAdd > 0)
						{
							--upgradesHPToAdd;
							//decrease the total count
							--upgradeCount;
							//recalculate the cost
							CalculateCost(upgradeCount);
							//update the currency used display
							GameUpdateCurrencySpentDisplay();
						}
						break;
					}
					case 6:
					{
						if (upgradesCurrentHP < 5)
						{
							if (upgradesHPToAdd < 5 - upgradesCurrentHP)
							{
								//increase the total count
								++upgradeCount;
								//recalculate the cost
								CalculateCost(upgradeCount);
								if (upgradeCost <= currencyTemp)
								{
									++upgradesHPToAdd;
									CalculateCost(upgradeCount);
									//update the currency used display
									GameUpdateCurrencySpentDisplay();
								}
								else
								{
									//decrease the total count
									--upgradeCount;
									//recalculate the cost
									CalculateCost(upgradeCount);
									//update the currency used display
									GameUpdateCurrencySpentDisplay();
								}
							}
						}
						break;
					}
					case 7:
					{
						if (upgradesSpeedToAdd > 0)
						{
							--upgradesSpeedToAdd;
							//decrease the total count
							--upgradeCount;
							//recalculate the cost
							CalculateCost(upgradeCount);
							//update the currency used display
							GameUpdateCurrencySpentDisplay();
						}
						break;
					}
					case 8:
					{
						if (upgradesCurrentSpeed < 5)
						{
							if (upgradesSpeedToAdd < 5 - upgradesCurrentSpeed)
							{
								//increase the total count
								++upgradeCount;
								//recalculate the cost
								CalculateCost(upgradeCount);
								if (upgradeCost <= currencyTemp)
								{
									//increase the total count
									++upgradesSpeedToAdd;
									//recalculate the cost
									CalculateCost(upgradeCount);
									//update the currency used display
									GameUpdateCurrencySpentDisplay();
								}
								else
								{
									//decrease the total count
									--upgradeCount;
									//recalculate the cost
									CalculateCost(upgradeCount);
									//update the currency used display
									GameUpdateCurrencySpentDisplay();
								}

							}
						}
						break;
					}
					case 9:
					{
						//resets the all the counter to 0
						upgradesCurrentHP = 0;
						upgradesHPToAdd = 0;
						upgradesCurrentAttack = 0;
						upgradesAttackToAdd = 0;
						upgradesCurrentSpeed = 0;
						upgradesSpeedToAdd = 0;
						
						//apply the resets in the files
						ResetButton();
						//update the total currency display
						GameUpdateCurrencyDisplay();
						upgradeCount = 0;
						//update the currency used display
						GameUpdateCurrencySpentDisplay();
						break;
					}
					case 10:
					{
						if (upgradesAttackToAdd > 0 || upgradesHPToAdd > 0 || upgradesSpeedToAdd > 0)
						{
							upgradesCurrentAttack += upgradesAttackToAdd;
							upgradesAttackToAdd = 0;
							upgradesCurrentHP += upgradesHPToAdd;
							upgradesHPToAdd = 0;
							upgradesCurrentSpeed += upgradesSpeedToAdd;
							upgradesSpeedToAdd = 0;

							// Write to file here
							AddPlayerHealth(upgradesCurrentHP);
							AddPlayerAttack(upgradesCurrentAttack);
							AddPlayerSpeed(upgradesCurrentSpeed);
							ConfirmButton();
							//update the total currency display
							GameUpdateCurrencyDisplay();
							upgradeCount = 0;
							//update the currency used display
							GameUpdateCurrencySpentDisplay();
						}
						break;
					}
					default:
						break;
					}
					AudioMenuAnyButtonClicked();
				}
			}
		}
		if (ButtonCheckClick(&backBtn, CP_Vector_Set(CP_Input_GetMouseWorldX(), CP_Input_GetMouseWorldY())))
		{
			AudioMenuBackButtonClicked();
			upgradesAttackToAdd = 0;
			upgradesHPToAdd = 0;
			upgradesSpeedToAdd = 0;
			upgradeCount = 0;

			GameUpdateCurrencySpentDisplay();
			GameUpdateCurrencyDisplay();
			state = MAIN;
		}
	}
	for (int i = 0; i < sizeof(upgradesBtns) / sizeof(upgradesBtns[0]); ++i)
	{
		if (upgradesBtns[i].btnInteractable && ButtonCheckHover(&upgradesBtns[i], CP_Vector_Set(CP_Input_GetMouseWorldX(), CP_Input_GetMouseWorldY())))
		{
			upgradesBtns[i].btnHovering = true;
		}
		else
		{
			upgradesBtns[i].btnHovering = false;
		}
	}
	if (ButtonCheckHover(&backBtn, CP_Vector_Set(CP_Input_GetMouseWorldX(), CP_Input_GetMouseWorldY())))
	{
		backBtn.btnHovering = true;
	}
	else
	{
		backBtn.btnHovering = false;
	}
}

void MainMenuDrawCredits(void)
{
	if (currentPage == 1)
	{
		ButtonDraw(creditsBackdrop, 0.f);
	}
	switch (currentPage)
	{
	case 1: // Team members
	{

		for (int i = 0; i < 5; ++i)
		{
			ButtonDraw(creditsButtons[i], 0.f);
		}
		for (int i = 0; i < 5; ++i)
		{
			if (creditsButtons[i].btnHovering)
			{
				switch (i)
				{
				case 0:
				{
					CP_Image_Draw(gavin.btnImage,
						CP_Input_GetMouseWorldX() + 0.5f * (CP_Image_GetWidth(gavin.btnImage) * gavin.btnScale.x),
						CP_Input_GetMouseWorldY() - 0.5f * (CP_Image_GetHeight(gavin.btnImage) * gavin.btnScale.y),
						CP_Image_GetWidth(gavin.btnImage) * gavin.btnScale.x, CP_Image_GetHeight(gavin.btnImage) * gavin.btnScale.y,
						255);
					break;
				}
				case 1:
				{
					CP_Image_Draw(jiawin.btnImage,
						CP_Input_GetMouseWorldX() + 0.5f * (CP_Image_GetWidth(jiawin.btnImage) * jiawin.btnScale.x),
						CP_Input_GetMouseWorldY() - 0.5f * (CP_Image_GetHeight(jiawin.btnImage) * jiawin.btnScale.y),
						CP_Image_GetWidth(jiawin.btnImage) * jiawin.btnScale.x, CP_Image_GetHeight(jiawin.btnImage) * jiawin.btnScale.y,
						255);
					break;
				}
				case 2:
				{
					CP_Image_Draw(leonard.btnImage,
						CP_Input_GetMouseWorldX() + 0.5f * (CP_Image_GetWidth(leonard.btnImage) * leonard.btnScale.x),
						CP_Input_GetMouseWorldY() - 0.5f * (CP_Image_GetHeight(leonard.btnImage) * leonard.btnScale.y),
						CP_Image_GetWidth(leonard.btnImage) * leonard.btnScale.x, CP_Image_GetHeight(leonard.btnImage) * leonard.btnScale.y,
						255);
					break;
				}
				case 3:
				{
					CP_Image_Draw(ryan.btnImage,
						CP_Input_GetMouseWorldX() + 0.5f * (CP_Image_GetWidth(ryan.btnImage) * ryan.btnScale.x),
						CP_Input_GetMouseWorldY() - 0.5f * (CP_Image_GetHeight(ryan.btnImage) * ryan.btnScale.y),
						CP_Image_GetWidth(ryan.btnImage) * ryan.btnScale.x, CP_Image_GetHeight(ryan.btnImage) * ryan.btnScale.y,
						255);
					break;
				}
				case 4:
				{
					CP_Image_Draw(marc.btnImage,
						CP_Input_GetMouseWorldX() + 0.5f * (CP_Image_GetWidth(marc.btnImage) * marc.btnScale.x),
						CP_Input_GetMouseWorldY() - 0.5f * (CP_Image_GetHeight(marc.btnImage) * marc.btnScale.y),
						CP_Image_GetWidth(marc.btnImage) * marc.btnScale.x, CP_Image_GetHeight(marc.btnImage) * marc.btnScale.y,
						255);
					break;
				}
				default:
					break;
				}
			}
		}
		ButtonDraw(backBtn, 0.f);
		break;
	}
	case 2: // Presidents/Execs
	{
		ButtonDraw(creditsPage2, 0.f);
		break;
	}
	case 3: // Made at Digipen/Instructors
	{
		ButtonDraw(creditsPage3, 0.f);
		break;
	}
	default:
		break;
	}

	if (currentPage < numPages)
	{
		ButtonDraw(nextPageBtn, 0.f);
	}
	if (currentPage > 1)
	{
		ButtonDraw(prevPageBtn, 0.f);
	}

}

void MainMenuCheckCreditsClicks(void)
{
	if (CP_Input_MouseClicked())
	{
		if (currentPage == 1 && ButtonCheckClick(&backBtn, CP_Vector_Set(CP_Input_GetMouseWorldX(), CP_Input_GetMouseWorldY())))
		{
			state = MAIN;
			AudioMenuBackButtonClicked();
		}
		if (currentPage < numPages)
		{
			if (ButtonCheckClick(&nextPageBtn, CP_Vector_Set(CP_Input_GetMouseWorldX(), CP_Input_GetMouseWorldY())))
			{
				++currentPage;
				AudioMenuAnyButtonClicked();
			}
		}
		if (currentPage > 1)
		{
			if (ButtonCheckClick(&prevPageBtn, CP_Vector_Set(CP_Input_GetMouseWorldX(), CP_Input_GetMouseWorldY())))
			{
				--currentPage;
				AudioMenuAnyButtonClicked();
			}
		}
	}
	for (int i = 0; i < 5; ++i)
	{
		if (creditsButtons[i].btnInteractable && ButtonCheckHover(&creditsButtons[i], CP_Vector_Set(CP_Input_GetMouseWorldX(), CP_Input_GetMouseWorldY())))
		{
			creditsButtons[i].btnHovering = true;
		}
		else
		{
			creditsButtons[i].btnHovering = false;
		}
	}
	if (ButtonCheckHover(&backBtn, CP_Vector_Set(CP_Input_GetMouseWorldX(), CP_Input_GetMouseWorldY())))
	{
		backBtn.btnHovering = true;
	}
	else
	{
		backBtn.btnHovering = false;
	}
	if (ButtonCheckHover(&nextPageBtn, CP_Vector_Set(CP_Input_GetMouseWorldX(), CP_Input_GetMouseWorldY())))
	{
		nextPageBtn.btnHovering = true;
	}
	else
	{
		nextPageBtn.btnHovering = false;
	}
	if (ButtonCheckHover(&prevPageBtn, CP_Vector_Set(CP_Input_GetMouseWorldX(), CP_Input_GetMouseWorldY())))
	{
		prevPageBtn.btnHovering = true;
	}
	else
	{
		prevPageBtn.btnHovering = false;
	}
}


void MainMenuInitButtons(void)
{
	ButtonInit(&mainMenuGameTitle, CP_Vector_Set(1920.f / 2.f, (1080.f / 10.f) * 2.f), CP_Vector_Set(1.f, 1.f), 255, "gameTitle.png", false, "", false, "", false);
	ButtonInit(&mainMenuPanel, CP_Vector_Set(1920.f / 2.f, (1080.f / 10.f) * 7.f), CP_Vector_Set(1.f, 1.f), 255, "MainMenuPanel.png", false, "", false, "", false);
	ButtonInit(&mainMenuBackdrop, CP_Vector_Set(1920.f / 2.f, 1080.f / 2.f), CP_Vector_Set(1.f, 1.f), 255, "Background.png", false, "", false, "", false);
	// init and set pos and scale relative to screen size
	// Change the height to be relative to resolution size latur
	ButtonInit(&settingsBtn, CP_Vector_Set(1920.f / 2.f, mainMenuPanel.btnPos.y), CP_Vector_Set(1.f, 1.f), 255, "settingsBtn.png", true, "", false, "", false);
	ButtonInit(&playBtn,	 CP_Vector_Set(1920.f / 2.f, mainMenuPanel.btnPos.y - ((float)CP_Image_GetHeight(settingsBtn.btnImage) * settingsBtn.btnScale.y * 2.f)),
		CP_Vector_Set(1.f, 1.f), 255, "playBtn.png", true, "", false, "", false);
	ButtonInit(&upgradesBtn, CP_Vector_Set(1920.f / 2.f, mainMenuPanel.btnPos.y - ((float)CP_Image_GetHeight(settingsBtn.btnImage) * settingsBtn.btnScale.y)),
		CP_Vector_Set(1.f, 1.f), 255, "upgradesBtn.png", true, "", false, "", false);
	ButtonInit(&creditsBtn,	 CP_Vector_Set(1920.f / 2.f, mainMenuPanel.btnPos.y + ((float)CP_Image_GetHeight(settingsBtn.btnImage) * settingsBtn.btnScale.y)),
		CP_Vector_Set(1.f, 1.f), 255, "creditsBtn.png", true, "", false, "", false);
	ButtonInit(&exitBtn,	 CP_Vector_Set(1920.f / 2.f, mainMenuPanel.btnPos.y + ((float)CP_Image_GetHeight(settingsBtn.btnImage) * settingsBtn.btnScale.y) * 2.f),
		CP_Vector_Set(1.f, 1.f), 255, "exitBtn.png",true, "", false, "", false);
	// add the buttons to the respective screen
	mainMenuBtns[0] = playBtn;
	mainMenuBtns[1] = upgradesBtn;
	mainMenuBtns[2] = settingsBtn;
	mainMenuBtns[3] = creditsBtn;
	mainMenuBtns[4] = exitBtn;


	// Buttons for settings
	ButtonInit(&settingsBackdrop, CP_Vector_Set(1920.f / 2.f, 1080.f / 2.f), CP_Vector_Set(1.f, 1.f), 255,
		"SettingsMenuBackdrop.png", false, "", false, "", false);
	// Audio Bar icons
	ButtonInit(&audioBarActiveIcon, CP_Vector_Set(1.f, (1080.f / 10.f) * 3.f),
		CP_Vector_Set(1.f, 1.f), 255, "PauseAudioBarActive.png", false, "", false, "", false);
	ButtonInit(&audioBarInactiveIcon, CP_Vector_Set(1.f, (1080.f / 10.f) * 3.f),
		CP_Vector_Set(1.f, 1.f), 255, "PauseAudioBarInactive.png", false, "", false, "", false);

	// Audio Increment and Decrement buttons
	ButtonInit(&audioDecreaseBtn, CP_Vector_Set((1920.f / 2.f) - (CP_Image_GetWidth(audioBarInactiveIcon.btnImage) * 5.5f), (1080.f / 10) * 3.f),
		CP_Vector_Set(1.f, 1.f), 255, "PauseAudioDec.png", true, "", false, "", false);
	ButtonInit(&audioIncreaseBtn, CP_Vector_Set((1920.f / 2.f) + (CP_Image_GetWidth(audioBarInactiveIcon.btnImage) * 5.5f), (1080.f / 10) * 3.f),
		CP_Vector_Set(1.f, 1.f), 255, "PauseAudioInc.png", true, "", false, "", false);

	// Resolution buttons
	ButtonInit(&resolutionFullScreenBtn, CP_Vector_Set((1920.f / 8.f) * 5.f, (1080.f / 10.f) * 5.f),
		CP_Vector_Set(1.f, 1.f), 255, "PauseFullReso.png", true, "", false, "", false);
	ButtonInit(&resolution1080Btn, CP_Vector_Set((1920.f / 2.f), (1080.f / 10.f) * 5.f),
		CP_Vector_Set(1.f, 1.f), 255, "Pause1080Reso.png", true, "", false, "", false);
	ButtonInit(&resolution720Btn, CP_Vector_Set((1920.f / 8.f) * 3.f, (1080.f / 10.f) * 5.f),
		CP_Vector_Set(1.f, 1.f), 255, "Pause720Reso.png", true, "", false, "", false);


	settingsBtns[0] = audioDecreaseBtn;
	settingsBtns[1] = audioIncreaseBtn;

	settingsBtns[2] = resolution720Btn;
	settingsBtns[3] = resolution1080Btn;
	settingsBtns[4] = resolutionFullScreenBtn;

	// Upgrades vars

	ButtonInit(&upgradesBackdrop, CP_Vector_Set(1920.f / 2.f, 1080.f / 2.f), CP_Vector_Set(1.f, 1.f), 255,
		"UpgradesMenuBackdrop.png", false, "", false, "", false);

	// Upgrade Icons
	ButtonInit(&upgradesAttackIcon, CP_Vector_Set((1920.f / 8.f) * 4.f, (1080.f / 10.f) * 4.f),
		CP_Vector_Set(1.f, 1.f), 255, "atkIcon.png", false, "", false, "", false);
	ButtonInit(&upgradesHPIcon, CP_Vector_Set((1920.f / 8.f) * 2.75f, (1080.f / 10.f) * 4.f),
		CP_Vector_Set(1.f, 1.f), 255, "hpIcon.png", false, "", false, "", false);
	ButtonInit(&upgradesSpeedIcon, CP_Vector_Set((1920.f / 8.f) * 5.25f, (1080.f / 10.f) * 4.f),
		CP_Vector_Set(1.f, 1.f), 255, "spdIcon.png", false, "", false, "", false);
	
	// Upgrade increment and decrement buttons
	ButtonInit(&upgradesAtkDecBtn, CP_Vector_Set(upgradesAttackIcon.btnPos.x - ((float)CP_Image_GetWidth(upgradesAttackIcon.btnImage)), (1080.f / 10.f) * 4.f),
		CP_Vector_Set(1.f, 1.f), 255, "audioDecBtn.png", true, "", false, "", false);
	ButtonInit(&upgradesAtkIncBtn, CP_Vector_Set(upgradesAttackIcon.btnPos.x + ((float)CP_Image_GetWidth(upgradesAttackIcon.btnImage)), (1080.f / 10.f) * 4.f),
		CP_Vector_Set(1.f, 1.f), 255, "audioIncBtn.png", true, "", false, "", false);

	ButtonInit(&upgradesHpDecBtn, CP_Vector_Set(upgradesHPIcon.btnPos.x - ((float)CP_Image_GetWidth(upgradesHPIcon.btnImage)), (1080.f / 10.f) * 4.f),
		CP_Vector_Set(1.f, 1.f), 255, "audioDecBtn.png", true, "", false, "", false);
	ButtonInit(&upgradesHpIncBtn, CP_Vector_Set(upgradesHPIcon.btnPos.x + ((float)CP_Image_GetWidth(upgradesHPIcon.btnImage)), (1080.f / 10.f) * 4.f),
		CP_Vector_Set(1.f, 1.f), 255, "audioIncBtn.png", true, "", false, "", false);

	ButtonInit(&upgradesSpdDecBtn, CP_Vector_Set(upgradesSpeedIcon.btnPos.x - ((float)CP_Image_GetWidth(upgradesSpeedIcon.btnImage)), (1080.f / 10.f) * 4.f),
		CP_Vector_Set(1.f, 1.f), 255, "audioDecBtn.png", true, "", false, "", false);
	ButtonInit(&upgradesSpdIncBtn, CP_Vector_Set(upgradesSpeedIcon.btnPos.x + ((float)CP_Image_GetWidth(upgradesSpeedIcon.btnImage)), (1080.f / 10.f) * 4.f),
		CP_Vector_Set(1.f, 1.f), 255, "audioIncBtn.png", true, "", false, "", false);

	ButtonInit(&upgradesBarActiveIcon, CP_Vector_Set(1.f, upgradesAttackIcon.btnPos.y + (float)CP_Image_GetHeight(upgradesAttackIcon.btnImage)),
		CP_Vector_Set(1.f, 1.f), 255, "upgradesBarActive.png", false, "", false, "", false);
	ButtonInit(&upgradesBarInactiveIcon, CP_Vector_Set(1.f, upgradesAttackIcon.btnPos.y + (float)CP_Image_GetHeight(upgradesAttackIcon.btnImage)),
		CP_Vector_Set(1.f, 1.f), 255, "upgradesBarInactive.png", false, "", false, "", false);
	ButtonInit(&upgradesBarPendingIcon, CP_Vector_Set(1.f, upgradesAttackIcon.btnPos.y + (float)CP_Image_GetHeight(upgradesAttackIcon.btnImage)),
		CP_Vector_Set(1.f, 1.f), 255, "upgradesBarPending.png", false, "", false, "", false);

	ButtonInit(&upgradesConfirmBtn, CP_Vector_Set((1920.f / 10.f) * 6.1f, (1080.f / 10.f) * 6.5f),
		CP_Vector_Set(1.f, 1.f), 255, "confirmBtn.png", true, "", false, "", false);

	ButtonInit(&upgradesResetBtn, CP_Vector_Set((1920.f / 10.f) * 3.9f, (1080.f / 10.f) * 6.5f),
		CP_Vector_Set(1.f, 1.f), 255, "resetBtn.png", true, "", false, "", false);

	upgradesBtns[0] = upgradesAttackIcon;
	upgradesBtns[1] = upgradesHPIcon;
	upgradesBtns[2] = upgradesSpeedIcon;
	upgradesBtns[3] = upgradesAtkDecBtn;
	upgradesBtns[4] = upgradesAtkIncBtn;
	upgradesBtns[5] = upgradesHpDecBtn;
	upgradesBtns[6] = upgradesHpIncBtn;
	upgradesBtns[7] = upgradesSpdDecBtn;
	upgradesBtns[8] = upgradesSpdIncBtn;
	upgradesBtns[9] = upgradesResetBtn;
	upgradesBtns[10] = upgradesConfirmBtn;
	
	// For Credits UI

	ButtonInit(&creditsBackdrop, CP_Vector_Set(1920.f / 2.f, 1080.f / 2.f), CP_Vector_Set(1.f, 1.f), 255, "credits.png", false, "", false, "", false);
	ButtonInit(&creditsPage2, CP_Vector_Set(1920.f / 2.f, 1080.f / 2.f), CP_Vector_Set(1.f, 1.f), 255, "creditsPage2.png", false, "", false, "", false);
	ButtonInit(&creditsPage3, CP_Vector_Set(1920.f / 2.f, 1080.f / 2.f), CP_Vector_Set(1.f, 1.f), 255, "creditsPage3.png", false, "", false, "", false);

	ButtonInit(&gavinName, CP_Vector_Set((1920.f / 10.f) * 7.f, (1080.f / 10) * 5.f), CP_Vector_Set(1.f, 1.f), 255, "gavin.png", true, "", false, "", false);
	ButtonInit(&jiaWinName, CP_Vector_Set((1920.f / 10.f) * 7.f, (1080.f / 10) * 6.f), CP_Vector_Set(1.f, 1.f), 255, "jiawin.png", true, "", false, "", false);
	ButtonInit(&leonardName, CP_Vector_Set((1920.f / 10.f) * 2.75f, (1080.f / 10) * 5.f), CP_Vector_Set(1.f, 1.f), 255, "leonard.png", true, "", false, "", false);
	ButtonInit(&ryanName, CP_Vector_Set(1920.f / 2.f, (1080.f / 10) * 4.f), CP_Vector_Set(1.f, 1.f), 255, "ryan.png", true, "", false, "", false);
	ButtonInit(&marcName, CP_Vector_Set((1920.f / 10.f) * 3.f, (1080.f / 10) * 6.f), CP_Vector_Set(1.f, 1.f), 255, "marc.png", true, "", false, "", false);

	ButtonInit(&gavin, CP_Vector_Set(1.f, 1.f), CP_Vector_Set(1.f, 1.f), 255, "gavinImage.png", false, "", false, "", false);
	ButtonInit(&jiawin, CP_Vector_Set(1.f, 1.f), CP_Vector_Set(1.f, 1.f), 255, "jiawinImage.png", false, "", false, "", false);
	ButtonInit(&leonard, CP_Vector_Set(1.f, 1.f), CP_Vector_Set(1.f, 1.f), 255, "leonardImage.png", false, "", false, "", false);
	ButtonInit(&ryan, CP_Vector_Set(1.f, 1.f), CP_Vector_Set(1.f, 1.f), 255, "ryanImage.png", false, "", false, "", false);
	ButtonInit(&marc, CP_Vector_Set(1.f, 1.f), CP_Vector_Set(1.f, 1.f), 255, "marcImage.png", false, "", false, "", false);

	ButtonInit(&nextPageBtn, CP_Vector_Set((1920.f / 10.f) * 9.f, (1080.f / 10.f) * 2.f), CP_Vector_Set(1.f, 1.f), 255,
		"nextBtn.png", true, "", false, "", false);
	ButtonInit(&prevPageBtn, CP_Vector_Set((1920.f / 10.f) * 1.f, (1080.f / 10.f) * 2.f), CP_Vector_Set(1.f, 1.f), 255,
		"prevBtn.png", true, "", false, "", false);

	creditsButtons[0] = gavinName;
	creditsButtons[1] = jiaWinName;
	creditsButtons[2] = leonardName;
	creditsButtons[3] = ryanName;
	creditsButtons[4] = marcName;
	creditsButtons[5] = gavin;
	creditsButtons[6] = jiawin;
	creditsButtons[7] = leonard;
	creditsButtons[8] = ryan;
	creditsButtons[9] = marc;

	ButtonInit(&backBtn, CP_Vector_Set(1920.f / 2.f, (1080.f / 10.f) * 8.f)
		, CP_Vector_Set(1.f, 1.f), 255, "backBtn.png", true, "", false, "", false);

	// Setting resolution sizes for window res changing settings
	for (int j = RES_720; j < DISPLAY_END; ++j)
	{
		for (int i = 0; i < sizeof(settingsBtns) / sizeof(settingsBtns[0]); ++i)
		{
			ButtonSetRes(&settingsBtns[i], j, CP_Vector_Set(1920.f, 1080.f));
		}
		for (int i = 0; i < sizeof(mainMenuBtns) / sizeof(mainMenuBtns[0]); ++i)
		{
			ButtonSetRes(&mainMenuBtns[i], j, CP_Vector_Set(1920.f, 1080.f));
		}
		for (int i = 0; i < sizeof(upgradesBtns) / sizeof(upgradesBtns[0]); ++i)
		{
			ButtonSetRes(&upgradesBtns[i], j, CP_Vector_Set(1920.f, 1080.f));
		}
		for (int i = 0; i < sizeof(creditsButtons) / sizeof(creditsButtons[0]); ++i)
		{
			ButtonSetRes(&creditsButtons[i], j, CP_Vector_Set(1920.f, 1080.f));
		}

		ButtonSetRes(&backBtn, j, CP_Vector_Set(1920.f, 1080.f));

		ButtonSetRes(&audioBarActiveIcon, j, CP_Vector_Set(1920.f, 1080.f));
		ButtonSetRes(&audioBarInactiveIcon, j, CP_Vector_Set(1920.f, 1080.f));

		ButtonSetRes(&upgradesBarActiveIcon, j, CP_Vector_Set(1920.f, 1080.f));
		ButtonSetRes(&upgradesBarInactiveIcon, j, CP_Vector_Set(1920.f, 1080.f));
		ButtonSetRes(&upgradesBarPendingIcon, j, CP_Vector_Set(1920.f, 1080.f));

		ButtonSetRes(&mainMenuPanel, j, CP_Vector_Set(1920.f, 1080.f));
		ButtonSetRes(&mainMenuGameTitle, j, CP_Vector_Set(1920.f, 1080.f));
		ButtonSetRes(&mainMenuBackdrop, j, CP_Vector_Set(1920.f, 1080.f));

		ButtonSetRes(&settingsBackdrop, j, CP_Vector_Set(1920.f, 1080.f));
		ButtonSetRes(&upgradesBackdrop, j, CP_Vector_Set(1920.f, 1080.f));
		ButtonSetRes(&creditsBackdrop, j, CP_Vector_Set(1920.f, 1080.f));

		ButtonSetRes(&nextPageBtn, j, CP_Vector_Set(1920.f, 1080.f));
		ButtonSetRes(&prevPageBtn, j, CP_Vector_Set(1920.f, 1080.f));
		ButtonSetRes(&creditsPage2, j, CP_Vector_Set(1920.f, 1080.f));
		ButtonSetRes(&creditsPage3, j, CP_Vector_Set(1920.f, 1080.f));

		
	}

}

//function that sets the upgrades after reading the player save file
void MainMenuInitUpgradeCounter(void)
{
		upgradesCurrentHP = healthTempCounter;
		upgradesCurrentAttack = attackTempCounter;
		upgradesCurrentSpeed = speedTempCounter;
}

//function that updates the display text for the total currency
void GameUpdateCurrencyDisplay()
{
	ReadPlayerCurrencyTextFile();
	sprintf_s(playerCurrencyDisplay, DISPLAY_CURRENCY_ARR, "%d", currencyTemp);
}

//function that renders that player total currency
void GameRenderCurrencyDisplay()
{
	float xVal = 260.0f;
	float yVal = 260.0f;
	float textSize = 30.0f;
	float offSet = 50.0f;
	float ratio = CP_System_GetWindowHeight() / 720.0f;
	xVal *= ratio;
	yVal *= ratio;
	textSize *= ratio;
	offSet *= ratio;

	CP_Settings_TextSize(textSize);
	CP_Settings_Fill(white);
	CP_Font_DrawText(
		"Total Currency",
		(float)CP_System_GetWindowWidth() - xVal,
		yVal);
	CP_Font_DrawText(
		playerCurrencyDisplay,
		(float)CP_System_GetWindowWidth() - xVal,
		yVal + offSet);
}

//function that updates the display text for the currency used
void GameUpdateCurrencySpentDisplay()
{
	CalculateCost(upgradeCount);
	sprintf_s(playerCurrencySpentDisplay, DISPLAY_CURRENCY_ARR, "%d", upgradeCost);
	// printf("%d\n", upgradeCost);
}

//function that renders that player currency used
void GameRenderCurrencySpentDisplay()
{
	float xVal = 260.0f;
	float yVal = 130.0f;
	float textSize = 30.0f;
	float offSet = 50.0f;
	float ratio = CP_System_GetWindowHeight() / 720.0f;
	xVal *= ratio;
	yVal *= ratio;
	textSize *= ratio;
	offSet *= ratio;

	CP_Settings_TextSize(textSize);
	CP_Settings_Fill(white);
	CP_Font_DrawText(
		"Total Cost",
		(float)CP_System_GetWindowWidth() - xVal,
		yVal);
	CP_Font_DrawText(
		playerCurrencySpentDisplay,
		(float)CP_System_GetWindowWidth() - xVal,
		yVal + offSet);
}


void MainMenuInit(void)
{
	//load font
	font = CP_Font_Load("./Assets/font/8514oem.ttf");
	CP_Font_Set(font);
	
	//color white
	white = CP_Color_Create(255, 255, 255, 255);

	//test = true;
	state = MAIN;

	//audioBarActive	 = CP_Image_Load("Assets/audioBarActive.png");
	//audioBarInactive = CP_Image_Load("Assets/audioBarInactive.png");
	audioMasterVolumeActiveBars = 1;

	//Read Player Data
	//ReadPlayerTextFile();

	// Read Upgrades file here

	upgradesCurrentAttack = 0;
	upgradesCurrentHP = 0;
	upgradesCurrentSpeed = 0;
	//printf("%d, %d, %d \n", upgradesCurrentAttack, upgradesCurrentHP, upgradesCurrentSpeed);
	
	//reads the player file
	ReadPlayerTextFile();
	MainMenuInitUpgradeCounter();
	// printf("%d, %d, %d \n", upgradesCurrentHP, upgradesCurrentAttack, upgradesCurrentSpeed);

	//read the currency
	ReadPlayerCurrencyTextFile();

	upgradesAttackToAdd = 0;
	upgradesHPToAdd = 0;
	upgradesSpeedToAdd = 0;

	numPages = 3;
	currentPage = 1;

	fadeIntoMenu = true;
	fadeAlpha = 255;
	fadeTimer = 0.5f;

	SpriteInit(&mainMenuCursor, "PlayerCursor.png", false, 1.f, 255);
	SpriteBetterInit(&transitionClose720Main, "transition_closing720.png", 1.f, 255, 4, 4, 14);
	SpriteBetterInit(&transitionClose1080Main, "transition_closing1080.png", 1.f, 255, 4, 4, 14);
	SpriteBetterInit(&transitionOpen720Main, "transition_opening720.png", 1.f, 255, 3, 3, 8);
	SpriteBetterInit(&transitionOpen1080Main, "transition_opening1080.png", 1.f, 255, 3, 3, 8);

	CP_System_ShowCursor(false);

	CP_Settings_ImageMode(CP_POSITION_CENTER);
	CP_Settings_ImageWrapMode(CP_IMAGE_WRAP_CLAMP);
	//CP_System_SetWindowSize(1920, 1080);
	//CP_System_Fullscreen();

	MainMenuInitButtons();
	//CP_System_SetWindowSize(1280, 720);

	// Star Logic
	MenuStarParticleInit();
	// Update UI Res also sets stars boundaries
	MainMenuUpdateUIRes(GetDisplay());

	// Audio
	AudioInit();
	// Load saved from savefile audio settings 
	ReadAudioTextFile();
	audioMasterVolumeActiveBars = masterVolumeTemp;
	AudioSetVolume(AUDIO_GROUP_MASTER, masterVolumeTemp);

	// Particle - Only can render simple particles, no camera offset
	ParticleInitSimple();

	// Check if quit exe (altf4) or game (game.c)
	isQuittingToGame = false;

	AudioPlayMusic(AUDIO_TYPE_MENU_MUSIC);

}

void MainMenuUpdate(void)
{
	CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));
	ButtonDraw(mainMenuBackdrop, 0.f);
	MenuStarParticleUpdate();
	ParticleUpdate();
	ParticleRender();

	// draw ui and check button/mouse clicks according to state
	switch (state)
	{
	case MAIN:
	{
		MainMenuDraw();
		if (!GetSTM() || !GetMTG() || !GetGTM())
			MainMenuCheckClicks();
		break;
	}
	case UPGRADES:
	{
		MainMenuDrawUpgrades();
		MainMenuCheckUpgradesClicks();
		GameRenderCurrencyDisplay();
		GameRenderCurrencySpentDisplay();
		break;
	}
	case SETTINGS:
	{
		MainMenuDrawSettings();
		MainMenuCheckSettingsClicks();
		break;
	}
	case CREDITS:
	{

		MainMenuDrawCredits();
		MainMenuCheckCreditsClicks();
		break;
	}
	default:
		break;
	}
	SpriteUpdate(&mainMenuCursor);
	SpritePlayAnim(mainMenuCursor,
		CP_Vector_Set(CP_Input_GetMouseX() + 0.5f * CP_Image_GetHeight(mainMenuCursor.sprLeftSpriteSheet),
			CP_Input_GetMouseY() + 0.5f * CP_Image_GetHeight(mainMenuCursor.sprLeftSpriteSheet)));


	// Draw Transition Sprites
	// for going to game from main menu
	if (GetMTG())
	{
		if (GetDisplay() == RES_720)
		{
			SpriteUpdate(&transitionClose720Main);
			if (transitionClose720Main.sprAnimIndex >= transitionClose720Main.sprTotalFrames - 1)
			{
				CP_Engine_SetNextGameState(GameInit, GameUpdate, GameExit);
			}

			SpriteBetterPlayAnim(transitionClose720Main, CP_Vector_Set((float)CP_System_GetWindowWidth() / 2.f, (float)CP_System_GetWindowHeight() / 2.f));
		}
		else
		{
			SpriteUpdate(&transitionClose1080Main);
			if (transitionClose1080Main.sprAnimIndex >= transitionClose1080Main.sprTotalFrames - 1)
			{
				CP_Engine_SetNextGameState(GameInit, GameUpdate, GameExit);
			}

			SpriteBetterPlayAnim(transitionClose1080Main, CP_Vector_Set((float)CP_System_GetWindowWidth() / 2.f, (float)CP_System_GetWindowHeight() / 2.f));
		}
	}
	// for going to main menu from game
	if (GetGTM())
	{
		if (GetDisplay() == RES_720)
		{
			SpriteUpdate(&transitionOpen720Main);
			if (transitionOpen720Main.sprAnimIndex >= transitionOpen720Main.sprTotalFrames - 1)
			{
				SetGTM(false);
			}

			SpriteBetterPlayAnim(transitionOpen720Main, CP_Vector_Set((float)CP_System_GetWindowWidth() / 2.f, (float)CP_System_GetWindowHeight() / 2.f));
		}
		else
		{
			SpriteUpdate(&transitionOpen1080Main);
			if (transitionOpen1080Main.sprAnimIndex >= transitionOpen1080Main.sprTotalFrames - 1)
			{
				SetGTM(false);
			}

			SpriteBetterPlayAnim(transitionOpen1080Main, CP_Vector_Set((float)CP_System_GetWindowWidth() / 2.f, (float)CP_System_GetWindowHeight() / 2.f));
		}
	}
	// for going from splash screen to main menu
	if (GetSTM())
	{
		fadeAlpha -= (CP_System_GetDt() / fadeTimer) * 255.f;
		if (fadeAlpha <= 0.f)
		{
			SetSTM(false);
		}
		CP_Settings_Fill(CP_Color_Create(0, 0, 0, (int)fadeAlpha));
		CP_Graphics_DrawRect(0.f,0.f, (float)CP_System_GetWindowWidth(), (float)CP_System_GetWindowHeight());
	}
	
}

void MainMenuExit(void)
{
	for (int i = 0; i < sizeof(mainMenuBtns) / sizeof(mainMenuBtns[0]); ++i)
	{
		ButtonReleaseVars(&mainMenuBtns[i]);
	}
	for (int j = 0; j < sizeof(settingsBtns) / sizeof(settingsBtns[0]); ++j)
	{
		ButtonReleaseVars(&settingsBtns[j]);
	}
	for (int k = 0; k < sizeof(upgradesBtns) / sizeof(upgradesBtns[0]); ++k)
	{
		ButtonReleaseVars(&upgradesBtns[k]);
	}
	for (int l = 0; l < sizeof(creditsButtons) / sizeof(creditsButtons[0]); ++l)
	{
		ButtonReleaseVars(&creditsButtons[l]);
	}
	ButtonReleaseVars(&backBtn);
	//ButtonReleaseVars(&audioBarActive);
	//ButtonReleaseVars(&audioBarInactive);
	ButtonReleaseVars(&audioBarActiveIcon);
	ButtonReleaseVars(&audioBarInactiveIcon);
	ButtonReleaseVars(&upgradesBarActiveIcon);
	ButtonReleaseVars(&upgradesBarInactiveIcon);
	ButtonReleaseVars(&upgradesBarPendingIcon);
	ButtonReleaseVars(&mainMenuPanel);
	ButtonReleaseVars(&mainMenuGameTitle);
	ButtonReleaseVars(&mainMenuBackdrop);
	ButtonReleaseVars(&settingsBackdrop);
	ButtonReleaseVars(&upgradesBackdrop);
	ButtonReleaseVars(&creditsBackdrop);
	ButtonReleaseVars(&nextPageBtn);
	ButtonReleaseVars(&prevPageBtn);
	ButtonReleaseVars(&creditsPage2);
	ButtonReleaseVars(&creditsPage3);


	SpriteReleaseVars(&mainMenuCursor);

	SpriteReleaseVars(&transitionClose720Main);
	SpriteReleaseVars(&transitionClose1080Main);
	SpriteReleaseVars(&transitionOpen720Main);
	SpriteReleaseVars(&transitionOpen1080Main);

	// Stops rendering particle and stops audio
	ParticleExit();
	AudioExit();
	if (isQuittingToGame == false)
	{
		// Game was altf4'd
		ParticleQuitGame();
		AudioQuitGame();
		// printf("Quitting exe...\n");
	}
	else
	{
		// printf("Quitting to game...\n");
	}

}
