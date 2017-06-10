/*
 * Evan Lee
 * Andrew Timkov
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
 
#include "Game.h"
#include "mechanicalTurk.h"

#define NUM_STUDENT_TYPES 6
#define PATH_A "RLRLRL"
#define PATH_B "RRLRLLLRLRL"
#define PATH_C "RRLRLLRLRLLLRLRL"
#define MAX_MANAGED_ARCS 6
#define FALSE 0
#define TRUE 1


/* TODO:
 * Try build arcs/campuses past the hardcoded routes
 * May need to loop until a path is legal (give up after 50 attempts)
 * Maybe try R by default always, so that campuses can be built with arcs
 * Build from both initial campuses
 * Avoid 7's
 */

int leastOfStudent (int students[], int numOfTypes);
int mostOfStudent (int students[], int numOfTypes);
action buildCampus (Game g, int player);
action buildARC (Game g, int player);
action buildGO8 (Game g, int player);
void copyChars (path to, int numChars, int player);
 
action decideAction (Game g) {   
	action nextAction;

	int id = getWhoseTurn(g);
	int arcs = getARCs(g, id);
	int campuses = getCampuses(g, id) - 2;
	int go8s = getGO8s(g, id);
	// the build condition lets us build a campus
	// build condition is to build a campus for every 2 ARCs
	int buildCondition = arcs - (campuses * 2);

	// we need to try build GO8s first, else the other else if branches
	// will be executed and set the actionCodes to pass once enough
	// ARCs and campuses have been built
	if (isLegalAction(g, buildGO8(g, id)) == TRUE && campuses > go8s) {
		// if there are sufficient resources try to build a GO8
		nextAction = buildGO8(g, id);
	} else if (isLegalAction(g, buildCampus(g, id)) == TRUE &&
			   buildCondition == 2 && arcs <= MAX_MANAGED_ARCS) {
		// if there are sufficient resources try to build a campus
		nextAction = buildCampus(g, id);
	} else if (isLegalAction(g, buildARC(g, id)) == TRUE &&
			   arcs < MAX_MANAGED_ARCS && buildCondition != 2) {
		// we only account for 6 arcs in our hardcoded path
		// so if getARCs is 5, the condition will be true and after
		// this code executes getARCs will be at 6
		// if there are sufficient resources try to build an ARC
		nextAction = buildARC(g, id);
	} else if (getStudents(g, id, STUDENT_MJ) >= 1 &&
			   getStudents(g, id, STUDENT_MTV) >= 1 &&
			   getStudents(g, id, STUDENT_MMONEY) >= 1) {
		// if there are sufficient resources start a spinoff
		nextAction.actionCode = START_SPINOFF;
	} else {
		int students[5] = {getStudents(g, id, STUDENT_BPS),
						   getStudents(g, id, STUDENT_BQN),
						   getStudents(g, id, STUDENT_MJ),
						   getStudents(g, id, STUDENT_MTV),
						   getStudents(g, id, STUDENT_MMONEY)};
		// calculate the type of student with lowest/greatest quantity
		int most = mostOfStudent(students, 5) + 1;
		int least = leastOfStudent(students, 5) + 1;
		int rate = getExchangeRate(g, id, most, least);

		// use the rate to determine if you can retrain
		if (getStudents(g, id, most) >= rate) {
			// retrain from abundant supply of one student to a rarer type
			nextAction.disciplineFrom = most;
			nextAction.disciplineTo = least;
			nextAction.actionCode = RETRAIN_STUDENTS;
		} else {
			nextAction.actionCode = PASS;
		}
	}

	printf("%d\n", nextAction.actionCode);


   
	return nextAction;
}

// building a campus
action buildCampus (Game g, int player) {
	action nextAction;
	// determine when the path from a campus actually starts
	int pathStart = (player - 1) * 5;
	// length of the destination path
	int length = (getCampuses(g, player) - 1) * 2 + pathStart;
	copyChars(nextAction.destination, length, player);
	printf("player%d buildCampus path: %s\n", player, nextAction.destination);
	nextAction.actionCode = BUILD_CAMPUS;

	return nextAction;
}

// building an arc
// basically the same as buildCampus apart from the length
action buildARC (Game g, int player) {
	action nextAction;
	int pathStart = (player - 1) * 5;
	// with the campus we wanted to build on an existing arc
	// here we want to extend the road, so we use the +1
	int length = getARCs(g, player) + 1 + pathStart;
	copyChars(nextAction.destination, length, player);
	nextAction.actionCode = OBTAIN_ARC;
	return nextAction;
}

action buildGO8 (Game g, int player) {
	action nextAction;
	nextAction.actionCode = PASS;
	int canBuild = FALSE;
	// determine when the path from a campus actually starts
	int pathStart = (player - 1) * 5;
	// length of the furthest possible controlled path
	int length = MAX_MANAGED_ARCS + pathStart;
	// copy that whole path into the destination
	copyChars(nextAction.destination, length, player);
	while (length > pathStart && canBuild == FALSE) {
		printf("player%d buildGO8 path: %s\n", player, nextAction.destination);
		if (getCampus(g, nextAction.destination) == player) {
			canBuild = TRUE;
			nextAction.actionCode = BUILD_GO8;
		} else {
			length--;
			nextAction.destination[length] = '\0';
			length--;
			nextAction.destination[length] = '\0';
		}
	}
	return nextAction;
}

void copyChars (path to, int numChars, int player) {
	if (player == UNI_A) {
		strncpy(to, PATH_A, numChars);
		to[numChars] = '\0';
	} else if (player == UNI_B) {
		strncpy(to, PATH_B, numChars);
		to[numChars] = '\0';
	} else if (player == UNI_C) {
		strncpy(to, PATH_C, numChars);
		to[numChars] = '\0';
	}
}

// return the type of student with lowest quantity
int leastOfStudent (int students[], int numOfTypes) {
	int student = 0;
	int i = 0;
	int temp = 0;

	while (i < numOfTypes) {
		if (i == 0) {
			temp = students[i];
		} else if (students[i] < temp) {
			student = i;
			temp = students[i];
		}
		i++;
	}

	return student;
}

// return the type of student with greatest quantity
int mostOfStudent (int students[], int numOfTypes) {
	int student = 0;
	int i = 0;
	int temp = 0;

	while (i < numOfTypes) {
		if (i == 0) {
			temp = students[i];
		} else if (students[i] > temp) {
			student = i;
			temp = students[i];
		}
		i++;
	}

	return student;
}