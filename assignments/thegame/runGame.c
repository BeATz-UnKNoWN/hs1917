/*
 * yo follow me on soundcloud
 * https://soundcloud.com/heyheyeddiesfathaha
 * Jason Hidajat
 * Evan Lee
 * Eddie Qi
 * Andrew Timkov
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Game.h"

#define KPI_WIN_VALUE 150
#define LEFT 'L'
#define RIGHT 'R'
#define BACK 'B'
#define DEFAULT_DISCIPLINES {STUDENT_BQN, STUDENT_MMONEY, STUDENT_MJ, \
                STUDENT_MMONEY, STUDENT_MJ, STUDENT_BPS, STUDENT_MTV, \
                STUDENT_MTV, STUDENT_BPS,STUDENT_MTV, STUDENT_BQN, \
                STUDENT_MJ, STUDENT_BQN, STUDENT_THD, STUDENT_MJ, \
                STUDENT_MMONEY, STUDENT_MTV, STUDENT_BQN, STUDENT_BPS }
#define DEFAULT_DICE {9,10,8,12,6,5,3,11,3,11,4,6,4,7,9,2,8,10,5}
#define TERRA_NULLIUS -1

int rollDice (void);
void doTurn (Game g);
int hasPlayerWon(Game g);
void printStats (Game g, int whoseTurn);
void printResults(Game g, int winner);

int main(int argc, char *argv[]) {
	//create a random dice roll seed
	srand(time(NULL));

	//load map data
	int disciplines[] = DEFAULT_DISCIPLINES;
	int dice[] = DEFAULT_DICE;

	Game g = newGame(disciplines, dice);

	int gameOver = FALSE;
	int diceValue;
	int whoseTurn = NO_ONE;
	int turnNum = TERRA_NULLIUS;

	//while no on has won, run the game
	while (gameOver == FALSE) {
		diceValue = rollDice();
		diceValue += rollDice();
		throwDice(g, diceValue);

		whoseTurn = getWhoseTurn(g);
		turnNum = getTurnNumber(g);

		printf("Turn %d - Player %d\n",turnNum, whoseTurn);
      	printf("The dice value %d\n", diceValue);

		doTurn(g);
		printStats(g, whoseTurn);
		gameOver = hasPlayerWon(g);
	}

	printResults(g, gameOver);

	disposeGame(g);

	return EXIT_SUCCESS;
}

void doTurn (Game g) {
	action move;
	char direction;
	int i = 0;

	printf("Enter move action code\n");
	scanf("%d", &move.actionCode);

	//while player still wants to do something
	while (move.actionCode != PASS) {
		if (move.actionCode == START_SPINOFF) {
			if (rand() % 3 <= 1) {
				move.actionCode = OBTAIN_PUBLICATION;
			} else {
				move.actionCode = OBTAIN_IP_PATENT;
			}
		}
		else if (move.actionCode == BUILD_CAMPUS ||
		    	 move.actionCode == BUILD_GO8 ||
		   		 move.actionCode == OBTAIN_ARC) {
			printf("Type a path using L, R, and B.\n");
			direction = getchar();
			while (i < PATH_LIMIT) {
			move.destination[i] = 0;
			i++;
			}
			i = 0;

			//move.destination = {0};

			while ((i < PATH_LIMIT) && (direction == LEFT ||
				direction == RIGHT || direction == BACK)) {
				move.destination[i] = direction;
				direction = getchar();
				i++;
			}
		}
		else if (move.actionCode == RETRAIN_STUDENTS) {
			printf("Convert students from: ");
			scanf("%d", &move.disciplineFrom);
			printf("To: ");
			scanf("%d", &move.disciplineTo);
		}

		if (isLegalAction(g, move) == TRUE) {
			makeAction(g, move);
		} else {
			printf("That move is invalid.\n");
			printf("Enter move action code\n");
			scanf("%d", &move.actionCode);
		}
		
	}
	return;
}

int hasPlayerWon(Game g) {
	int winner = NO_ONE;

	if (getKPIpoints(g, UNI_A) >= KPI_WIN_VALUE) {
		winner = UNI_A;
	} else if (getKPIpoints(g, UNI_B) >= KPI_WIN_VALUE) {
		winner = UNI_B;
	} else if (getKPIpoints(g, UNI_C) >= KPI_WIN_VALUE) {
		winner = UNI_C;
	}

	return winner;
}

int rollDice (void) {
	return rand() % 6 + 1;
}

void printStats (Game g, int whoseTurn) {
	printf("CURRENT STATS\n");
 	printf("Publications: %d\n", getPublications(g, whoseTurn));
 	printf("Patents: %d\n", getIPs(g, whoseTurn));
 	printf("Campuses: %d\n", getCampuses(g, whoseTurn));
 	printf("GO8's: %d\n", getGO8s(g, whoseTurn));
 	printf("ARC's: %d\n", getARCs(g, whoseTurn));

	// The Students
 	printf("\n");
 	printf("THD's: %d\n", getStudents(g, whoseTurn, STUDENT_THD));
 	printf("BPS's: %d\n", getStudents(g, whoseTurn, STUDENT_BPS));
 	printf("BQN's: %d\n", getStudents(g, whoseTurn, STUDENT_BQN));
 	printf("MJ's: %d\n", getStudents(g, whoseTurn, STUDENT_MJ));
 	printf("MTV's: %d\n", getStudents(g, whoseTurn, STUDENT_MTV));
 	printf("MMONEY: %d\n", getStudents(g, whoseTurn, STUDENT_MMONEY));

	// Other Statistics
	printf("\n");
	printf("KPI: %d\n", getKPIpoints(g,whoseTurn));
	printf("\n");

	return;
}

void printResults(Game g, int winner) {
	printf("Player %d has won!!!!\n", winner);
	return;
}
