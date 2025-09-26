/*!
@copyright	DIGIPEN INSTITUTE OF TECHNOLOGY SINGAPORE
@file 		Audio.c
@author 	Lau Jia Win (jiawin.lau)
@course     CSD1401 Software Engineering Project 1
@section    a
@tutorial   Final Project
@date 		06/11/2023
@brief 		This file contains the function definitions for
			the Audio system in the game.
*//*______________________________________________________________________*/
#include "Audio.h"
#include <stdio.h>			// Debug: printf
#include <stdbool.h>		// bool

// Definitions
#define AUDIODEF_NORM_MIN	0.1F						// Min volume (scale of 0.0->1.0)
#define AUDIODEF_NORM_MAX	1.0F						// Max volume (scale of 0.0->1.0)
#define AUDIODEF_PITCH		1.0F						// Audio pitch for all
#define AUDIODEF_NO_LOOPING	0							// No loop audio boolean
#define AUDIODEF_LOOPING	1							// Loop audio boolean
#define AUDIODEF_VOL_MIN	0							// Min volume
#define AUDIODEF_VOL_MAX	10							// Max volume
#define AUDIODEF_VOL_CHANGE	1							// Amount to increase/decrease volume
#define AUDIODEF_VOL_NORM	(1.0F/AUDIODEF_VOL_MAX.F)	// Scales audio to b/w 0.0->1.0
#define AUDIODEF_SPATIAL_RD	(1280*0.5F)					// Spatial radius (half screenwidth)
#define AUDIODEF_FADEOUT_TO	0.85F						// Fade music in menu to this pitch IN MENU
#define AUDIODEF_FADEIN_TO	1.0F						// Fade music in menu to this pitch EXIT MENU
#define AUDIODEF_FADE_RATE	2.0F						// Rate to fade in/out music in menus
#define AUDIODEF_MAX_RPF	1							// Maximum repeat per frame same audio (enemy hit)
														// that is repeated in a single frame (else very loud)

// "Private variable"
AudioObject audioObjList[AUDIO_TYPE_MAX_COUNT];			// List of all audioObjects
float audioSpatialRadius = 0.0f;						// Radius used to calculate spatial audio
bool isAudioLoaded = false;								// Prevents loading audio twice
int audioVolumeList[AUDIO_GROUP_MAX_COUNT] = { 0 };		// List of volumes of all types (sfx,music,master)
bool audioMusicFadeInStarted = false;					// Start to fade in music if true (exit menu)
bool audioMusicFadeOutStarted = false;					// Start to fade out music if true (enter menu)
float percentage = 0.0f;								// perecentage fading in/out (enter/exit menu)
int repeatedAudioCount = 0;								// Counts number of time audio is repeated in single frame
														// and prevents too much playing at once

#pragma region Audio Standard Functions

void AudioInit()
{
	// If audio already loaded, don't load again
	if (isAudioLoaded)
		return;

	// Set spatial radius
	audioSpatialRadius = AUDIODEF_SPATIAL_RD;

	// Load all audio files
	AudioLoad(AUDIO_TYPE_EXPLOSION_SFX, "./Assets/Audio/projectile_explode.ogg", AUDIO_GROUP_SFX);
	AudioLoad(AUDIO_TYPE_PLAYER_SHOOT_SFX, "./Assets/Audio/player_weapon_normal.ogg", AUDIO_GROUP_SFX);
	AudioLoad(AUDIO_TYPE_PLAYER_MULTI_SFX, "./Assets/Audio/player_weapon_multi.ogg", AUDIO_GROUP_SFX);
	
	AudioLoad(AUDIO_TYPE_MENU_POP_UIFX, "./Assets/Audio/menu_pop.ogg", AUDIO_GROUP_UIFX);
	AudioLoad(AUDIO_TYPE_MENU_HOVER_UIFX, "./Assets/Audio/menu_hover.ogg", AUDIO_GROUP_UIFX);
	AudioLoad(AUDIO_TYPE_MENU_BACK_UIFX, "./Assets/Audio/menu_back.ogg", AUDIO_GROUP_UIFX);

	AudioLoad(AUDIO_TYPE_PLAYER_HIT_SFX, "./Assets/Audio/player_hit.ogg", AUDIO_GROUP_SFX);
	AudioLoad(AUDIO_TYPE_ENEMY_HIT_SFX, "./Assets/Audio/enemy_hit.ogg", AUDIO_GROUP_SFX);

	AudioLoad(AUDIO_TYPE_SICKBEATS_MUSIC, "./Assets/Audio/Arcade Music Loop by joshuaempyre Id-251461.wav", AUDIO_GROUP_MUSIC);
	AudioLoad(AUDIO_TYPE_MENU_MUSIC, "./Assets/Audio/Cold In Space by XHALE303 Id-534691.wav", AUDIO_GROUP_MUSIC);

	// Initialize all volume to MAX
	for (int i = 0; i < AUDIO_GROUP_MAX_COUNT; i++)
	{
		audioVolumeList[i] = AUDIODEF_VOL_MAX;
	}

	// Flag audio already loaded
	isAudioLoaded = true;
}

