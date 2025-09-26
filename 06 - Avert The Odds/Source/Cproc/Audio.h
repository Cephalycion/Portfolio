/*!
@copyright	DIGIPEN INSTITUTE OF TECHNOLOGY SINGAPORE
@file 		Audio.h
@author 	Lau Jia Win (jiawin.lau)
@course     CSD1401 Software Engineering Project 1
@section    a
@tutorial   Final Project
@date 		06/11/2023
@brief 		This file contains the function declarations, enum and struct for
			the Audio system in the game.
*//*______________________________________________________________________*/
#pragma once
#include "CProcessing.h"	// CP_Sound, CP_SOUND_GROUP

// Define all the different types of audio to be played
typedef enum AUDIO_TYPE {
	AUDIO_TYPE_EXPLOSION_SFX = 0,
	AUDIO_TYPE_PLAYER_SHOOT_SFX,
	AUDIO_TYPE_PLAYER_MULTI_SFX,
	AUDIO_TYPE_MENU_POP_UIFX,
	AUDIO_TYPE_MENU_HOVER_UIFX,
	AUDIO_TYPE_MENU_BACK_UIFX,
	AUDIO_TYPE_PLAYER_HIT_SFX,
	//AUDIO_TYPE_ENEMY_DIE_SFX,
	AUDIO_TYPE_ENEMY_HIT_SFX,

	AUDIO_TYPE_SICKBEATS_MUSIC,
	AUDIO_TYPE_MENU_MUSIC,

	AUDIO_TYPE_MAX_COUNT
} AUDIO_TYPE;

// Matches with CP_SOUND_GROUP for SFX and Music
// But will use this specific to this Audio file instead
typedef enum AUDIO_GROUP {
	AUDIO_GROUP_SFX = 0,			// SFX only
	AUDIO_GROUP_UIFX,				// Menu sounds only
	AUDIO_GROUP_MUSIC,

	AUDIO_GROUP_MASTER,				// Master volume (all audio)

	AUDIO_GROUP_MAX_COUNT
} AUDIO_GROUP;


// Main Audio Module - AudioObject
typedef struct AudioObject {
	CP_Sound soundFile;				// Contains the CP_Sound to be played
	AUDIO_GROUP audioGroup;			// Plays the CP_Sound in this audio group

} AudioObject;


// ==============================================================
// Call these 2 functions in the main menu scene file respectively
// 
void AudioInit();						// Loads all audio files (#DEFINE as spatial radius)
void AudioUpdate();
void AudioExit();						// Stop all audio (moving between main menu and game)
void AudioQuitGame();					// Unloads all audio files, CALL WHEN EXIT GAME
//
// ==============================================================


// ==============================================================
// Call these functions to play audio whereever
// 
void AudioPlay(AUDIO_TYPE type);						// Play audio of specified type
// Play spatial audio of type, taking into account player pos, and entity pos
void AudioPlaySpatial(AUDIO_TYPE type, float playerX, float playerY, float entityX, float entityY);
void AudioPlayMusic(AUDIO_TYPE type);					// Plays type under music group
//
// ==============================================================


// ==============================================================
// Control Functions
// 
void AudioPause(AUDIO_GROUP groupToPause);				// Temporarily pauses group
void AudioUnpause(AUDIO_GROUP groupToUnpause);			// Unpauses	group
void AudioStop(AUDIO_GROUP groupToStop);				// Stop playing all in that group 
void AudioIncreaseVolume(AUDIO_GROUP groupToIncrease);	// Increase volume by #DEFINE amt
void AudioDecreaseVolume(AUDIO_GROUP groupToDecrease);	// Decrease volume by #DEFINE amt
// In game
void AudioEnterMenu();
void AudioExitMenu();
// Menu options
void AudioMenuAnyButtonClicked();
void AudioMenuBackButtonClicked();

// Save Load usage
void AudioSetVolume(AUDIO_GROUP groupToSet, int volume);// Sets volume of specified group to specified level
int AudioGetVolume(AUDIO_GROUP groupToGet);				// Get current volume level of specified group
//
// ==============================================================


// Private (Only Audio.c use) Functions
void AudioLoad(AUDIO_TYPE type, char const* fileName, AUDIO_GROUP group);	// Load audio into audioObject
float AudioGetScaledVolume(AUDIO_TYPE type, float optionalInputVolume);		// Scale the audio with volume settings
float AudioLerp(float start, float end, float percentage);
