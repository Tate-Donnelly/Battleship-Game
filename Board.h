/*
 * Board.h
 *
 *  Created on: Mar 9, 2021
 *      Author: tedon
 */


#ifndef BOARD_H_
#define BOARD_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <curses.h>
#include "Ship.h"
#include "Cruiser.h"
#include "Destroyer.h"
#include "BS.h"
#include "Carrier.h"
#include "Submarine.h"
#define ROWS 10
#define COLS 10

#define LOG_FILE_NAME "battleship.log"

#define NUM_OF_SHIPS  5

#define HORIZONTAL    0
#define VERTICAL      1

#define PLAYER_ONE    0
#define PLAYER_TWO    1


#define WATER         '~'
#define HIT           '*'
#define MISS          'm'

#define CARRIER       'c'
#define BATTLESHIP    'b'
#define CRUISER       'r'
#define SUBMARINE     's'
#define DESTROYER     'd'

#define NORTH 0
#define SOUTH 1
#define WEST  2
#define EAST  3
/*
typedef enum {
	CARRIER_L    = 5,
	BATTLESHIP_L = 4,
	CRUISER_L    = 3,
	SUBMARINE_L  = 3,
	DESTROYER_L  = 2
} ShipType;
*/

typedef struct stats {
	int        numHits;
	int        numMisses;
	int        totalShots;
	double     hitMissRatio;
} Stats;

typedef struct coordinate {
	int        row;
	int        column;
} coordinate;

typedef struct cell {
	char       symbol;
	coordinate position;
} Cell;
class Board {
public:
	Board();
	virtual ~Board();
	void initializeGameBoard(Cell gameBoard[][COLS]);
	void printGameBoard(Cell gameBoard [][COLS], short showPegs);
	void putShipOnGameBoard(Cell gameBoard[][COLS], Ship* ship, coordinate position, int direction);
	void manuallyPlaceShipsOnGameBoard(Cell gameBoard[][COLS], Ship* ship[]);
	void randomlyPlaceShipsOnGameBoard(Cell gameBoard[][COLS], Ship* ship[]);
	void updateGameBoard(Cell gameBoard[][COLS], coordinate target);
	void checkBoundsOfCardinal(bool cardinals[], int bound, int direction);
	bool checkSunkShip(short sunkShip[][NUM_OF_SHIPS], short player, char shipSymbol, FILE *stream);
	bool isValidLocation(Cell gameBoard[][COLS], coordinate position, int direction, int length);
	bool convertStringtoPosition(coordinate position[], char *stringPosition, int length);
	coordinate generatePosition(int direction, int length);
	coordinate getTarget();
	short checkShot(Cell gameBoard[][COLS], coordinate target);
	int getRandomNumber(int lowest, int highest);
};

#endif /* BOARD_H_ */
