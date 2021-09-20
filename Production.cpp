/*
 * Production.cpp
 *
 *  Created on: Mar 11, 2021
 *      Author: tedon
 */
#include <iostream>
#include <fstream>
using namespace std;
#include "Production.h"
Production::Production() {
	// TODO Auto-generated constructor stub

}

Production::~Production() {
	// TODO Auto-generated destructor stub
}
bool Production::prod(int argc, char* argv[]){
	bool answer=false;
	short cpuBoardDisplayStatus=0;
	if(argc <=1) //no interesting information
	{
		puts("Didn't find any arguments.");
		fflush(stdout);
		answer = false;
	}
	else //there is interesting information
	{

		printf("Found %d interesting arguments.\n", argc-1);
		fflush(stdout);
		for(int i = 1; i<argc; i++) //don't want to read argv[0]
		{//argv[i] is a string
			switch(i){
				case 1:
				{
					char* temp1;
					int temp2=(int) strtol(argv[i], &temp1, 10);
					if(temp2==1){
						cpuBoardDisplayStatus += 1;
						printf("Player can see CPU's board\n");
					}else if(temp2 == 0) {
						printf("Player can't see CPU's board\n");
					}
					break;
				}
				case 2:
				{
					char* temp1;
					int temp2=(int) strtol(argv[i], &temp1, 10);
					if(temp2 == 1) {
						printf("Player can see CPU hit/miss board\n");
						cpuBoardDisplayStatus += 2;
					}
				}
				default:
				{
					puts("Unexpected argument count."); fflush(stdout);
					answer = false;
					break;
				}
		}
	}
	Stats players[2] = {{0, 0, 0, 0.0}, {0, 0, 0, 0.0}};

	Cell playerOneGameBoard[ROWS][COLS];       /* Player one game board */
	Cell playerTwoGameBoard[ROWS][COLS];       /* Player two game board */

	coordinate target;             /* x, y value of a target */
	coordinate targetTemp;         /* x, y value that holds a temporary value*/
	coordinate targetOrigin;       /* x, y value of the original target */
	coordinate targetAI;/* x, y value of the targets using AI technique */

	Carrier* carrier=new Carrier();
	carrier->setName("Carrier");
	carrier->setSymbol('c');
	carrier->setLength(5);
	//0001020304
	BS* bs= new BS();
	bs->setLength(4);
	bs->setName("Battleship");
	bs->setSymbol('b');
	//10111213
	Cruiser* cruiser=new Cruiser();
	cruiser->setName("Cruiser");
	cruiser->setLength(3);
	cruiser->setSymbol('r');
	//202122
	Submarine* sub=new Submarine();
	sub->setLength(3);
	sub->setName("Submarine");
	sub->setSymbol('s');
	//303132
	Destroyer* des=new Destroyer;
	des->setLength(2);
	des->setName("Destroyer");
	des->setSymbol('d');
	//4041
	Ship* ship[NUM_OF_SHIPS] = {carrier,
							   bs,
							   cruiser,
							   sub,
							   des};

	bool huntMode       = true;                     /* mode of randomly selecting a target */
	bool targetMode     = false;                    /* mode when there is a hit */
	bool flipper        = true;	                  /* flips values of boolean */
	bool cardinals[4]   = {true, true, true, true}; /* represents the 4 cardinals, N, S, W, E */
	bool hasAShipSunked = false;                    /* if a ship has sank */


	short sunkShip[2][NUM_OF_SHIPS] = {{5, 4, 3, 3, 2},
									   {5, 4, 3, 3, 2}};  /* tracks parts of the ship destroyed */

	short player  = 0;	           /* 0 -> player1, 1 -> player2 */
	short shot    = 0;             /* holds temp value if ship has been shot */
	int   north   = 0,             /* holds change of value when going north */
		  south   = 0,             /* holds change of value when going south */
		  east    = 0,             /* holds change of value when going east */
		  west    = 0;             /* holds change of value when going west */
	int   i       = 0,
		  counter = 1;             /* i and counter are used as counters */

	char  shipSymbol = '\0';       /* temporary value to save character symbol of the ship */

	FILE *outStream = NULL;        /* stream to output file battleship.log */

	/* Start of Program */
	outStream = fopen (LOG_FILE_NAME, "w");

	srand ((unsigned int) time (NULL));

	/**
	 * Print welcome screen
	 */
	welcomeScreen();
//
//	/**
//	 * Each Player's game board should be initialized to all '~' indicating
//	 * no ships have been placed on either board.
//	 */
	Board* p1=new Board();
	Board* p2=new Board();
	p1->initializeGameBoard(playerOneGameBoard);
	p2->initializeGameBoard(playerTwoGameBoard);
	int   option  = 0;/* option for player to place ship manually or randomly */
	printf ("> Please select from the following menu:\n");
	printf ("> [1] Manually\n");
	printf ("> [2] Randomly\n");
	printf ("> Enter Option: ");
	cin>>option;

	switch (option) {
		case 1: p1->manuallyPlaceShipsOnGameBoard(playerOneGameBoard, ship);
				break;
		case 2: p1->randomlyPlaceShipsOnGameBoard(playerOneGameBoard, ship);
				break;
	}
	/**
	 * Player2's ships must be randomly placed. Note that the placement of
	 * Player2's ships must be unknown. Thus, Player2's board will only
	 * display '~' in each cell after the placement of each ship.
	 */


	p2->randomlyPlaceShipsOnGameBoard (playerTwoGameBoard, ship);
	printf ("> Player 2 (Computer's) board has been generated.\n");
	if(cpuBoardDisplayStatus != 0 && cpuBoardDisplayStatus != 2){
		p2->printGameBoard(playerTwoGameBoard, cpuBoardDisplayStatus);
	}

	/**
	 * The program should randomly select Player1 or Player2 to go first.
	 */
	player = p1->getRandomNumber(0, 1);
	printf ("> Player %d has been randomly selected to go first.\n", player + 1);


	/**
	 * Once it has been decided on which player goes first, the game starts.
	 * Whenever it's Player1's turn, a prompt should be displayed asking for
	 * a position to target (specifying where to "shoot") on the enemy's
	 * (Player2's) board (2-dimensional array). The position should be
	 * specified in terms of a row and a column on the board. The row and
	 * column should always be displayed along with the board. If the
	 * position specified happens to hit a ship, then a '*' should replace
	 * the '-' on Player2's board. If the positioned specified misses any
	 * one of the ships in the fleet, then a 'm' should replace the '-' on
	 * Player2's board. Note that from turn-to-turn each player should NOT
	 * be allowed to enter the same position. Also, between turns clear the
	 * screen (system("cls")). In one turn, a player can only take one shot
	 * at the enemy's (other player's) fleet. When Player2 takes a shot at
	 * Player1's board, each hit should be specified with a '*' and each
	 * miss with a 'm' on Player1's board. The game is over win Player1 or
	 * Player2 has sunk all of the ships in the fleet of the enemy.
	 */

	/**
	 * For each move made by Player1 and Player2, the results should be
	 * echoed to a file called "battleship.log". In this file, you should
	 * log the targeted position by each player on each move and whether it
	 * was a hit on one of the ships in the fleet. Also, if one of the ships
	 * happens to sink, then note this in the log file.
	 */
	bool done=false;
	while (!done) {

		/* Write to battleship.log */
		fprintf (outStream, "Player %d's turn.\n", player + 1);

		/* switches back and forth between player 1 and player 2 */
		switch (player) {

			case PLAYER_ONE:
				/* Print player 2's game board*/
				printf ("> Player 2's Board:\n");
				if(cpuBoardDisplayStatus != 0 && cpuBoardDisplayStatus != 2) {
					p2->printGameBoard(playerTwoGameBoard, cpuBoardDisplayStatus);
				}
				else {
					printf("[CIA INTELLIGENCE]\n");
					printf("[  REDACTED FOR  ]\n");
					printf("[YOUR SAFETY AND ]\n");
					printf("[ THE SAFETY OF  ]\n");
					printf("[     OTHERS     ]\n");
				}
				printf ("> PLAYER 1'S TURN\n");

				/* Prompts user to enter target */
				do {
					target = p2->getTarget();
					shot = p2->checkShot(playerTwoGameBoard, target);

					/* prompts user that input is invalid or either a hit or miss cell */
					if (shot == -1)
						printf ("> Try inputting another target!\n");

				} while (shot == -1);

				/* saves the character of the targeted cell */
				shipSymbol = playerTwoGameBoard[target.row][target.column].symbol;
				break;

			case PLAYER_TWO:

			/**** IMPLEMENTATION OF ARTIFICIAL INTELLIGENCE ****/

			/* Print player 1's game board */
			printf ("> Player 1's Board:\n");
			p1->printGameBoard(playerOneGameBoard, true);
			printf ("> COMPUTER'S TURN\n");

			/**
			 * check from a previous cycle if a ship has sunk,
			 * if yes, it reinitializes values to go back to hunt mode
			 */
			if (hasAShipSunked) {
				hasAShipSunked = false;
				targetMode = false;
				huntMode = true;
			}

			/**
			 * TARGET MODE
			 * This mode is true when the computer randomly selects a target,
			 * and it happens to be a hit, this mode is set to true.
			 */
			if (targetMode) {
				/* retains value of targetAI */
				target = targetAI;

				do {
					if (cardinals[NORTH]) {        /* NORTH */
						target.row = north;
					} else if (cardinals[SOUTH]) { /* SOUTH */
						target.row = south;
					} else if (cardinals[WEST]) {  /* WEST */
						target.column = west;
					} else if (cardinals[EAST]) {  /* EAST */
						target.column = east;
					} else if (!cardinals[NORTH] && !cardinals[SOUTH] &&
							   !cardinals[WEST]  && !cardinals[EAST]  &&
							   !hasAShipSunked) {
						/* All cardinals are false but has not sunk a ship */

						/* reinitiliazes target to the original target value */
						target = targetOrigin;
						targetTemp = target;

						/**
						 * Counter increments by one, when the loop cycles again and
						 * all cardinals are still false. This ensures that we are checking
						 * one cell over the adjacent cells
						 */
						north = target.row - counter;
						targetTemp.row = north;

						/* checks if the value NORTH of the target is a hit or miss */
						if (p1->checkShot(playerOneGameBoard, targetTemp) != -1 && north >= 0) {
							cardinals[NORTH] = true;
						}

						targetTemp = target;
						south = target.row + counter;
						targetTemp.row = south;

						/* checks if the value SOUTH of the target is a hit or miss */
						if (p1->checkShot(playerOneGameBoard, targetTemp) != -1 && south <= 9) {
							cardinals[SOUTH] = true;
						}

						targetTemp = target;
						west = target.column - counter;
						targetTemp.column = west;

						/* checks if the value WEST of the target is a hit or miss */
						if (p1->checkShot(playerOneGameBoard, targetTemp) != -1 && west >= 0) {
							cardinals[WEST] = true;
						}

						targetTemp = target;
						east = target.column + counter;
						targetTemp.column = east;

						/* checks if the value EAST of the target is a hit or miss */
						if (p1->checkShot(playerOneGameBoard, targetTemp) != -1 && east <= 9) {
							cardinals[EAST] = true;
						}

						/**
						 * increments counter every cycle, serves as a addend to how
						 * many cells to check from the target cell
						 */
						counter++;

					} else  {
						/* when all conditions are not met, it reverts back to Hunt mode */
						targetMode = false;
						huntMode = true;
						break;
					}

					/* checks if the shot is a hit or miss */
					shot = p1->checkShot(playerOneGameBoard, target);

				} while (shot == -1 && targetMode == true);

				/**
				 * this loop flips the values of the cardinals when it is not needed
				 * takes the cardinal off the stack for directions to check
				 */
				if (shot == 1 && huntMode == false) {
					for (i = 0; i < 4; i++) {
						if (flipper == false)
							cardinals[i] = false;

						if (cardinals[i] == flipper)
							flipper = false;
					}
				} else {
					for (i = 0; i < 4; i++) {
						if (flipper == true && cardinals[i] != false) {
							cardinals[i] = false;
							break;
						}
					}
				}

				/* reinitializes the value of flipper */
				flipper = true;
			}

			/**
			 * HUNT MODE
			 * This is starting mode of Player 2's turn. This pertains to the
			 * computer randomly selecting cells as targets and checking if they are
			 * just plain water. When a cell equates to a Hit, then hunt mode is
			 * set to false and target mode is set to true
			 */
			if (huntMode) {

				/* reinitializes important values */
				counter = 1;
				flipper = true;
				for (i = 0; i < 4; i++)
					cardinals[i] = true;

				/* loop that gets a random number of a cell as target */
				do {
					target.row = p1->getRandomNumber(0, 9);
					target.column =  p1->getRandomNumber(0, 9);

					/* checks target if it is a miss or hit cell */
					shot = p1->checkShot(playerOneGameBoard, target);
				} while (shot == -1);

				/* if it is a Hit, this saves the value of the original target */
				if (shot == 1) targetOrigin = target;
			}

			/**
			 * shot values (1 means craft is hit, 0 means missed and -1 means
			 * it is already a hit or miss cell
			 */
			if (shot == 1) {

				/**
				 * if all cardinals are false and no ship was sunk,
				 * reintializes target from value of original target
				 */
				if (!cardinals[NORTH] && !cardinals[SOUTH] &&
					!cardinals[WEST]  && !cardinals[EAST]  &&
					!hasAShipSunked) { target = targetOrigin; }

				/* hunt mode is false during a target mode */
				huntMode = false;
				targetMode = true;
				targetAI = target;

				/**
				 * get values of adjacent cells and ensuring that
				 * that it is withing the bounds of gameboard for
				 * NORTH, SOUTH, WEST & EAST
				 */
				if (cardinals[NORTH] == true) {  /* NORTH */
					north = (targetAI.row - 1);
					p1->checkBoundsOfCardinal (cardinals, north, NORTH);
					targetTemp = target;
					targetTemp.row = north;
					if (p1->checkShot(playerOneGameBoard, targetTemp) == -1)
						cardinals[NORTH] = false;
				}

				if (cardinals[SOUTH] == true) {  /* SOUTH */
					south = targetAI.row + 1;
					p1->checkBoundsOfCardinal (cardinals, south, SOUTH);
					targetTemp = target;
					targetTemp.row = south;
					if (p1->checkShot (playerOneGameBoard, targetTemp) == -1)
						cardinals[SOUTH] = false;
				}

				if (cardinals[WEST] == true) {   /* WEST */
					west  = targetAI.column - 1;
					p1->checkBoundsOfCardinal (cardinals, west, WEST);
					targetTemp = target;
					targetTemp.column = west;
					if (p1->checkShot (playerOneGameBoard, targetTemp) == -1)
						cardinals[WEST] = false;
				}

				if (cardinals[EAST] == true) {   /* EAST */
					east  = targetAI.column + 1;
					p1->checkBoundsOfCardinal(cardinals, east, EAST);
					targetTemp = target;
					targetTemp.column = east;
					if (p1->checkShot(playerOneGameBoard, targetTemp) == -1)
						cardinals[EAST] = false;
				}
			}

			/* saves the character of the targeted cell */
			shipSymbol = playerOneGameBoard[target.row][target.column].symbol;
			break;
	}

	/**
	 * Prompts player if it's a hit or miss
	 */
	if (shot == 1) { /* HIT */
		printf ("> %d, %d is a hit!\n", target.row, target.column);

		/* Write to battleship.log */
		fprintf (outStream, "%d, %d is a hit!\n", target.row, target.column);

		/* Keeps track so number of hits for stats */
		players[player].numHits++;

		/* Checks if the ship has sank */
		if (player == 1)
			hasAShipSunked = p1->checkSunkShip(sunkShip, !player, shipSymbol, outStream);
		else
			p2->checkSunkShip(sunkShip, !player, shipSymbol, outStream);

	} else {        /* MISS */
		printf ("> %d, %d is a miss!\n", target.row, target.column);

		/* Write to battleship.log */
		fprintf (outStream, "%d, %d is a miss!\n", target.row, target.column);
		players[player].numMisses++;
	}

	if (player == 0) /* If Player 1 then update player 2's gameboard */
	{
		p2->updateGameBoard(playerTwoGameBoard, target);
	}
	else             /* If Player 2 then update player 1's gameboard */
	{
		p1->updateGameBoard(playerOneGameBoard, target);
	}
	/* Determines if there is a winner based on number of hits */
	if(isWinner(players, sunkShip, player)){
		printf ("\n> Player %d wins!\n", player + 1);
		//answer=true;
		done=true;
		/* Write to battleship.log */
		fprintf (outStream, "\n>>>>> Player %d wins! <<<<<\n", player + 1);
		break;
	}
	/* switches from player 1 to player 2 */
	player = !player;


	}

	/**
	 * At the end of the game, Player1's and Player2's statistics should be
	 * written to "battleship.log". The stats include total number of hits,
	 * total number of misses, total number of shots, hits to misses ratio
	 * (as a percentage), and won or lost the game. Note that the statistics
	 * should be placed into a structure called Stats. You need two variables
	 * of type Stats, one for Player1 and one for Player2. Once the game has
	 * ended you should write the contents of each struct variable to the
	 * "battleship.log" file.
	 */
	players[0].totalShots = players[0].numHits + players[0].numMisses;
	if(players[0].numHits!=0){
		players[0].hitMissRatio = ((double) players[0].numHits/(double) players[0].numMisses) * 100;
	}else{
		players[0].hitMissRatio = 1;
	}
	if(players[1].numHits!=0){
		players[1].hitMissRatio = ((double) players[1].numHits/(double) players[1].numMisses) * 100;
	}else{
		players[1].hitMissRatio = 1;
	}
	players[1].totalShots = players[1].numHits + players[1].numMisses;
	players[1].hitMissRatio = ((double) players[1].numHits/(double) players[1].numMisses) * 100;
	fprintf (outStream, "+===================================================\n");
	fprintf (outStream, "|                    PLAYER STATS                   \n");
	fprintf (outStream, "+---------------------------------------------------\n");
	fprintf (outStream, "| PLAYER 1 : %d hits                                \n", players[0].numHits);
	fprintf (outStream, "|            %d misses                              \n", players[0].numMisses);
	fprintf (outStream, "|            %d total shots                         \n", players[0].totalShots);
	fprintf (outStream, "|            %.2lf%% hit/miss ratio                 \n", players[0].hitMissRatio);
	fprintf (outStream, "| PLAYER 2 : %d hits                                \n", players[1].numHits);
	fprintf (outStream, "|            %d misses                              \n", players[1].numMisses);
	fprintf (outStream, "|            %d total shots                         \n", players[1].totalShots);
	fprintf (outStream, "|            %.2lf%% hit/miss ratio                 \n", players[1].hitMissRatio);
	fprintf (outStream, "+===================================================");

	fclose (outStream);
	}
	answer=true;
	return answer;
}
void Production::welcomeScreen() {
	printf ("XXXXX   XXXX  XXXXXX XXXXXX XX     XXXXXX  XXXXX XX  XX XX XXXX\n");
	printf ("XX  XX XX  XX   XX     XX   XX     XX     XX     XX  XX XX XX  XX\n");
	printf ("XXXXX  XX  XX   XX     XX   XX     XXXX    XXXX  XXXXXX XX XXXX\n");
	printf ("XX  XX XXXXXX   XX     XX   XX     XX         XX XX  XX XX XX\n");
	printf ("XXXXX  XX  XX   XX     XX   XXXXXX XXXXXX XXXXX  XX  XX XX XX\n");
	printf ("\n\n");
	printf ("RULES OF THE GAME:\n");
	printf ("1. This is a two player game.\n");
	printf ("2. Player 1 is you and Player 2 is the computer.\n");
	printf ("3. Player 1 will be prompted if user wants to manually input coordinates\n");
	printf ("   for the game board or have the computer randomly generate a game board\n");
	printf ("4. There are five types of ships to be placed by longest length to the\n");
	printf ("   shortest; [c] Carrier has 5 cells, [b] Battleship has 4 cells, [r] Cruiser\n");
	printf ("   has 3 cells, [s] Submarine has 3 cells, [d] Destroyer has 2 cells\n");
	printf ("5. The computer randomly selects which player goes first\n");
	printf ("6. The game begins as each player tries to guess the location of the ships\n");
	printf ("   of the opposing player's game board; [*] hit and [m] miss\n");
	printf ("7. First player to guess the location of all ships wins\n\n");
}

bool Production::isWinner(Stats players[], short sunkShips[][5],int player) {
    bool isWin = true;
    int opponent = (player+1)%2;
    for(int i = 0; i<NUM_OF_SHIPS; i++){
        if(sunkShips[opponent][i]!=0){
            isWin=false;
        }
    }
    return isWin;
}
