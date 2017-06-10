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
#include <assert.h>

#include "Game.h"
#include "mechanicalTurk.h"

#define DEFAULT_DISCIPLINES {STUDENT_BQN, STUDENT_MMONEY, STUDENT_MJ, \
                STUDENT_MMONEY, STUDENT_MJ, STUDENT_BPS, STUDENT_MTV, \
                STUDENT_MTV, STUDENT_BPS,STUDENT_MTV, STUDENT_BQN, \
                STUDENT_MJ, STUDENT_BQN, STUDENT_THD, STUDENT_MJ, \
                STUDENT_MMONEY, STUDENT_MTV, STUDENT_BQN, STUDENT_BPS}
#define DEFAULT_DICE {9,10,8,12,6,5,3,11,3,11,4,6,4,7,9,2,8,10,5}

void initialValueTests (Game g);

int main (int argc, char *argv[]) {

	//initiate data for game creation
    int disciplines[] = DEFAULT_DISCIPLINES;
    int dice[] = DEFAULT_DICE;

    //create a game
    Game g = newGame(disciplines, dice);

    //test the values of the game during TERRA NULLIUS
    initialValueTests (g);

    return EXIT_SUCCESS;
}

//test the inital game values
void initialValueTests (Game g) {
	assert(getDiscipline(g,  0) == STUDENT_BQN);
	assert(getDiscipline(g,  1) == STUDENT_MMONEY);
	assert(getDiscipline(g,  2) == STUDENT_MJ);
	assert(getDiscipline(g,  3) == STUDENT_MMONEY);
	assert(getDiscipline(g,  4) == STUDENT_MJ);
	assert(getDiscipline(g,  5) == STUDENT_BPS);
	assert(getDiscipline(g,  6) == STUDENT_MTV);
	assert(getDiscipline(g,  7) == STUDENT_MTV);
	assert(getDiscipline(g,  8) == STUDENT_BPS);
	assert(getDiscipline(g,  9) == STUDENT_MTV);
	assert(getDiscipline(g, 10) == STUDENT_BQN);
	assert(getDiscipline(g, 11) == STUDENT_MJ);
	assert(getDiscipline(g, 12) == STUDENT_BQN);
	assert(getDiscipline(g, 13) == STUDENT_THD);
	assert(getDiscipline(g, 14) == STUDENT_MJ);
	assert(getDiscipline(g, 15) == STUDENT_MMONEY);
	assert(getDiscipline(g, 16) == STUDENT_MTV);
	assert(getDiscipline(g, 17) == STUDENT_BQN);
	assert(getDiscipline(g, 18) == STUDENT_BPS);

	assert(getDiceValue(g,  0) == 9);
	assert(getDiceValue(g,  1) == 10);
	assert(getDiceValue(g,  2) == 8);
	assert(getDiceValue(g,  3) == 12);
	assert(getDiceValue(g,  4) == 6);
	assert(getDiceValue(g,  5) == 5);
	assert(getDiceValue(g,  6) == 3);
	assert(getDiceValue(g,  7) == 11);
	assert(getDiceValue(g,  8) == 3);
	assert(getDiceValue(g,  9) == 11);
	assert(getDiceValue(g, 10) == 4);
	assert(getDiceValue(g, 11) == 6);
	assert(getDiceValue(g, 12) == 4);
	assert(getDiceValue(g, 13) == 7);
	assert(getDiceValue(g, 14) == 9);
	assert(getDiceValue(g, 15) == 2);
	assert(getDiceValue(g, 16) == 8);
	assert(getDiceValue(g, 17) == 10);
	assert(getDiceValue(g, 18) == 5);

	assert(getMostARCs(g) == NO_ONE);
	assert(getMostPublications(g) == NO_ONE);
	assert(getTurnNumber(g) == -1);
	assert(getWhoseTurn(g) == NO_ONE);

	assert(getKPIpoints(g, UNI_A) == 20);
	assert(getKPIpoints(g, UNI_B) == 20);
	assert(getKPIpoints(g, UNI_C) == 20);

	assert(getARCs(g, UNI_A) == 0);
	assert(getARCs(g, UNI_B) == 0);
	assert(getARCs(g, UNI_C) == 0);

	assert(getGO8s(g, UNI_A) == 0);
	assert(getGO8s(g, UNI_B) == 0);
	assert(getGO8s(g, UNI_C) == 0);

	assert(getCampuses(g, UNI_A) == 2);
	assert(getCampuses(g, UNI_B) == 2);
	assert(getCampuses(g, UNI_C) == 2);

	assert(getIPs(g, UNI_A) == 0);
	assert(getIPs(g, UNI_B) == 0);
	assert(getIPs(g, UNI_C) == 0);

	assert(getStudents(g, UNI_A, STUDENT_THD) == 0);
	assert(getStudents(g, UNI_A, STUDENT_BPS) == 3);
	assert(getStudents(g, UNI_A, STUDENT_BQN) == 3);
	assert(getStudents(g, UNI_A, STUDENT_MJ) == 1);
	assert(getStudents(g, UNI_A, STUDENT_MTV) == 1);
	assert(getStudents(g, UNI_A, STUDENT_MMONEY) == 1);

	assert(getStudents(g, UNI_B, STUDENT_THD) == 0);
	assert(getStudents(g, UNI_B, STUDENT_BPS) == 3);
	assert(getStudents(g, UNI_B, STUDENT_BQN) == 3);
	assert(getStudents(g, UNI_B, STUDENT_MJ) == 1);
	assert(getStudents(g, UNI_B, STUDENT_MTV) == 1);
	assert(getStudents(g, UNI_B, STUDENT_MMONEY) == 1);

	assert(getStudents(g, UNI_C, STUDENT_THD) == 0);
	assert(getStudents(g, UNI_C, STUDENT_BPS) == 3);
	assert(getStudents(g, UNI_C, STUDENT_BQN) == 3);
	assert(getStudents(g, UNI_C, STUDENT_MJ) == 1);
	assert(getStudents(g, UNI_C, STUDENT_MTV) == 1);
	assert(getStudents(g, UNI_C, STUDENT_MMONEY) == 1);

	printf("Initial value tests passed!\n");

	return;
}
