/*!
@copyright	DIGIPEN INSTITUTE OF TECHNOLOGY SINGAPORE
@file 		PlayerStatusUpgrade.c
@author 	Gavin Ang Jun Liang(ang.g)
@course 	csd1401f23
@section 	A
@date 		18/11/2023
@brief 		This file contains the function definitions for
			the calculations of the persistant upgrades in the game.
*//*______________________________________________________________________*/
#include "PlayerStatusUpgrade.h"

//function will called to count the number of times the HP upgrade has been clicked
void AddPlayerHealth(int healthCounter)
{
	//takes in the counter from the player
	newHealthCounter = healthCounter;
}

//function will called to count the number of times the Attack upgrade has been clicked
void AddPlayerAttack(int attackCounter)
{
	//takes in the counter from the player
	newAttackCounter = attackCounter;
}

//function will called to count the number of times the Speed upgrade has been clicked
void AddPlayerSpeed(int speedCounter)
{
	//takes in the counter from the player
	newSpeedCounter = speedCounter;
}

//function will called to confirm the selected upgrades and then write them into their respective save file
void ConfirmButton()
{
	//saves the counter into the player file
	WritePlayerTextFile(newHealthCounter, newAttackCounter, newSpeedCounter);

	//saves and checks the amount of currency the player has left after upgrading
	WritePlayerCurrencyTextFile((-upgradeCost));
}

//used when there is aneed to calculate the player statistics after upgrading
void CalculatePlayerStats()
{
	//base stats of the player
	int initialHealth = 100;
	float initialAttack = 1.0f;
	int initialSpeed = 200;
	
	//reading the player file to get the upgraded stats counter
	ReadPlayerTextFile();
	
	//calculates the new stats the player will have using the initial stats + the new upgraded amount
	player->healthMaximum = (float)initialHealth + (float)(healthTempCounter * (initialHealth / 5));
	damageModifer = initialAttack + ((float)attackTempCounter * (initialAttack / 5.0f));
	player->speed = (float)initialSpeed + (float)(speedTempCounter * (100 / 5));
}

//function is used when calculating the total cost of the upgrades
void CalculateCost(int upgradeCounter)
{
	int costPerUpgrade = 250;
	upgradeCost = upgradeCounter * costPerUpgrade;

}

//function fully resets the player's stats and also refunds all the currecy used when upgrading
void ResetButton()
{
	//base for everything before calculating
	int totalCounter = 0, totalCostRefund = 0, costPerUpgrade = 250;

	//reads the player file to see how much the player has upgraded
	ReadPlayerTextFile();

	//counter to count how many points in total the player has upgraded into
	totalCounter = healthTempCounter + attackTempCounter + speedTempCounter;

	//calculates the total currency the player has used to be refunded
	totalCostRefund = totalCounter * costPerUpgrade;

	//write to player file to reset all upgrades to 0 to have initial stats
	WritePlayerTextFile(0, 0, 0);

	//write to the player currency file to refund their currency used
	WritePlayerCurrencyTextFile(totalCostRefund);

}