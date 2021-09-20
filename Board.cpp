/*
 * Board.cpp
 *
 *  Created on: Mar 11, 2021
 *      Author: tedon
 */
#include <iostream>
#include <fstream>
using namespace std;
#include "Board.h"

Board::Board() {
	// TODO Auto-generated constructor stub

}

Board::~Board() {
	// TODO Auto-generated destructor stub
}

void Board::initializeGameBoard(Cell gameBoard[][COLS]){
	int i = 0, j = 0;
	for (i = 0; i < ROWS; i++){
		for (j = 0; j < COLS; j++) {
			gameBoard[i][j].symbol=WATER;
			gameBoard[i][j].position.row=j;
			gameBoard[i][j].position.row=i;
		}
	}
}
void Board::printGameBoard (Cell gameBoard[][COLS], short showPegs) {
	int i = 0, j = 0;

	printf ("  0 1 2 3 4 5 6 7 8 9\n"); fflush(stdout);
	for (i = 0; i < ROWS; i++) {
		printf ("%d ", i); fflush(stdout);

		for (j = 0; j < COLS; j++) {
			if (showPegs == 1){
				printf ("%c ", gameBoard[i][j].symbol); fflush(stdout);
			}
			else if(showPegs == 3) {
				switch (gameBoard[i][j].symbol) {
					case HIT:
						printf ("%c ", HIT); fflush(stdout);
						break;
					case MISS:
						printf ("%c ", MISS); fflush(stdout);
						break;
					case WATER:
					default:
						printf ("%c ", WATER); fflush(stdout);
						break;
				}
			}
			//else { // don't show anything (showCPUboard is 0) }
		}

		putchar ('\n'); fflush(stdout);
	}
}
void Board::putShipOnGameBoard (Cell gameBoard[][COLS], Ship* ship, coordinate position, int direction) {
	int i = ship->getLength() - 1;

	for (i = 0; i < ship->getLength(); i++) {
		if (direction == HORIZONTAL){
			gameBoard[position.row][position.column + i].symbol = ship->getSymbol();
		}
		else{ /* VERTICAL */
			gameBoard[position.row + i][position.column].symbol = ship->getSymbol();
		}

	}
	printGameBoard(gameBoard, true);
}

/**
 * Function name : manuallyPlaceShipsOnGameBoard ()
 * Date Created  : 17 October 2012
 * Usage         : manuallyPlaceShipsOnGameBoard (Cell [][], Ship []);
 * Definition    : This function allows user to manually place ship on the specified
 *                 gameboard with specific ship
 * Pre-condition : Assuming user inputs correct coordinates as it does not check
 *                 if input is correctly inputted
 * Post-condition: Ships placed on game board
 */
void Board::manuallyPlaceShipsOnGameBoard (Cell gameBoard[][COLS], Ship* ship[]) {
	char       stringPosition[11] = "";

	coordinate position[5];
	bool    isValid = true;
	printGameBoard(gameBoard, true);
	//fflush (stdin);
	for (int i = 0; i < NUM_OF_SHIPS; i++) {
		while (isValid) {

			printf("Please enter the %d cells to place the %s across (no spaces):\n", ship[i]->getLength(), ship[i]->getName());
			fflush(stdout);
			printf("> ");
			fflush(stdout);
			cin>>stringPosition;

			/* convertStringtoPosition returns false if unsuccessful */
			if (convertStringtoPosition(position, stringPosition, ship[i]->getLength())) {

				isValid = true;

				for (int j = 0; j < ship[i]->getLength(); j++){

					if (gameBoard[position[j].row][position[j].column].symbol == WATER) {
						gameBoard[position[j].row][position[j].column].symbol = ship[i]->getSymbol();

					} else {
						isValid = false;
						printf ("> Invalid input!\n");

						if (j != 0){
							while (j >= 0) {
								gameBoard[position[j].row][position[j].column].symbol = WATER;
								j--;
							}
						}
						break;
					}
				}
			} else {
				isValid = false;
				printf ("> Invalid input!\n");fflush(stdout);
			}

			if (isValid == true){
				break;
			}
		}
		printGameBoard(gameBoard, true);
	}
}

/**
 * Function name : randomlyPlaceShipsOnGameBoard ()
 * Date Created  : 17 October 2012
 * Usage         : randomlyPlaceShipsOnGameBoard (Cell [][], Ship []);
 * Definition    : This function lets the computer randomly place ship on the
 *                 game board
 * Pre-condition : n/a
 * Post-condition: Ships placed on game board
 */
