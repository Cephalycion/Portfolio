#include "monsters.h"

// array of struct SMapMonster known as monsterList
SMapMonster monsterList[50];

int monsterCollision(int Y, int X, char(*myArray)[80], bool button0, bool button2, bool button4, bool button6, bool button8)
{
	if (((button0 == 1) && (myArray[Y][X] == '1')) ||// allows players to walk on hidden paths/doors after buttons are pressed
		((button2 == 1) && (myArray[Y][X] == '3')) ||
		((button4 == 1) && (myArray[Y][X] == '5')) ||
		((button6 == 1) && (myArray[Y][X] == '7')) ||
		((button8 == 1) && (myArray[Y][X] == '9')))
	{
		return 1;
	}

	if ((myArray[Y][X] == '0') ||
		(myArray[Y][X] == '2') ||
		(myArray[Y][X] == '4') ||
		(myArray[Y][X] == '6') ||
		(myArray[Y][X] == '8'))
	{
		return 1;
	}

	// if the player is not stepping on a wall or numbers (numbers are used to denote the buttons and paths),
	// return a 1.
	if (((myArray[Y][X] < '0') || (myArray[Y][X] > '9')) && (myArray[Y][X] != '#'))
	{
		return 1;
	}
	return 0;
}

// 0 Down 1 Up 2 Left 3 Right 4 Reset
// function for moving monsters up and down
void moveUpDownMonster(SMapMonster &monster, bool &somethingHappened, char(*myArray)[80], bool button0, bool button2, bool button4, bool button6, bool button8)
{
	switch (monster.m_iDirection)
	{
	case 0:
		if (monsterCollision(monster.m_cLocation.Y + 1, monster.m_cLocation.X, myArray, button0, button2, button4, button6, button8) == 1)
		{
			monster.m_cLocation.Y++;
			somethingHappened = true;
		}
		else
		{
			monster.m_iDirection = 1;
		}
		break;
	case 1:
		if (monsterCollision(monster.m_cLocation.Y - 1, monster.m_cLocation.X, myArray, button0, button2, button4, button6, button8) == 1)
		{
			monster.m_cLocation.Y--;
			somethingHappened = true;
		}
		else
		{
			monster.m_iDirection = 0;
		}
		break;
	case 4:
		if (myArray[monster.m_cLocation.Y + 1][monster.m_cLocation.X] != '#')
		{
			monster.m_iDirection = 0;
		}
		else
		{
			monster.m_iDirection = 1;
		}
		break;
	}
}

// function for moving monsters left and right
void moveLeftRightMonster(SMapMonster &monster, bool &somethingHappened, char(*myArray)[80], bool button0, bool button2, bool button4, bool button6, bool button8)
{
	switch (monster.m_iDirection)
	{
	case 2:
		if (monsterCollision(monster.m_cLocation.Y, monster.m_cLocation.X - 1, myArray, button0, button2, button4, button6, button8) == 1)
		{
			monster.m_cLocation.X--;
			somethingHappened = true;
		}
		else
		{
			monster.m_iDirection = 3;
		}
		break;
	case 3:
		if (monsterCollision(monster.m_cLocation.Y, monster.m_cLocation.X + 1, myArray, button0, button2, button4, button6, button8) == 1)
		{
			monster.m_cLocation.X++;
			somethingHappened = true;
		}
		else
		{
			monster.m_iDirection = 2;
		}
		break;
	case 4:
		if (myArray[monster.m_cLocation.Y][monster.m_cLocation.X - 1] != '#')
		{
			monster.m_iDirection = 2;
		}
		else
		{
			monster.m_iDirection = 3;
		}
		break;
	}
}

