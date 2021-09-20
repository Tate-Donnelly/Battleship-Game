/*
 * Production.h
 *
 *  Created on: Mar 11, 2021
 *      Author: tedon
 */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "Board.h"
#include "Cruiser.h"
#include "Destroyer.h"
#include "BS.h"
#include "Carrier.h"
#include "Submarine.h"
#ifndef PRODUCTION_H_
#define PRODUCTION_H_
#define LOG_FILE_NAME "battleship.log"

class Production {
public:
	Production();
	virtual ~Production();
	bool prod(int argc, char* argv[]);
	void welcomeScreen();
	void systemMessage (char* message);
	bool isWinner(Stats players[], short sunkShips[][5], int player);
};

#endif /* PRODUCTION_H_ */
