/*!
@copyright	DIGIPEN INSTITUTE OF TECHNOLOGY SINGAPORE
@file 		Read.h
@author 	Gavin Ang Jun Liang(ang.g)
@course 	csd1401f23
@section 	A
@date 		18/11/2023
@brief 		This file contains the function declartions for
			saving and loading of game data in the game.
*//*______________________________________________________________________*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Game.h"

int healthTempCounter, attackTempCounter, speedTempCounter;
//write to the player file the player's status
void WritePlayerTextFile(int health, int attack, int speed);
//reads the player's status from the player file
void ReadPlayerTextFile(void);

int currencyTemp;
//write to the player currency file the player's currency earned/used
void WritePlayerCurrencyTextFile(int currency);
//reads the player's currency from the player currency file
void ReadPlayerCurrencyTextFile(void);

int masterVolumeTemp, sfxVolumeTemp, bgmVolumeTemp;
//write to the audio file the volume adjusted
void WriteAudioTextFile(int masterVolume);
//reads the adjusted volume from the audio file
void ReadAudioTextFile(void);