// function for moving monsters clockwise
void moveClockwiseMonster(SMapMonster &monster, bool &somethingHappened, char(*myArray)[80], bool button0, bool button2, bool button4, bool button6, bool button8)
{
	switch (monster.m_iDirection)
	{
	case 0:
		if (monsterCollision(monster.m_cLocation.Y + 1, monster.m_cLocation.X, myArray, button0, button2, button4, button6, button8) == 1)
		{
			monster.m_cLocation.Y++;
			somethingHappened = true;
		}
		else
		{
			monster.m_iDirection = 2;
		}
		break;
	case 1:
		if (monsterCollision(monster.m_cLocation.Y - 1, monster.m_cLocation.X, myArray, button0, button2, button4, button6, button8) == 1)
		{
			monster.m_cLocation.Y--;
			somethingHappened = true;
		}
		else
		{
			monster.m_iDirection = 3;
		}
		break;
	case 2:
		if (monsterCollision(monster.m_cLocation.Y, monster.m_cLocation.X - 1, myArray, button0, button2, button4, button6, button8) == 1)
		{
			monster.m_cLocation.X--;
			somethingHappened = true;
		}
		else
		{
			monster.m_iDirection = 1;
		}
		break;
	case 3:
		if (monsterCollision(monster.m_cLocation.Y, monster.m_cLocation.X + 1, myArray, button0, button2, button4, button6, button8) == 1)
		{
			monster.m_cLocation.X++;
			somethingHappened = true;
		}
		else
		{
			monster.m_iDirection = 0;
		}
		break;
	case 4:
		if (myArray[monster.m_cLocation.Y][monster.m_cLocation.X - 1] == '#' && myArray[monster.m_cLocation.Y + 1][monster.m_cLocation.X] == '#')
		{
			monster.m_iDirection = 1;
		}
		if (myArray[monster.m_cLocation.Y][monster.m_cLocation.X - 1] == '#' && myArray[monster.m_cLocation.Y - 1][monster.m_cLocation.X] == '#')
		{
			monster.m_iDirection = 3;
		}
		if (myArray[monster.m_cLocation.Y - 1][monster.m_cLocation.X] == '#' && myArray[monster.m_cLocation.Y][monster.m_cLocation.X + 1] == '#')
		{
			monster.m_iDirection = 0;
		}
		if (myArray[monster.m_cLocation.Y + 1][monster.m_cLocation.X] == '#' && myArray[monster.m_cLocation.Y][monster.m_cLocation.X + 1] == '#')
		{
			monster.m_iDirection = 2;
		}
		// 0 Down 1 Up 2 Left 3 Right 4 Reset
		// up, right, down, left, nothing
		if (myArray[monster.m_cLocation.Y - 1][monster.m_cLocation.X] == '#' && myArray[monster.m_cLocation.Y][monster.m_cLocation.X + 1] != '#' && myArray[monster.m_cLocation.Y + 1][monster.m_cLocation.X] != '#' && myArray[monster.m_cLocation.Y][monster.m_cLocation.X - 1] != '#')
		{
			monster.m_iDirection = 3;
		}
		if (myArray[monster.m_cLocation.Y - 1][monster.m_cLocation.X] != '#' && myArray[monster.m_cLocation.Y][monster.m_cLocation.X + 1] == '#' && myArray[monster.m_cLocation.Y + 1][monster.m_cLocation.X] != '#' && myArray[monster.m_cLocation.Y][monster.m_cLocation.X - 1] != '#')
		{
			monster.m_iDirection = 0;
		}
		if (myArray[monster.m_cLocation.Y - 1][monster.m_cLocation.X] != '#' && myArray[monster.m_cLocation.Y][monster.m_cLocation.X + 1] != '#' && myArray[monster.m_cLocation.Y + 1][monster.m_cLocation.X] == '#' && myArray[monster.m_cLocation.Y][monster.m_cLocation.X - 1] != '#')
		{
			monster.m_iDirection = 2;
		}
		if (myArray[monster.m_cLocation.Y - 1][monster.m_cLocation.X] != '#' && myArray[monster.m_cLocation.Y][monster.m_cLocation.X + 1] != '#' && myArray[monster.m_cLocation.Y + 1][monster.m_cLocation.X] != '#' && myArray[monster.m_cLocation.Y][monster.m_cLocation.X - 1] == '#')
		{
			monster.m_iDirection = 1;
		}
		if (myArray[monster.m_cLocation.Y - 1][monster.m_cLocation.X] != '#' && myArray[monster.m_cLocation.Y][monster.m_cLocation.X + 1] != '#' && myArray[monster.m_cLocation.Y + 1][monster.m_cLocation.X] != '#' && myArray[monster.m_cLocation.Y][monster.m_cLocation.X - 1] != '#')
		{
			monster.m_iDirection = 3;
		}
		break;
	}
}

