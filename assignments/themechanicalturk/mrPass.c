/*
 *  Mr Pass.  Brain the size of a planet!
 *
 *  Proundly Created by Richard Buckland
 *  Share Freely Creative Commons SA-BY-NC 3.0. 
 *
 */

/*
 * Evan Lee
 * Andrew Timkov
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
 
#include "Game.h"
#include "mechanicalTurk.h"
 
action decideAction (Game g) {   
	action nextAction;

	int ourPlayer = getWhoseTurn(g);

	if (getStudents(g, ourPlayer, STUDENT_MJ) > 0 &&
		getStudents(g, ourPlayer, STUDENT_MTV) > 0 &&
		getStudents(g, ourPlayer, STUDENT_MMONEY) > 0) {
		nextAction.actionCode = START_SPINOFF;
	} else {
		nextAction.actionCode = PASS;
	}
   
	return nextAction;
}