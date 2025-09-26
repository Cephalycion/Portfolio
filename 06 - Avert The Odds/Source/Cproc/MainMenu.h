#pragma once
/*!
@copyright	DIGIPEN INSTITUTE OF TECHNOLOGY SINGAPORE
@file       MainMenu.h
@author     Marc Alviz Evangelista(marcalviz.e),
			Gavin Ang Jun Liang(ang.g),
			Lau Jia Win (jiawin.lau)
@course     CSD1401 Software Engineering Project 1
@section    A
@tutorial   Final Project
@date       18/11/2023
@brief      This file contains the declarations of functions which renders the MainMenu UI
			to screen and also has the settings and persistant upgrades for the game.
*//*_________________________________________________________________________*/

#include "cprocessing.h"
#include <stdio.h>
#include "Button.h"
#include "Sprite.h"
#include "Game.h"
#include "PlayerStatusUpgrade.h"
#include "ReadFile.h"
#include "Utils.h"
#include "GameFlags.h"
#include "MenuStarParticle.h"

enum MENU_STATE 
{
	DEFAULT = 0,
	MAIN,
	UPGRADES,
	SETTINGS,
	CREDITS
};

// Used to check which screen to draw
enum MENU_STATE state;

// Vars for Main Menu
Button mainMenuBackdrop;
Button mainMenuPanel;
Button mainMenuGameTitle;

Button mainMenuBtns[5];
Button exitBtn;
Button playBtn;
Button creditsBtn;
Button settingsBtn;
Button upgradesBtn;

Button settingsBtns[5];
// Vars for audio
Button settingsBackdrop;

Button audioIncreaseBtn;
Button audioDecreaseBtn;
CP_Image audioBarActive;
CP_Image audioBarInactive;
int audioMasterVolumeActiveBars;

Button audioBarActiveIcon;
Button audioBarInactiveIcon;

// Vars for resolution
Button resolutionFullScreenBtn;
Button resolution1080Btn;
Button resolution720Btn;

Button upgradesBackdrop;


// Vars for upgrades
Button upgradesHPIcon;
Button upgradesAttackIcon;
Button upgradesSpeedIcon;

Button upgradesAtkDecBtn;
Button upgradesAtkIncBtn;
Button upgradesHpDecBtn;
Button upgradesHpIncBtn;
Button upgradesSpdDecBtn;
Button upgradesSpdIncBtn;

Button upgradesConfirmBtn;
Button upgradesResetBtn;

Button upgradesBtns[11];

int upgradesCurrentHP;
int upgradesCurrentAttack;
int upgradesCurrentSpeed;

int upgradesHPToAdd;
int upgradesAttackToAdd;
int upgradesSpeedToAdd;

int upgradeCount;
char playerCurrencyDisplay[DISPLAY_CURRENCY_ARR];
//function that updates the display text for the total currency
void GameUpdateCurrencyDisplay(void);
//function that renders that player total currency
void GameRenderCurrencyDisplay(void);

char playerCurrencySpentDisplay[DISPLAY_CURRENCY_ARR];
//function that updates the display text for the currency used
void GameUpdateCurrencySpentDisplay(void);
//function that renders that player currency used
void GameRenderCurrencySpentDisplay(void);

Button upgradesBarActiveIcon;
Button upgradesBarInactiveIcon;
Button upgradesBarPendingIcon;

// Credits Vars

Button creditsBackdrop;

Button creditsButtons[10];

Button gavin;
Button leonard;
Button jiawin;
Button ryan;
Button marc;

// Buttons to show the pictures of the above
Button gavinName;
Button leonardName;
Button jiaWinName;
Button ryanName;
Button marcName;

Button nextPageBtn;
Button prevPageBtn;

int numPages;
int currentPage;

Button creditsPage2;
Button creditsPage3;

// Back Button
Button backBtn;


// Splash > Main Menu Fade
bool fadeIntoMenu;
float fadeTimer;
float fadeAlpha;

// Main Menu > Game Fade
Sprite transitionClose720Main;
Sprite transitionClose1080Main;

// Game > Main Menu Fade
Sprite transitionOpen720Main;
Sprite transitionOpen1080Main;


Sprite mainMenuCursor;

// Check if quit exe (altf4) or game (game.c)
bool isQuittingToGame;		// USE ONLY FROM MAINMENU.C (PRIVATE VARIABLE)

// This is to initialize all buttons used in Main Menu scene
void MainMenuInitButtons(void);
// Update the positions and scaling of UI elements when user changes display settings
void MainMenuUpdateUIRes(int);

void MainMenuInit(void);
void MainMenuUpdate(void);
void MainMenuExit(void);

// the 4 functions below draw their respective UIs according to the MAIN_MENU_STATE
void MainMenuDraw(void);
void MainMenuDrawSettings(void);
void MainMenuDrawUpgrades(void);
void MainMenuDrawCredits(void);

// the 4 functions below check their respective mouse clicks according to the MAIN_MENU_STATE
void MainMenuCheckClicks(void);
void MainMenuCheckSettingsClicks(void);
void MainMenuCheckUpgradesClicks(void);
void MainMenuCheckCreditsClicks(void);

void MainMenuInitUpgradeCounter(void);

CP_Font font;