// function for moving monsters anticlockwise
void moveAntiClockwiseMonster(SMapMonster &monster, bool &somethingHappened, char(*myArray)[80], bool button0, bool button2, bool button4, bool button6, bool button8)
{
	switch (monster.m_iDirection)
	{
	case 0:
		if (monsterCollision(monster.m_cLocation.Y + 1, monster.m_cLocation.X, myArray, button0, button2, button4, button6, button8) == 1)
		{
			monster.m_cLocation.Y++;
			somethingHappened = true;
		}
		else
		{
			monster.m_iDirection = 3;
		}
		break;
	case 1:
		if (monsterCollision(monster.m_cLocation.Y - 1, monster.m_cLocation.X, myArray, button0, button2, button4, button6, button8) == 1)
		{
			monster.m_cLocation.Y--;
			somethingHappened = true;
		}
		else
		{
			monster.m_iDirection = 2;
		}
		break;
	case 2:
		if (monsterCollision(monster.m_cLocation.Y, monster.m_cLocation.X - 1, myArray, button0, button2, button4, button6, button8) == 1)
		{
			monster.m_cLocation.X--;
			somethingHappened = true;
		}
		else
		{
			monster.m_iDirection = 0;
		}
		break;
	case 3:
		if (monsterCollision(monster.m_cLocation.Y, monster.m_cLocation.X + 1, myArray, button0, button2, button4, button6, button8) == 1)
		{
			monster.m_cLocation.X++;
			somethingHappened = true;
		}
		else
		{
			monster.m_iDirection = 1;
		}
		break;
	case 4:
		if (myArray[monster.m_cLocation.Y][monster.m_cLocation.X - 1] == '#' && myArray[monster.m_cLocation.Y + 1][monster.m_cLocation.X] == '#')
		{
			monster.m_iDirection = 3;
		}
		if (myArray[monster.m_cLocation.Y][monster.m_cLocation.X - 1] == '#' && myArray[monster.m_cLocation.Y - 1][monster.m_cLocation.X] == '#')
		{
			monster.m_iDirection = 0;
		}
		if (myArray[monster.m_cLocation.Y - 1][monster.m_cLocation.X] == '#' && myArray[monster.m_cLocation.Y][monster.m_cLocation.X + 1] == '#')
		{
			monster.m_iDirection = 2;
		}
		if (myArray[monster.m_cLocation.Y + 1][monster.m_cLocation.X] == '#' && myArray[monster.m_cLocation.Y][monster.m_cLocation.X + 1] == '#')
		{
			monster.m_iDirection = 1;
		}
		// 0 Down 1 Up 2 Left 3 Right 4 Reset
		// up, right, down, left, nothing
		if (myArray[monster.m_cLocation.Y - 1][monster.m_cLocation.X] == '#' && myArray[monster.m_cLocation.Y][monster.m_cLocation.X + 1] != '#' && myArray[monster.m_cLocation.Y + 1][monster.m_cLocation.X] != '#' && myArray[monster.m_cLocation.Y][monster.m_cLocation.X - 1] != '#')
		{
			monster.m_iDirection = 2;
		}
		if (myArray[monster.m_cLocation.Y - 1][monster.m_cLocation.X] != '#' && myArray[monster.m_cLocation.Y][monster.m_cLocation.X + 1] == '#' && myArray[monster.m_cLocation.Y + 1][monster.m_cLocation.X] != '#' && myArray[monster.m_cLocation.Y][monster.m_cLocation.X - 1] != '#')
		{
			monster.m_iDirection = 1;
		}
		if (myArray[monster.m_cLocation.Y - 1][monster.m_cLocation.X] != '#' && myArray[monster.m_cLocation.Y][monster.m_cLocation.X + 1] != '#' && myArray[monster.m_cLocation.Y + 1][monster.m_cLocation.X] == '#' && myArray[monster.m_cLocation.Y][monster.m_cLocation.X - 1] != '#')
		{
			monster.m_iDirection = 3;
		}
		if (myArray[monster.m_cLocation.Y - 1][monster.m_cLocation.X] != '#' && myArray[monster.m_cLocation.Y][monster.m_cLocation.X + 1] != '#' && myArray[monster.m_cLocation.Y + 1][monster.m_cLocation.X] != '#' && myArray[monster.m_cLocation.Y][monster.m_cLocation.X - 1] == '#')
		{
			monster.m_iDirection = 0;
		}
		if (myArray[monster.m_cLocation.Y - 1][monster.m_cLocation.X] != '#' && myArray[monster.m_cLocation.Y][monster.m_cLocation.X + 1] != '#' && myArray[monster.m_cLocation.Y + 1][monster.m_cLocation.X] != '#' && myArray[monster.m_cLocation.Y][monster.m_cLocation.X - 1] != '#')
		{
			monster.m_iDirection = 2;
		}
		break;
	}
}