// Music fading
void AudioUpdate()
{
	repeatedAudioCount = 0;
	// If spam it kinda goes from 0.9 to 0.85,0.86... instead of 0.9,0.89...0.85,0.86..
	if (audioMusicFadeOutStarted)
	{
		percentage += AUDIODEF_FADE_RATE * CP_System_GetDt();

		float res = AudioLerp(AUDIODEF_FADEIN_TO, AUDIODEF_FADEOUT_TO, percentage);
		//printf("fade out %f% res: %f\n", percentage, res);
		CP_Sound_SetGroupPitch(AUDIO_GROUP_MUSIC, res);

		if (percentage >= 1.f)
		{
			percentage = 0;
			audioMusicFadeOutStarted = false;
		}
	}
	else if (audioMusicFadeInStarted)
	{
		percentage += AUDIODEF_FADE_RATE * CP_System_GetDt();

		float res = AudioLerp(AUDIODEF_FADEOUT_TO, AUDIODEF_FADEIN_TO, percentage);
		//printf("fade in %f% res: %f\n", percentage, res);
		CP_Sound_SetGroupPitch(AUDIO_GROUP_MUSIC, res);

		if (percentage >= 1.f)
		{
			percentage = 0;
			audioMusicFadeInStarted = false;
		}
	}
}

// Stop all audio (moving between main menu and game)
void AudioExit()
{
	AudioStop(AUDIO_GROUP_MASTER);
}

// Unloads all audio files, CALL WHEN EXIT GAME
void AudioQuitGame()
{
	for (int i = 0; i < AUDIO_TYPE_MAX_COUNT; i++)
	{
		CP_Sound_Free(&audioObjList[i].soundFile);
	}

	// Flag audio unloaded
	isAudioLoaded = false;
}
#pragma endregion


#pragma region Audio Play Functions

void AudioPlay(AUDIO_TYPE type)
{
	CP_Sound_PlayAdvanced(audioObjList[type].soundFile, AudioGetScaledVolume(type, 1.f), AUDIODEF_PITCH, AUDIODEF_NO_LOOPING, audioObjList[type].audioGroup);
}

void AudioPlaySpatial(AUDIO_TYPE type, float playerX, float playerY, float entityX, float entityY)
{
	// Prevents too many same enemy hit to be played in one frame (very loud)
	if (type == AUDIO_TYPE_ENEMY_HIT_SFX)
	{
		++repeatedAudioCount;
		if (repeatedAudioCount > AUDIODEF_MAX_RPF)
		{
			return;
		}
	}

	// Find dist of vector Player->Entity
	float volumeBasedOnDist = (entityX - playerX) * (entityX - playerX) + (entityY - playerY) * (entityY - playerY);	// Refactored

	// Uses the formula (value-MIN)/(MAX-MIN) -> MIN=0, MAX=audioSpatialRadius
	volumeBasedOnDist /= (audioSpatialRadius * audioSpatialRadius);														// Refactored
	// Reverse it so that it will be softer further away, rather than other way around
	volumeBasedOnDist = 1.0f - volumeBasedOnDist;
	// Normalize the distance between MIN(0.1) and MAX(1.0)
	volumeBasedOnDist = (volumeBasedOnDist < AUDIODEF_NORM_MIN ? AUDIODEF_NORM_MIN : volumeBasedOnDist);
	volumeBasedOnDist = (volumeBasedOnDist > AUDIODEF_NORM_MAX ? AUDIODEF_NORM_MAX : volumeBasedOnDist);

	// Scale and play audio
	CP_Sound_PlayAdvanced(audioObjList[type].soundFile, AudioGetScaledVolume(type, volumeBasedOnDist), AUDIODEF_PITCH, AUDIODEF_NO_LOOPING, audioObjList[type].audioGroup);
}

// Unfortunately, can't change volume of music while being played..
// Only way is quit game.c and enter again...
void AudioPlayMusic(AUDIO_TYPE type)
{
	CP_Sound_StopGroup(CP_SOUND_GROUP_MUSIC);
	CP_Sound_PlayAdvanced(audioObjList[type].soundFile, AudioGetScaledVolume(type, 0.3f), AUDIODEF_PITCH, AUDIODEF_LOOPING, audioObjList[type].audioGroup);
}
#pragma endregion


#pragma region Audio Control Functions

void AudioPause(AUDIO_GROUP groupToPause)
{
	switch (groupToPause)
	{
	case AUDIO_GROUP_SFX:
	case AUDIO_GROUP_UIFX:
	case AUDIO_GROUP_MUSIC:
	{
		CP_Sound_PauseGroup(groupToPause);
		break;
	}
	case AUDIO_GROUP_MASTER:
	{
		CP_Sound_PauseAll();
		break;
	}
	default:
	{
		// printf("Audio: Invalid Pause parameter!\n");
		break;
	}
	}
}

