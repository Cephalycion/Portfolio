/*!
@copyright	DIGIPEN INSTITUTE OF TECHNOLOGY SINGAPORE
@file 		Read.c
@author 	Gavin Ang Jun Liang(ang.g)
@course 	csd1401f23
@section 	A
@date 		18/11/2023
@brief 		This file contains the function definations for
			saving and loading of game data in the game.
*//*______________________________________________________________________*/

#include "ReadFile.h"


//function write to the player file the player's status
void WritePlayerTextFile(int health, int attack, int speed)
{
	FILE* fileName;

	//Declaring a variable of type errno_t to store the return value of fopen_s
	errno_t error_code;

	//Opening file in w mode
	//opens the file to write into it and if the file does not exists it creates a new file
	error_code = fopen_s(&fileName, "playerStatus.txt", "w");

	//checks that the file is not equal to null
	if (error_code != 0) {
		// printf("Error! Failed to open !");
	}
	else
	{
		//prints the new value into the file
		fprintf(fileName, "%d %d %d\n", health, attack, speed);

		//Closing the files
		fclose(fileName);
	}
}

//function reads the player's status from the player file
void ReadPlayerTextFile(void)
{
	FILE* fileName;

	//Declaring a variable of type errno_t to store the return value of fopen_s
	errno_t error_code;

	//Opening file in r mode
	//r mode only reads the file
	error_code = fopen_s(&fileName, "playerStatus.txt", "r");
	if (error_code != 0) {
		// printf("Error! Failed to open !");

		//saving player data to text file
		WritePlayerTextFile(0, 0, 0);
	}
	else
	{
		//scanfs the content of the file and sets them into temp holders
		fscanf_s(fileName, "%d %d %d\n", &healthTempCounter, &attackTempCounter, &speedTempCounter);
		//Closing the files
		fclose(fileName);
	}
	
}

//function write to the player currency file the player's currency earned/used
void WritePlayerCurrencyTextFile(int currency)
{
	FILE* fileName;

	//Declaring a variable of type errno_t to store the return value of fopen_s
	errno_t error_code;

	//Opening file in w mode
	//opens the file to write into it and if the file does not exists it creates a new file
	error_code = fopen_s(&fileName, "playerCurrency.txt", "w");

	//checks that the file is not equal to null
	if (error_code != 0) {
		// printf("Error! Failed to open !");
	}
	else
	{
		int newCurrency = currencyTemp + currency;
		//prints the new value into the file
		fprintf(fileName, "%d\n", newCurrency);

		//Closing the files
		fclose(fileName);
	}
}

//function reads the player's currency from the player currency file
void ReadPlayerCurrencyTextFile(void)
{
	FILE* fileName;

	//Declaring a variable of type errno_t to store the return value of fopen_s
	errno_t error_code;

	//Opening file in r mode
	//r mode only reads the file
	error_code = fopen_s(&fileName, "playerCurrency.txt", "r");
	if (error_code != 0) {
		// printf("Error! Failed to open !");

		//saving player currency to text file
		WritePlayerCurrencyTextFile(0);
	}
	else
	{
		//scanfs the content of the file and sets them into temp holders
		fscanf_s(fileName, "%d\n", &currencyTemp);
		//Closing the files
		fclose(fileName);
	}
}

//function write to the audio file the volume adjusted
void WriteAudioTextFile(int masterVolume)
{
	FILE* fileName;

	//Declaring a variable of type errno_t to store the return value of fopen_s
	errno_t error_code;

	//Opening file in w mode
	//opens the file to write into it and if the file does not exists it creates a new file
	error_code = fopen_s(&fileName, "audioFile.txt", "w");

	//checks that the file is not equal to null
	if (error_code != 0) {
		// printf("Error! Failed to open !");
	}
	else
	{
		//prints the new value into the file
		fprintf(fileName, "%d\n", masterVolume);
		masterVolumeTemp = masterVolume;

		//Closing the files
		fclose(fileName);
	}
}

//function reads the adjusted volume from the audio file
void ReadAudioTextFile(void)
{
	FILE* fileName;

	//Declaring a variable of type errno_t to store the return value of fopen_s
	errno_t error_code;

	//Opening file in r mode
	//r mode only reads the file
	error_code = fopen_s(&fileName, "audioFile.txt", "r");
	if (error_code != 0) {
		// printf("Error! Failed to open !");
		WriteAudioTextFile(10);
	}
	else
	{
		//scanfs the content of the file and sets them into temp holders
		fscanf_s(fileName, "%d\n", &masterVolumeTemp);
		//Closing the files
		fclose(fileName);
	}
}