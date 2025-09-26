/*!
@copyright	DIGIPEN INSTITUTE OF TECHNOLOGY SINGAPORE
@file 		PlayerStatusUpgrade.h
@author 	Gavin Ang Jun Liang(ang.g)
@course 	csd1401f23
@section 	A
@date 		18/11/2023
@brief 		This file contains the function declartions for
			the calculations of the persistant upgrades in the game.
*//*______________________________________________________________________*/

#include <stdio.h>
#include "Game.h"
#include "ReadFile.h"

int newHealthCounter, newAttackCounter, newSpeedCounter;
//function will called to count the number of times the HP upgrade has been clicked
void AddPlayerHealth(int healthCounter);

//function will called to count the number of times the Attack upgrade has been clicked
void AddPlayerAttack(int attackCounter);

//function will called to count the number of times the Speed upgrade has been clicked
void AddPlayerSpeed(int speedCounter);

//function will called to confirm the selected upgrades and then write them into their respective save file
void ConfirmButton(void);

//used when there is aneed to calculate the player statistics after upgrading
void CalculatePlayerStats(void);

int upgradeCost;
//function is used when calculating the total cost of the upgrades
void CalculateCost(int upgradeCounter);


//function fully resets the player's stats and also refunds all the currecy used when upgrading
void ResetButton();