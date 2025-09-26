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
@brief      This file contains the declarations of functions which execute game
			logic, as well as render UI and game objects to screen.
*//*_________________________________________________________________________*/

#pragma once
#include "Utils.h"
#include "MapGeneration.h"
#include "Obstacle.h"
#include "Particle.h"
#include "Audio.h"
#include "Sprite.h"
#include <time.h>
#include "WeaponInfo.h"
#include "Button.h"
#include "PlayerStatusUpgrade.h"
#include "GameFlags.h"

#define MAP_BOUNDS_X	1500
#define MAP_BOUNDS_Y	1500
#define WEAPONINFO_SIZE	3
#define DISPLAY_ARR		11
#define DISPLAY_CURRENCY_ARR	50

GameObject* player;
GameObject objects[200];
GameObject* GameGetObject(GAMEOBJ_TYPE type);

WeaponInfo weaponInfo[WEAPONINFO_SIZE];

Tween tweens[100];

void GameClearTween(Tween* tween);
void GameSetTween(
	int objectIndex,
	CP_Vector direction,
	float speed,
	float decay
);

CP_Vector cameraOffset;
CP_Color white;
CP_Color red;
CP_Color yellow;
CP_Color blue;
CP_Color cyan;
CP_Color purple;
CP_Color orange;
CP_Color green;

CP_Vector gameCenter;
Sprite playerSpriteWalking;
Sprite playerSpriteStanding;

// Enemies
Sprite enemySprite;
Sprite enemyEliteSprite;
Sprite enemyBossSprite;
float waveTimer;			// current wave time (counts down to 0)
float waveInterval;			// interval between enemy waves in seconds
float eliteWaveInterval;	// interval between Elite enemy waves in seconds
int enemiesPerWave;			// how many enemies to spawn per wave
int waveCounter;			// counts the number oof waves before spawning elite enemy
int bossSpawnCounter;		// counts number of waves before boss spawn
int enemyBulletCounter;
float enemyFireTimer;
float enemyFireInterval;
float enemyBurstTimer;
float enemyBurstInterval;
bool enemyBossBurst;

// Player
int playerLevel;
int playerExp;
int playerExpLimit;
int playerIsMoving;
int playerIsFacingLeft;

// Ryan's UI
int playerScore;
char playerScoreDisplay[DISPLAY_ARR];
char playerHealthDisplay[DISPLAY_ARR];
Button playerHealthIcon;

// Basic Fire
float bfTimer;
float bfInterval;

// Weapons General
int maxWeaponLevel;

// Weapons_Multishot
float msTimer;				// current wave time to spawn multi_shot Projectile
float msInterval;			// Interval between each spawn

// Weapons_Circling
float cirTimer;				// current wave time to spawn circling Projectile
float cirInterval;			// Interval between each spawn
float cirHitInterval;		// Interval to hit each enemy

// Weapons_AOE
GameObject* allyAOE;
float aoeTimer;				// current wave time to spawn ELECTRO_DOME
float aoeInterval;			// Interval between each appearance
int aoeActive;				// if its not active, then spawn ELECTRO_DOME
CP_Color aoe;
float aoeAlpha;
float aoeAlphaChanger;

float damageModifer;

// Pause bools
bool gameLevelUp;		// For the pause menu/level up screen
bool gamePause;
bool gameEnd;
int weaponMenuOpen;

// Level Up UI

Button electroIcon;
Button plasmaIcon;
Button oribitersIcon;

Button levelUpBackdrop;
Button buttonLevelUpOption1;
Button buttonLevelUpOption2;
Button buttonLevelUpOption3;

Button levelUpMaxedOption;

Button levelUpOptionButtons[3];

WEAPON_TYPE levelUpOption1Type;
WEAPON_TYPE levelUpOption2Type;
WEAPON_TYPE levelUpOption3Type;

// Pause Menu UI
Button pauseBackdrop;

Button pauseAudioBarActiveIcon;
Button pauseAudioBarInactiveIcon;

int pauseAudioBarActiveCount;

Button pauseAudioIncButton;
Button pauseAudioDecButton;

Button pause720ResolutionButton;
Button pause1080ResolutionButton;
Button pauseFullResolutionButton;

Button pauseExitButton;
Button pauseResumeButton;

Button pauseMenuButtons[7];

// Game End Screen
Button endBackdrop;

Button endQuitToMainButton;
Button endRestartButton;

Button endScreenButtons[2];

// Game UI
Sprite cursorUI;
Sprite cursorAim;

// Game > Main Menu Fade
Sprite transitionClose720Game;
Sprite transitionClose1080Game;

// Main Menu > Game Fade
Sprite transitionOpen720Game;
Sprite transitionOpen1080Game;

// Check if quit exe (altf4) or exit to main menu (from pause/end game)
bool isQuittingToMainMenu;		// USE ONLY FROM GAME.C (PRIVATE VARIABLE)

void GameDrawEndScreen(void);
void GameDrawPauseMenuUI(void);
void GameDrawLevelUpUI(void);
void GameSetupLevelUpOptions(void);
void GameCheckButtonClicks(void);
void GameInitButtons(void);
void GameUpdateUIRes(int);
void GameGetWeaponLevel(WEAPON_TYPE weapo, char arr[]);

void GameSpriteProgress(GameObject* object);
float GameDealDamage(GameObject* object, float damage);
void GameProjectileExplode(GameObject *projectile, bool isCollideObstacle);
void GameGainExp(int exp);
void GameUpdateScoreDisplay();
void GameUpdateHealthDisplay();
void GameClearObject(GameObject* object);
void GameInit(void);
void GameUpdate(void);
void GameExit(void);

void CreateBasicFireProjectile(void);
void CreateMultiShotProjectile(void);
void CreateCirclingProjectile(void);

void CreateBossEnemyFireProjectile(GameObject* object);

void GamePlayParticleAndSound(GameEvent eventType, GameObject* objectSoundFrom, int value);