// this function gets the type of monster it is, there are 4 types
// this is put into the struct variable "cType"
void getMonsterXYType(char(*myArray)[80])
{
	int i = 0;

	for (int y = 0; y < 25; y++)
	{
		for (int x = 0; x < 80; x++)
		{
			switch (myArray[y][x])
			{
			case '+':
				monsterList[i].m_cLocation.Y = y;
				monsterList[i].m_cLocation.X = x;
				monsterList[i].m_cType = '+';
				monsterList[i].m_iDirection = 4;
				i++;
				break;
			case '=':
				monsterList[i].m_cLocation.Y = y;
				monsterList[i].m_cLocation.X = x;
				monsterList[i].m_cType = '=';
				monsterList[i].m_iDirection = 4;
				i++;
				break;
			case '$':
				monsterList[i].m_cLocation.Y = y;
				monsterList[i].m_cLocation.X = x;
				monsterList[i].m_cType = '$';
				monsterList[i].m_iDirection = 4;
				i++;
				break;
			case '%':
				monsterList[i].m_cLocation.Y = y;
				monsterList[i].m_cLocation.X = x;
				monsterList[i].m_cType = '%';
				monsterList[i].m_iDirection = 4;
				i++;
			}
		}
	}
}

// this is to clear the monsters
void clearMonsters(char(*myArray)[80])
{
	for (int i = 0; i < 50; i++)
	{
		monsterList[i].m_cLocation = { 0, };
		monsterList[i].m_iDirection = 0;
		monsterList[i].m_cType = 0;
	}

	for (int i = 0; i < 25; i++)
	{
		for (int j = 0; j < 80; j++)
		{
			myArray[i][j] = 0;
		}
	}
}

// this is for rendering the monsters
// it scans through the array of monsters and renders them based on their coord and type
void renderMonsters(class Console &g_Console)
{
	WORD monsterColor = FOREGROUND_RED;
	for (int i = 0; i < 50; i++)
	{
		g_Console.writeToBuffer(monsterList[i].m_cLocation, (char)219, monsterColor);
	}
}

// this is the function to move monsters, based on their type they move in certain directions
void moveMonsters(char(*myArray)[80], double &g_dMonsterTime, double g_dElapsedTime, bool button0, bool button2, bool button4, bool button6, bool button8)
{
	bool bSomethingHappened = false;
	if (g_dMonsterTime > g_dElapsedTime)
		return;

	for (int i = 0; i < 50; i++)
	{
		switch (monsterList[i].m_cType)
		{
		case '+':
			moveUpDownMonster(monsterList[i], bSomethingHappened, myArray, button0, button2, button4, button6, button8);
			break;
		case '=':
			moveLeftRightMonster(monsterList[i], bSomethingHappened, myArray, button0, button2, button4, button6, button8);
			break;
		case '$':
			moveClockwiseMonster(monsterList[i], bSomethingHappened, myArray, button0, button2, button4, button6, button8);
			break;
		case '%':
			moveAntiClockwiseMonster(monsterList[i], bSomethingHappened, myArray, button0, button2, button4, button6, button8);
			break;
		}

		if (bSomethingHappened)
		{
			g_dMonsterTime = g_dElapsedTime + 0.1;
		}
	}
}