void Board::randomlyPlaceShipsOnGameBoard (Cell gameBoard[][COLS], Ship* ship[]) {
	coordinate position;
	int direction = -1;
	int i = 0;
	bool done=false;
	for (i = 0; i < NUM_OF_SHIPS; i++) {
		while (!done) {
			direction = getRandomNumber (0, 1); /* 0 -> horizontal, 1 -> vertical */
			//getRandNum is fine
			position = generatePosition (direction, ship[i]->getLength());
			//printf("GeneratePosition\n");
			if (isValidLocation(gameBoard, position, direction, ship[i]->getLength())){ //Error?
				//printf("Is valid location\n");
				done=true;
			}
		}
		done=false;

		putShipOnGameBoard(gameBoard, ship[i], position, direction);
		//printf("Put ship on game board\n"); fflush(stdout);
	}
}

/**
 * Function name : updateGameBoard ()
 * Date Created  : 17 October 2012
 * Usage         : updateGameBoard (Cell [][], coordinate);
 * Definition    : This function updates the game board whether
 *                 its a hit or miss
 * Pre-condition : n/a
 * Post-condition: Game board updated with proper symbol
 */
void Board::updateGameBoard (Cell gameBoard[][COLS], coordinate target) {
	switch (gameBoard[target.row][target.column].symbol) {
		/* miss */
		case WATER:
			gameBoard[target.row][target.column].symbol = MISS;
			break;

		/* hit */
		case CARRIER:
		case BATTLESHIP:
		case CRUISER:
		case SUBMARINE:
		case DESTROYER:
			gameBoard[target.row][target.column].symbol = HIT;
			break;

		case HIT:
		case MISS:
		default:
			break;
	}
}

/**
 * Function name : checkBoundsOfCardinal ()
 * Date Created  : 17 October 2012
 * Usage         : checkBoundsOfCardinal (bool [], int, int);
 * Definition    : This function checks if the specified direction is
 *                 not over the bounds and sets the cardinals with true
 *                 if its within bounds, otherwise false
 * Pre-condition : n/a
 * Post-condition: Updates the cardinals array
 */
void Board::checkBoundsOfCardinal (bool cardinals[], int bound, int direction) {
	switch (direction) {
		case NORTH:
			if (bound < 0)
				cardinals[0] = false;
			else
				cardinals[0] = true;
			break;

		case SOUTH:
			if (bound > 9)
				cardinals[1] = false;
			else
				cardinals[1] = true;
			break;

		case WEST:
			if (bound < 0)
				cardinals[2] = false;
			else
				cardinals[2] = true;
			break;

		case EAST:
			if (bound > 9)
				cardinals[3] = false;
			else
				cardinals[3] = true;
			break;
	}
}


/**
 * Function name : checkSunkShip ()
 * Date Created  : 17 October 2012
 * Usage         : checkSunkShip (shor [][], short, char, FILE *);
 * Definition    : This function check if a ship has sunk based on the
 *                 how of the length of ship is left. If a ship was sunk
 *                 it is logged on an output file
 * Pre-condition : stream to output file was created
 * Post-condition: n/a
 */
bool Board::checkSunkShip (short sunkShip[][NUM_OF_SHIPS], short player, char shipSymbol, FILE *stream) {
	bool sunked = false;

	switch (shipSymbol) {
		case CARRIER:
			if (--sunkShip[player][0] == 0) {
				printf ("> Player %d's Carrier sunked!\n", player + 1); fflush(stdout);

				/* Write to battleship.log */
				fprintf (stream, "Player %d's Carrier sunked!\n", player + 1); fflush(stdout);

				sunked = true;
			}
			break;

		case BATTLESHIP:
			if (--sunkShip[player][1] == 0) {
				printf ("> Player %d's Battleship sunked!\n", player + 1); fflush(stdout);

				/* Write to battleship.log */
				fprintf (stream, "Player %d's Battleship sunked!\n", player + 1); fflush(stdout);

				sunked = true;
			}
			break;

		case CRUISER:
			if (--sunkShip[player][2] == 0) {
				printf ("> Player %d's Cruiser sunked!\n", player + 1); fflush(stdout);

				/* Write to battleship.log */
				fprintf (stream, "Player %d's Cruiser sunked!\n", player + 1); fflush(stdout);

				sunked = true;
			}
			break;

		case SUBMARINE:
			if (--sunkShip[player][3] == 0) {
				printf ("> Player %d's Submarine sunked!\n", player + 1); fflush(stdout);

				/* Write to battleship.log */
				fprintf (stream, "Player %d's Submarine sunked!\n", player + 1); fflush(stdout);

				sunked = true;
			}
			break;

		case DESTROYER:
			if (--sunkShip[player][4] == 0) {
				printf ("> Player %d's Destroyer sunked!\n", player + 1); fflush(stdout);

				/* Write to battleship.log */
				fprintf (stream, "Player %d's Destroyer sunked!\n", player + 1); fflush(stdout);

				sunked = true;
			}
			break;
	}

	return sunked;
}