void AudioUnpause(AUDIO_GROUP groupToUnpause)
{
	switch (groupToUnpause)
	{
	case AUDIO_GROUP_SFX:
	case AUDIO_GROUP_UIFX:
	case AUDIO_GROUP_MUSIC:
	{
		CP_Sound_ResumeGroup(groupToUnpause);
		break;
	}
	case AUDIO_GROUP_MASTER:
	{
		CP_Sound_ResumeAll();
		break;
	}
	default:
	{
		// printf("Audio: Invalid Unpause parameter!\n");
		break;
	}
	}
}

void AudioStop(AUDIO_GROUP groupToStop)
{
	switch (groupToStop)
	{
	case AUDIO_GROUP_SFX:
	case AUDIO_GROUP_UIFX:
	case AUDIO_GROUP_MUSIC:
	{
		CP_Sound_StopGroup(groupToStop);
		break;
	}
	case AUDIO_GROUP_MASTER:
	{
		CP_Sound_StopAll();
		break;
	}
	default:
	{
		// printf("Audio: Invalid Stop parameter!\n");
		break;
	}
	}
}

void AudioIncreaseVolume(AUDIO_GROUP groupToIncrease)
{
	int currentVol = audioVolumeList[groupToIncrease];
	audioVolumeList[groupToIncrease] = currentVol + AUDIODEF_VOL_CHANGE > AUDIODEF_VOL_MAX ? AUDIODEF_VOL_MAX : currentVol + AUDIODEF_VOL_CHANGE;

	// printf("Audio: Increased vol to %d\n", audioVolumeList[groupToIncrease]);
}

void AudioDecreaseVolume(AUDIO_GROUP groupToDecrease)
{
	int currentVol = audioVolumeList[groupToDecrease];
	audioVolumeList[groupToDecrease] = currentVol - AUDIODEF_VOL_CHANGE < AUDIODEF_VOL_MIN ? AUDIODEF_VOL_MIN : currentVol - AUDIODEF_VOL_CHANGE;

	// printf("Audio: Decreased vol to %d\n", audioVolumeList[groupToDecrease]);
}

void AudioEnterMenu()
{
	// Enter a menu, pause etc
	AudioPause(AUDIO_GROUP_SFX);
	//CP_Sound_SetGroupPitch(AUDIO_GROUP_MUSIC, AUDIODEF_FADEOUT_TO);
	audioMusicFadeOutStarted = true;
	audioMusicFadeInStarted = false;
	percentage = 0;
}

void AudioExitMenu()
{
	// Exit a menu, back to game etc
	AudioUnpause(AUDIO_GROUP_SFX);
	//CP_Sound_SetGroupPitch(AUDIO_GROUP_MUSIC, AUDIODEF_FADEIN_TO);
	audioMusicFadeInStarted = true;
	audioMusicFadeOutStarted = false;
	percentage = 0;
}

void AudioMenuAnyButtonClicked()
{
	AudioPlay(AUDIO_TYPE_MENU_POP_UIFX);
}

void AudioMenuBackButtonClicked()
{
	AudioPlay(AUDIO_TYPE_MENU_BACK_UIFX);
}

void AudioSetVolume(AUDIO_GROUP groupToSet, int volume)
{
	audioVolumeList[groupToSet] = volume;
}

int AudioGetVolume(AUDIO_GROUP groupToGet)
{
	return audioVolumeList[groupToGet];
}
#pragma endregion


#pragma region Audio Private Functions

void AudioLoad(AUDIO_TYPE type, char const* fileName, AUDIO_GROUP group)
{
	audioObjList[type].soundFile = CP_Sound_Load(fileName);
	/*if (audioObjList[type].soundFile == NULL)
		printf("Audio: UNABLE TO LOAD \"%s\"!\n", fileName);*/

	audioObjList[type].audioGroup = group;
}

float AudioGetScaledVolume(AUDIO_TYPE type, float optionalInputVolume)
{
	// Ensure input volume is not 0 or < 0
	optionalInputVolume = optionalInputVolume > 0 ? optionalInputVolume : 1.0f;
	AUDIO_GROUP belongGroup = audioObjList[type].audioGroup;
	// Scale to current Group
	float finalVol = optionalInputVolume * (float)audioVolumeList[belongGroup] * AUDIODEF_VOL_NORM;
	// Scale to Master
	finalVol *= audioVolumeList[AUDIO_GROUP_MASTER] * AUDIODEF_VOL_NORM;
	return finalVol;
}

float AudioLerp(float start, float end, float percentage)
{
	return (1.f - percentage) * start + percentage * end;
}
#pragma endregion