/**
 * Function name : isValidLocation ()
 * Date Created  : 17 October 2012
 * Usage         : isValidLocation (Cell [][], coordinate, int, int);
 * Definition    : This function checks if specified position, direction and
 *                 length is valid on location specified on the game board
 * Pre-condition : n/a
 * Post-condition: n/a
 */
bool Board::isValidLocation (Cell gameBoard[][COLS], coordinate position, int direction, int length) {
	bool isValid = true;
	//printf("Begin\n");  fflush(stdout);
	for (int i = 0; isValid && i < length; i++) {
		if (direction == HORIZONTAL) {
			if (gameBoard [position.row][position.column + i].symbol != WATER ||
				(position.column + i) >= COLS){
				isValid = false;
			}
		} else if (gameBoard [position.row + i][position.column].symbol != WATER ||
					(position.row + i) >= ROWS){
					isValid = false;
			}
		}
	/*if(isValid){
		printf("Here\n");fflush(stdout);
	}*/
	return isValid;
}

/**
 * Function name : convertStringtoPosition ()
 * Date Created  : 17 October 2012
 * Usage         : convertStringtoPosition (Cell [], char *, int);
 * Definition    : This function converts string coordinates into individual
 *                 integer coordinates
 * Pre-condition : Assuming that user inputs correct coordinates since program
 *                 does not check for inccorrect coordinates
 * Post-condition: n/a
 */
bool Board::convertStringtoPosition(coordinate position[], char *stringPosition, int length) {
	bool flag = true;
	int i = 0, j = 0, k = 1;

	/* checks if length of input is good */
	int stringLen=strlen(stringPosition);
	if ((stringLen)/2 == length) {
		/* loops through the length of the ship */
		for (i = 0; i < length && flag; i++) {
			/* checks if each cell is a digit */
			if (isdigit (stringPosition[j]) && isdigit (stringPosition[k])) {
				position[i].row    = stringPosition[j] - '0';
				position[i].column = stringPosition[k] - '0';

				j += 2;
				k += 2;
			} else {
				flag = false;
			}
		}
	} else {
		flag = false;
	}

	return flag;
}


int Board::getRandomNumber (int lowest, int highest) {
	int num=0;
	if (lowest == 0){
		num=rand () % ++highest;
	}
	if (lowest > 0){
		num = rand () % ++highest + lowest;
	}
	return num;
}
/**
 * Function name : generatePosition ()
 * Date Created  : 17 October 2012
 * Usage         : generatePosition (int, int);
 * Definition    : This function generates position based on the
 *                 direction and length specified and it can't be
 *                 more than the game board size
 * Pre-condition : n/a
 * Post-condition: n/a
 */
coordinate Board::generatePosition (int direction, int length) {
	coordinate position;

	if (direction == HORIZONTAL) {
		position.row    = getRandomNumber (0, ROWS);
		position.column = getRandomNumber (0, COLS - length);
	} else { /* VERTICAL */
		position.row    = getRandomNumber (0, ROWS - length);
		position.column = getRandomNumber (0, COLS);
	}

	return position;
}

/**
 * Function name : getTarget ()
 * Date Created  : 17 October 2012
 * Usage         : getTarget ();
 * Definition    : This function reads a row and column values from
 *                 the user
 * Pre-condition : n/a
 * Post-condition: n/a
 */
coordinate Board::getTarget() {
	coordinate target;

	fflush (stdin);

	printf ("> Enter Target (ex. > 3 4):\n"); fflush(stdout);
	printf ("> "); fflush(stdout);
	scanf ("%d %d", &target.row, &target.column);

	return target;
}

/**
 * Function name : checkShot ()
 * Date Created  : 17 October 2012
 * Usage         : checkShot (Cell [][], coordinate);
 * Definition    : This function checks if the coordinates on the
 *                 game board is a miss, hit, water or water craft
 * Pre-condition : n/a
 * Post-condition: n/a
 */
short Board::checkShot (Cell gameBoard[][COLS], coordinate target) {
	int hit = -2;

	switch (gameBoard[target.row][target.column].symbol) {
		/* miss */
		case WATER:
			hit = 0;
			break;

		/* hit */
		case CARRIER:
		case BATTLESHIP:
		case CRUISER:
		case SUBMARINE:
		case DESTROYER:
			hit = 1;
			break;

		case HIT:
		case MISS:
		default:
			hit = -1;
			break;
	}

	return hit;
}
