/*
 * yo follow me on soundcloud
 * https://soundcloud.com/heyheyeddiesfathaha
 * Jason Hidajat
 * Evan Lee
 * Eddie Qi
 * Andrew Timkov
 * Game ADT interface function implementations
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "Game.h"

#define NUM_NODES 54
#define NUM_EDGES 72
#define NUM_STUDENT_TYPES 6
#define NUM_X 12
#define NUM_Y 11
#define NUM_AXES 2
#define X 0
#define Y 1
#define NODES_ON_HEX 6
#define NOTHING 0

#define POINTS_FOR_ARC 2
#define POINTS_FOR_CAMPUS 10
#define POINTS_FOR_GO8 20
#define POINTS_FOR_PATENT 10
#define POINTS_FOR_MOST_ARCS 10
#define POINTS_FOR_MOST_PUBLICATIONS 10

#define REGION_CENTRES {{1,3}, {1,5}, {1,7}, \
						{3,2}, {3,4}, {3,6}, {3,8}, \
						{5,1}, {5,3}, {5,5}, {5,7}, {5,9}, \
						{7,2}, {7,4}, {7,6}, {7,8}, \
						{9,3}, {9,5}, {9,7}}

/* ****************************
 * DEFINING THE GAME ADT
 * ****************************/

typedef struct _coord {
    int x;
    int y;
} Coord;

typedef struct _node {
    // since nodes are located at every coord, valid is required
    // to keep track of whether it's a valid node
    int valid;
    //type is used to store the type (GO8/campus) and the owner
    //through values of 1-6
    int type;
} Node;

typedef struct _edge {
    // an edge is defined by the nodes it connects
	// they only travel in one direction, and are indexed by their bottom-left-most
	// point
    Coord end;
	int valid;
    int type;
} Edge;

typedef struct _player {
	//UNI_A/B/C value
	int id;

	//kpi points of the player
	int kpi;

    // somewhat self explanatory counters
    int numARC;
    int numGO8;
    int numCampus;
    int numPatent;
    int numPublication;

    // counter for each type of student
    // index is defined by constant for each student type
    int numStudents[NUM_STUDENT_TYPES];
} Player;

typedef struct _edgeRef {
	Coord coord;
	int index;
} edgeRef;

struct _game {
	//stats to store
    int turnNumber;
    int whoseTurn;
    int uniWithMostARCs;
    int uniWithMostPublications;

    int discipline[NUM_REGIONS];
    int dice[NUM_REGIONS];

    // players
    Player p1;
    Player p2;
    Player p3;

    // the actual board
    Node nodes[NUM_X][NUM_Y]; // 2d array: 12x11
    Edge edges[NUM_X][NUM_Y][3];
};

/* ****************************
 * GAME FUNCTIONS
 * ****************************/

static void allocateResources(Game g, int diceScore);
static int playerWithHighest (int aVal, int bVal, int cVal);
static int countKPI (Game g, Player player, int playerNum);
static int isNode (Coord toTest);
static edgeRef getARCFromCoord (Game g, Coord start, Coord end);
static int canPlaceCampus (Game g, int player, Coord c);
static int canPlaceARC (Game g, int player, edgeRef index);
static Coord getCampusCoord (Game g, path p);
static edgeRef getARCFromPath(Game g, path p);
static Coord transform(int direction, Coord initial);
static int compareEdge(Coord a, Coord b);

Game newGame (int discipline[], int dice[]) {
	//allocate memory for the game
	Game g = malloc(sizeof(struct _game));

	//set some base data
	g->turnNumber = -1;
	g->whoseTurn = NO_ONE;
	g->uniWithMostARCs = NO_ONE;
	g->uniWithMostPublications = NO_ONE;

	//fill dice and discipline array with data from parameters
	int i = 0;
	while (i < NUM_REGIONS) {
		g->discipline[i] = discipline[i];
		g->dice[i] = dice[i];
		i++;
	}

	int startStudents[NUM_STUDENT_TYPES] = {0, 3, 3, 1, 1, 1};

	//set base player1 data
	g->p1.id = UNI_A;
	g->p1.numARC = 0;
	g->p1.numGO8 = 0;
	g->p1.numCampus = 2;
	g->p1.numPatent = 0;
	g->p1.numPublication = 0;
	i = 0;
	while (i < NUM_STUDENT_TYPES) {
		g->p1.numStudents[i] = startStudents[i];
		i++;
	}

	//set base player2 data
	g->p2.id = UNI_B;
	g->p2.numARC = 0;
	g->p2.numGO8 = 0;
	g->p2.numCampus = 2;
	g->p2.numPatent = 0;
	g->p2.numPublication = 0;
	i = 0;
	while (i < NUM_STUDENT_TYPES) {
		g->p2.numStudents[i] = startStudents[i];
		i++;
	}

	//set base player3 data
	g->p3.id = UNI_C;
	g->p3.numARC = 0;
	g->p3.numGO8 = 0;
	g->p3.numCampus = 2;
	g->p3.numPatent = 0;
	g->p3.numPublication = 0;
	i = 0;
	while (i < NUM_STUDENT_TYPES) {
		g->p3.numStudents[i] = startStudents[i];
		i++;
	}

	//set all board positions to 0
	Coord c = {0}; // counter

	while (c.y < NUM_Y) {
	    c.x = 0;
	    while (c.x < NUM_X) {
	    	// set type to 0, meaning it is not occupied
	    	g->nodes[c.x][c.y].type = VACANT_VERTEX;
	    	// check whether node is valid and store in struct
	    	g->nodes[c.x][c.y].valid = isNode(c);
	    	c.x++;
	    }
	    c.y++;
	}

	// set up the starting campuses of each player
	g->nodes[5][0].type = UNI_A;
	g->nodes[6][10].type = UNI_A;
	g->nodes[0][3].type = UNI_B;
	g->nodes[11][7].type = UNI_B;
	g->nodes[1][8].type = UNI_C;
	g->nodes[10][2].type = UNI_C;

	// logic for edges
	Coord current = {5, 0};
	Coord check = {0};
	int dir = 0;
	int existsFlag = 0;
	i = 0;
	while (current.x < NUM_X && current.y < NUM_Y) {
		// iterate through nodes until a valid one is found
		dir = 0;
		while (dir < 6) {
			check = transform(dir, current);
			if (g->nodes[check.x][check.y].valid == TRUE) {
				// found a valid adjacent node, check if edge exists
				i = 0;
				existsFlag = FALSE;
				while (i <= 3) {
					if (compareEdge(g->edges[check.x][check.y][i].end, current) == TRUE) {
						existsFlag = TRUE;
					}
					i++;
				}
				// if edge does not exist, create it
				if (existsFlag == FALSE) {
					// find available edge spot
					i = 0;
					while (g->edges[current.x][current.y][i].valid == TRUE) {
						i++;
					}
					// add edge
					g->edges[current.x][current.y][i].valid = TRUE;
					g->edges[current.x][current.y][i].end = check;
					g->edges[current.x][current.y][i].type = VACANT_ARC;
				}
			}
			dir++;
		}

		// go to next valid node
		do {
			if (current.x < NUM_X - 1) {
				current.x++;
			} else {
				current.y++;
				current.x = 0;
			}
		} while (g->nodes[current.x][current.y].valid == FALSE);
	}

	return g;
}

//clear the game from memory
void disposeGame (Game g) {
	free(g);
	return;
}

//performs the given action
void makeAction (Game g, action a) {
	if (a.actionCode == PASS) {
		// do nothing when passing
	} else if (a.actionCode == BUILD_CAMPUS) {
		// get the coordinate of the specified campus
		Coord c = getCampusCoord(g, a.destination);
		// no need to check if its a valid campus; that's handled by
		// isLegalAction, so we can directly set ownership of the campus
		g->nodes[c.x][c.y].type = g->whoseTurn;
		// remove the resources needed to buy a campus
		// also increment the campus counter
		if (g->whoseTurn == UNI_A) {
			g->p1.numStudents[STUDENT_BPS]--;
			g->p1.numStudents[STUDENT_BQN]--;
			g->p1.numStudents[STUDENT_MJ]--;
			g->p1.numStudents[STUDENT_MTV]--;
			g->p1.numCampus++;
		} else if (g->whoseTurn == UNI_B) {
			g->p2.numStudents[STUDENT_BPS]--;
			g->p2.numStudents[STUDENT_BQN]--;
			g->p2.numStudents[STUDENT_MJ]--;
			g->p2.numStudents[STUDENT_MTV]--;
			g->p2.numCampus++;
		} else if (g->whoseTurn == UNI_C) {
			g->p3.numStudents[STUDENT_BPS]--;
			g->p3.numStudents[STUDENT_BQN]--;
			g->p3.numStudents[STUDENT_MJ]--;
			g->p3.numStudents[STUDENT_MTV]--;
			g->p3.numCampus++;
		}
	} else if (a.actionCode == BUILD_GO8) {
		Coord c = getCampusCoord(g, a.destination);
		if (g->whoseTurn == UNI_A) {
			// change the node's value to a GO8 value
			g->nodes[c.x][c.y].type = GO8_A;
			g->p1.numStudents[STUDENT_MJ] -= 2;
			g->p1.numStudents[STUDENT_MMONEY] -= 3;
			g->p1.numCampus--;
			g->p1.numGO8++;
		} else if (g->whoseTurn == UNI_B) {
			g->nodes[c.x][c.y].type = GO8_B;
			g->p2.numStudents[STUDENT_MJ] -= 2;
			g->p2.numStudents[STUDENT_MMONEY] -= 3;
			g->p2.numCampus--;
			g->p2.numGO8++;
		} else if (g->whoseTurn == UNI_C) {
			g->nodes[c.x][c.y].type = GO8_C;
			g->p3.numStudents[STUDENT_MJ] -= 2;
			g->p3.numStudents[STUDENT_MMONEY] -= 3;
			g->p3.numCampus--;
			g->p3.numGO8++;
		}
	} else if (a.actionCode == OBTAIN_ARC) {
		// get the index for the g->edges[] array
		edgeRef index = getARCFromPath(g, a.destination);
		g->edges[index.coord.x][index.coord.y][index.index].type = g->whoseTurn;
		if (g->whoseTurn == UNI_A) {
			g->p1.numStudents[STUDENT_BPS]--;
			g->p1.numStudents[STUDENT_BQN]--;
			g->p1.numARC++;
		} else if (g->whoseTurn == UNI_B) {
			g->p2.numStudents[STUDENT_BPS]--;
			g->p2.numStudents[STUDENT_BQN]--;
			g->p2.numARC++;
		} else if (g->whoseTurn == UNI_C) {
			g->p3.numStudents[STUDENT_BPS]--;
			g->p3.numStudents[STUDENT_BQN]--;
			g->p3.numARC++;
		}
		int playerWithMostARCS = getMostARCs(g);
		// if there is a clear winner and no tie, update the struct
		if (playerWithMostARCS != NO_ONE) {
			g->uniWithMostARCs = playerWithMostARCS;
		}
	} else if (a.actionCode == START_SPINOFF) {
		if (g->whoseTurn == UNI_A) {
			g->p1.numStudents[STUDENT_MJ]--;
			g->p1.numStudents[STUDENT_MTV]--;
			g->p1.numStudents[STUDENT_MMONEY]--;
			// 1/3 chance to get a patent
			if (rand() % 3 <= 1) {
				g->p1.numPatent++;
			} else { // 2/3 chance to get a publication
				g->p1.numPublication++;
			}
		} else if (g->whoseTurn == UNI_B) {
			g->p2.numStudents[STUDENT_MJ]--;
			g->p2.numStudents[STUDENT_MTV]--;
			g->p2.numStudents[STUDENT_MMONEY]--;
			if (rand() % 3 <= 1) {
				g->p2.numPatent++;
			} else {
				g->p2.numPublication++;
			}
		} else if (g->whoseTurn == UNI_C) {
			g->p3.numStudents[STUDENT_MJ]--;
			g->p3.numStudents[STUDENT_MTV]--;
			g->p3.numStudents[STUDENT_MMONEY]--;
			if (rand() % 3 <= 1) {
				g->p3.numPatent++;
			} else {
				g->p3.numPublication++;
			}
		}
		int pWithMostPublications = getMostARCs(g);
		// if there is a clear winner and no tie, update the struct
		if (pWithMostPublications != NO_ONE) {
			g->uniWithMostARCs = pWithMostPublications;
		}
	} else if (a.actionCode == OBTAIN_PUBLICATION) {
		if (g->whoseTurn == UNI_A) {
			g->p1.numStudents[STUDENT_MJ]--;
			g->p1.numStudents[STUDENT_MTV]--;
			g->p1.numStudents[STUDENT_MMONEY]--;
			g->p1.numPublication++;
		} else if (g->whoseTurn == UNI_B) {
			g->p2.numStudents[STUDENT_MJ]--;
			g->p2.numStudents[STUDENT_MTV]--;
			g->p2.numStudents[STUDENT_MMONEY]--;
			g->p2.numPublication++;
		} else if (g->whoseTurn == UNI_C) {
			g->p3.numStudents[STUDENT_MJ]--;
			g->p3.numStudents[STUDENT_MTV]--;
			g->p3.numStudents[STUDENT_MMONEY]--;
			g->p3.numPublication++;
		}
		int pWithMostPublications = getMostARCs(g);
		// if there is a clear winner and no tie, update the struct
		if (pWithMostPublications != NO_ONE) {
			g->uniWithMostARCs = pWithMostPublications;
		}
	} else if (a.actionCode == OBTAIN_IP_PATENT) {
		if (g->whoseTurn == UNI_A) {
			g->p1.numStudents[STUDENT_MJ]--;
			g->p1.numStudents[STUDENT_MTV]--;
			g->p1.numStudents[STUDENT_MMONEY]--;
			g->p1.numPatent++;
		} else if (g->whoseTurn == UNI_B) {
			g->p2.numStudents[STUDENT_MJ]--;
			g->p2.numStudents[STUDENT_MTV]--;
			g->p2.numStudents[STUDENT_MMONEY]--;
			g->p2.numPatent++;
		} else if (g->whoseTurn == UNI_C) {
			g->p3.numStudents[STUDENT_MJ]--;
			g->p3.numStudents[STUDENT_MTV]--;
			g->p3.numStudents[STUDENT_MMONEY]--;
			g->p3.numPatent++;
		}
	} else if (a.actionCode == RETRAIN_STUDENTS) {
		// creation of new variables to make the later code shorter
		int to = a.disciplineTo;
		int from = a.disciplineFrom;
		// subtract students that get converted
		// then add 1 of the new student type
		if (g->whoseTurn == UNI_A) {
			g->p1.numStudents[from] -= getExchangeRate(g, UNI_A, from, to);
			g->p1.numStudents[to]++;
		} else if (g->whoseTurn == UNI_B) {
			g->p2.numStudents[from] -= getExchangeRate(g, UNI_B, from, to);
			g->p2.numStudents[to]++;
		} else if (g->whoseTurn == UNI_C) {
			g->p3.numStudents[from] -= getExchangeRate(g, UNI_C, from, to);
			g->p3.numStudents[to]++;
		}
	}
	return;
}

//signal next turn and throw dice
void throwDice (Game g, int diceScore) {
	// incrememt turn number
	g->turnNumber++;
	// update whoseTurn
	g->whoseTurn = (g->turnNumber % 3) + 1;

	// let a helper function handle allocation of resources
	allocateResources(g, diceScore);

	if (diceScore == 7) {
		// replacing MTV and M$ with THD's
		g->p1.numStudents[STUDENT_THD] += g->p1.numStudents[STUDENT_MTV];
		g->p1.numStudents[STUDENT_THD] += g->p1.numStudents[STUDENT_MMONEY];
		g->p1.numStudents[STUDENT_MTV] = 0;
		g->p1.numStudents[STUDENT_MMONEY] = 0;
		g->p2.numStudents[STUDENT_THD] += g->p2.numStudents[STUDENT_MTV];
		g->p2.numStudents[STUDENT_THD] += g->p2.numStudents[STUDENT_MMONEY];
		g->p2.numStudents[STUDENT_MTV] = 0;
		g->p2.numStudents[STUDENT_MMONEY] = 0;
		g->p3.numStudents[STUDENT_THD] += g->p3.numStudents[STUDENT_MTV];
		g->p3.numStudents[STUDENT_THD] += g->p3.numStudents[STUDENT_MMONEY];
		g->p3.numStudents[STUDENT_MTV] = 0;
		g->p3.numStudents[STUDENT_MMONEY] = 0;
	}
	return;
}

//return resources at a hexagon
int getDiscipline (Game g, int regionID) {
	return g->discipline[regionID];
}

//returns dice number on a hexagon
int getDiceValue (Game g, int regionID) {
	return g->dice[regionID];
}

//return player with most ARCs and update Game struct
int getMostARCs (Game g) {
	//get current values
	int p1Count = g->p1.numARC;
	int p2Count = g->p2.numARC;
	int p3Count = g->p3.numARC;
	// calculate if there is a player with the highest num of ARCs
	// the function will return NO_ONE if there is a tie
	int player = playerWithHighest(p1Count, p2Count, p3Count);

	return player;
}

//return player with most publications and update Game struct
int getMostPublications (Game g) {
	//get current values
	int p1Count = g->p1.numPublication;
	int p2Count = g->p2.numPublication;
	int p3Count = g->p3.numPublication;
	// calculate if there a player has the highest num of Publications
	// the function will return NO_ONE if there is a tie
	int player = playerWithHighest(p1Count, p2Count, p3Count);

	return player;
}

//returns the turn number.
int getTurnNumber (Game g) {
	return g->turnNumber;
}

//return player whose turn it is
int getWhoseTurn (Game g) {
	return g->whoseTurn;
}

//type info of the campus
int getCampus(Game g, path pathToVertex) {
    // get the coordinate of the campus
	Coord c = getCampusCoord(g, pathToVertex);
	

	// return the type, using the attained coord to reference the campus
	return g->nodes[c.x][c.y].type;
}

//get type info of the ARC
int getARC(Game g, path pathToEdge) {
	// get the index of the ARC
	edgeRef index = getARCFromPath(g, pathToEdge);
	// return the type, using the index to reference the ARC
	return g->edges[index.coord.x][index.coord.y][index.index].type;
}

// checks if the specified action is legal
int isLegalAction (Game g, action a) {
	int isLegal = FALSE;
	if (a.actionCode == PASS) {
		isLegal = TRUE;
	} else if (a.actionCode == BUILD_CAMPUS) {
		Coord c = getCampusCoord(g, a.destination);
		// first verify that the node is a valid campus and isn't owned
		// and can be placed, obeying the rules of no adjacent campuses
		// and connecting to friendly ARCs
		if (g->nodes[c.x][c.y].type == VACANT_VERTEX &&
			g->nodes[c.x][c.y].valid == TRUE &&
			canPlaceCampus(g, g->whoseTurn, c) == TRUE) {
			if (g->whoseTurn == UNI_A) {
				// check if the player has the required resources
				if (g->p1.numStudents[STUDENT_BPS] >= 1 &&
					g->p1.numStudents[STUDENT_BQN] >= 1 &&
					g->p1.numStudents[STUDENT_MJ] >= 1 &&
					g->p1.numStudents[STUDENT_MTV] >= 1) {
					isLegal = TRUE;
				}
        	} else if (g->whoseTurn == UNI_B) {
				if (g->p2.numStudents[STUDENT_BPS] >= 1 &&
					g->p2.numStudents[STUDENT_BQN] >= 1 &&
					g->p2.numStudents[STUDENT_MJ] >= 1 &&
					g->p2.numStudents[STUDENT_MTV] >= 1) {
					isLegal = TRUE;
				}
        	} else if (g->whoseTurn == UNI_C) {
				if (g->p3.numStudents[STUDENT_BPS] >= 1 &&
					g->p3.numStudents[STUDENT_BQN] >= 1 &&
					g->p3.numStudents[STUDENT_MJ] >= 1 &&
					g->p3.numStudents[STUDENT_MTV] >= 1) {
					isLegal = TRUE;
				}
        	}
		}
	} else if (a.actionCode == BUILD_GO8) {
		Coord c = getCampusCoord(g, a.destination);
		if (g->whoseTurn == UNI_A) {
			// first check if the required resources are available
			if (g->p1.numStudents[STUDENT_MJ] >= 2 &&
				g->p1.numStudents[STUDENT_MMONEY] >= 3) {
				// then check if the node is an existing campus owned
				// by the player, and is a valid position on the board
				if (g->nodes[c.x][c.y].type == CAMPUS_A &&
					g->nodes[c.x][c.y].valid == TRUE) {
					isLegal = TRUE;
				}
			}
        } else if (g->whoseTurn == UNI_B) {
			if (g->p2.numStudents[STUDENT_MJ] >= 2 &&
				g->p2.numStudents[STUDENT_MMONEY] >= 3) {
				if (g->nodes[c.x][c.y].type == CAMPUS_B &&
					g->nodes[c.x][c.y].valid == TRUE) {
					isLegal = TRUE;
				}
			}
        } else if (g->whoseTurn == UNI_C) {
			if (g->p3.numStudents[STUDENT_MJ] >= 2 &&
				g->p3.numStudents[STUDENT_MMONEY] >= 3) {
				if (g->nodes[c.x][c.y].type == CAMPUS_C &&
					g->nodes[c.x][c.y].valid == TRUE) {
					isLegal = TRUE;
				}
			}
        }
	} else if (a.actionCode == OBTAIN_ARC) {
		edgeRef index = getARCFromPath(g, a.destination);
		if (g->edges[index.coord.x][index.coord.y][index.index].type == VACANT_ARC &&
			canPlaceARC(g, g->whoseTurn, index) == TRUE) {
			if (g->whoseTurn == UNI_A) {
				if (g->p1.numStudents[STUDENT_BPS] >= 1 &&
					g->p1.numStudents[STUDENT_BQN] >= 1) {
					isLegal = TRUE;
				}
        	} else if (g->whoseTurn == UNI_B) {
				if (g->p2.numStudents[STUDENT_BPS] >= 1 &&
					g->p2.numStudents[STUDENT_BQN] >= 1) {
					isLegal = TRUE;
				}
        	} else if (g->whoseTurn == UNI_C) {
				if (g->p3.numStudents[STUDENT_BPS] >= 1 &&
					g->p3.numStudents[STUDENT_BQN] >= 1) {
					isLegal = TRUE;
				}
        	}
		}
	} else if (a.actionCode == START_SPINOFF) {
	    if (getStudents(g, g->whoseTurn, STUDENT_MJ) >= 1 &&
		    getStudents(g, g->whoseTurn, STUDENT_MTV) >= 1 &&
		    getStudents(g, g->whoseTurn, STUDENT_MMONEY) >= 1) {
		    isLegal = TRUE;
		}
	} else if (a.actionCode == RETRAIN_STUDENTS) {
	    int rate = getExchangeRate(g, g->whoseTurn, a.disciplineFrom, a.disciplineTo);
	    if (g->whoseTurn == UNI_A) {
            if (g->p1.numStudents[a.disciplineFrom] >= rate) {
                isLegal = TRUE;
            }
        } else if (g->whoseTurn == UNI_B) {
            if (g->p2.numStudents[a.disciplineFrom] >= rate) {
                isLegal = TRUE;
            }
        } else if (g->whoseTurn == UNI_C) {
            if (g->p3.numStudents[a.disciplineFrom] >= rate) {
                isLegal = TRUE;
            }
        }
	}

	return isLegal;
}

//return kpi points of a player
int getKPIpoints (Game g, int player) {
	int points = 0;

	if (player == UNI_A) {
		points = countKPI(g, g->p1, player);
	} else if (player == UNI_B) {
		points = countKPI(g, g->p2, player);
	} else if (player == UNI_C) {
		points = countKPI(g, g->p3, player);
	}

	return points;
}

//return number of ARC grants the player has
int getARCs (Game g, int player) {
	int arcs = 0;

	if (player == UNI_A) {
		arcs = g->p1.numARC;
	} else if (player == UNI_B) {
		arcs = g->p2.numARC;
	} else if (player == UNI_C) {
		arcs = g->p3.numARC;
	}

	return arcs;
}

//return  number of GO8 campuses the player has
int getGO8s (Game g, int player) {
	int go8s = 0;

	if (player == UNI_A) {
		go8s = g->p1.numGO8;
	} else if (player == UNI_B) {
		go8s = g->p2.numGO8;
	} else if (player == UNI_C) {
		go8s = g->p3.numGO8;
	}

	return go8s;
}

//return number of Campuses the player has
int getCampuses (Game g, int player) {
	int campuses = 0;

	if (player == UNI_A) {
		campuses = g->p1.numCampus;
	} else if (player == UNI_B) {
		campuses = g->p2.numCampus;
	} else if (player == UNI_C) {
		campuses = g->p3.numCampus;
	}

	return campuses;
}

//return number of IP Patents the player has
int getIPs (Game g, int player) {
	int ips = 0;

	if (player == UNI_A) {
		ips = g->p1.numPatent;
	} else if (player == UNI_B) {
		ips = g->p2.numPatent;
	} else if (player == UNI_C) {
		ips = g->p3.numPatent;
	}

	return ips;
}

//return number of Publications the player has
int getPublications (Game g, int player) {
	int publications = 0;

	if (player == UNI_A) {
		publications = g->p1.numPublication;
	} else if (player == UNI_B) {
		publications = g->p2.numPublication;
	} else if (player == UNI_C) {
		publications = g->p3.numPublication;
	}

	return publications;
}

//returns number of a student type the player has
int getStudents (Game g, int player, int discipline) {
	int students = 0;

	if (player == UNI_A) {
		students = g->p1.numStudents[discipline];
	} else if (player == UNI_B) {
		students = g->p2.numStudents[discipline];
	} else if (player == UNI_C) {
		students = g->p3.numStudents[discipline];
	}

	return students;
}

//returns the exchange rate between student types
int getExchangeRate (Game g, int player, int disciplineFrom, int disciplineTo) {
	// the default exchange rate
	int rate = 3;

	int go8Type = player + 3;

	// hardcoded retraining centres ftw!!!!!
	if (disciplineFrom == STUDENT_BPS) {
		if (g->nodes[2][8].type == player || g->nodes[2][8].type == go8Type ||
			g->nodes[3][9].type == player || g->nodes[3][9].type == go8Type) {
			rate = 2;
		}
	} else if (disciplineFrom == STUDENT_BQN) {
		if (g->nodes[11][5].type == player || g->nodes[11][5].type == go8Type ||
			g->nodes[10][6].type == player || g->nodes[10][6].type == go8Type) {
			rate = 2;
		}
	} else if (disciplineFrom == STUDENT_MJ) {
		if (g->nodes[9][8].type == player || g->nodes[9][8].type == go8Type ||
			g->nodes[8][9].type == player || g->nodes[8][9].type == go8Type) {
			rate = 2;
		}
	} else if (disciplineFrom == STUDENT_MTV) {
		if (g->nodes[3][1].type == player || g->nodes[3][1].type == go8Type ||
			g->nodes[4][1].type == player || g->nodes[4][1].type == go8Type) {
			rate = 2;
		}
	} else if (disciplineFrom == STUDENT_MMONEY) {
		if (g->nodes[7][1].type == player || g->nodes[7][1].type == go8Type ||
			g->nodes[8][1].type == player || g->nodes[8][1].type == go8Type) {
			rate = 2;
		}
	}

	return rate;
}

static int isNode (Coord toTest) {
    int valid = 0;
    if (toTest.y >= NUM_Y || toTest.y < 0 || toTest.x >= NUM_X || toTest.x < 0) {
        // straight up not valid, do nothing
        valid = 0;
    } else if (toTest.y == 0 || toTest.y == NUM_Y - 1) {
        // top and bottom - two valid nodes which means hardcode
        if (toTest.x == 5 || toTest.x == 6) {
            valid = 1;
        }
    } else if (toTest.y == 1 || toTest.y == NUM_Y - 2) {
        // 2nd from (top || bottom) - four valid nodes
        if (toTest.x == 3 || toTest.x == 4 || toTest.x == 7 || toTest.x == 8) {
            valid = 1;
        }
    } else if (((toTest.x - 2) % 4 == 0 || (toTest.x - 1) % 4 == 0) && toTest.x != 0) {
        // expression evaluates to true if y % 2 == 0 and node is valid
        if (toTest.y % 2 == 0) {
            valid = 1;
        }
    } else if (toTest.y % 2 == 1) {
        // if the 2nd expression above is false and y % 2 != 0
        // then it is a valid node
        valid = 1;
    }

    return valid;
}

static void allocateResources(Game g, int diceScore) {
	int region = 0;
	int vertexNum;
	Coord temp;
	int regionCentres[NUM_REGIONS][NUM_AXES] = REGION_CENTRES;

	// check each region for the rolled dice number
	while (region < NUM_REGIONS) {
		if (g->dice[region] == diceScore) {
			vertexNum = 0;
			// check each point of a hex
			while (vertexNum < NODES_ON_HEX) {
				// set the temp coord to the "centre" of the region
				temp.x = regionCentres[region][X];
				temp.y = regionCentres[region][Y];
				// adjust the centre coord for each point of hex
				if (vertexNum == 0) {
					temp.y--;
				} else if (vertexNum == 1) {
					temp.x++;
					temp.y--;
				} else if (vertexNum == 2) {
					temp.x += 2;
				} else if (vertexNum == 3) {
					temp.x++;
					temp.y++;
				} else if (vertexNum == 4) {
					temp.y++;
				} else if (vertexNum == 5) {
					temp.x--;
				}

				// if the node is valid (part of the board) do stuff
				if (g->nodes[temp.x][temp.y].valid == TRUE) {
					// add 1 student for campuses
					if (g->nodes[temp.x][temp.y].type == CAMPUS_A) {
						g->p1.numStudents[g->discipline[region]]++;
					} else if (g->nodes[temp.x][temp.y].type == CAMPUS_B) {
						g->p2.numStudents[g->discipline[region]]++;
					} else if (g->nodes[temp.x][temp.y].type == CAMPUS_C) {
						g->p3.numStudents[g->discipline[region]]++;
					//add 2 students for GO8s
					} else if (g->nodes[temp.x][temp.y].type == GO8_A) {
						g->p1.numStudents[g->discipline[region]] += 2;
					} else if (g->nodes[temp.x][temp.y].type == GO8_B) {
						g->p2.numStudents[g->discipline[region]] += 2;
					} else if (g->nodes[temp.x][temp.y].type == GO8_C) {
						g->p3.numStudents[g->discipline[region]] += 2;
					}
				}
				vertexNum++;
			}
		}
		region++;
	}

	return;
}

// returns the player with the highest of a set of values
static int playerWithHighest (int aVal, int bVal, int cVal) {
	int player;

	//determine if there is a tie or a single highest value
	if (aVal > bVal && aVal > cVal) {
		player = UNI_A;
	} else if (bVal > aVal && bVal > cVal) {
		player = UNI_B;
	} else if (cVal > aVal && cVal > bVal) {
		player = UNI_C;
	} else {
		player = NO_ONE;
	}

	return player;
}

// adds up the KPI points of a specified player
static int countKPI (Game g, Player player, int playerNum) {
	int points = 0;
	//calculate the bulk of the kpi points
	points += (player.numARC * POINTS_FOR_ARC);
	points += (player.numGO8 * POINTS_FOR_GO8);
	points += (player.numCampus * POINTS_FOR_CAMPUS);
	points += (player.numPatent * POINTS_FOR_PATENT);

	//check if player should get points for having "most" of something
	if (g->uniWithMostARCs == playerNum) {
		points += POINTS_FOR_MOST_ARCS;
	}
	if (g->uniWithMostPublications == playerNum) {
		points += POINTS_FOR_MOST_PUBLICATIONS;
	}

	return points;
}

// determine if a campus can be placed at the specified coord
// takes into account surrounding campuses and ARCs
static int canPlaceCampus (Game g, int player, Coord c) {
	int canPlace = FALSE;
	int path = 0;
	edgeRef edge;
	Coord adjacentPt;

	// check all directions for ARC grants
	while (path < 6 && canPlace == FALSE) {
		adjacentPt.x = c.x;
		adjacentPt.y = c.y;
		if (path == 0) {
			adjacentPt.x++;
			adjacentPt.y--;
		} else if (path == 1) {
			adjacentPt.x++;
		} else if (path == 2) {
			adjacentPt.x++;
			adjacentPt.y++;
		} else if (path == 3) {
			adjacentPt.x--;
			adjacentPt.y++;
		} else if (path == 4) {
			adjacentPt.x--;
		} else if (path == 5) {
			adjacentPt.x--;
			adjacentPt.y--;
		}

		// check that the initial and adjacent points are a part of the board
		if (g->nodes[adjacentPt.x][adjacentPt.y].valid == TRUE &&
			g->nodes[c.x][c.y].valid == TRUE) {
			// make sure no one owns the campus already
			if (g->nodes[adjacentPt.x][adjacentPt.y].type == NO_ONE) {
				// make sure the player isn't NO_ONE (during Terra Nullius)
				if (player != NO_ONE) {
					edge = getARCFromCoord(g, c, adjacentPt);
					// check that the touching ARC belongs to the player
					if (g->edges[edge.coord.x][edge.coord.y][edge.index].type == player) {
						canPlace = TRUE;
					}
				}
			}
		}

		path++;
	}

	return canPlace;
}

// determine if an ARC (given by the index) can be placed
static int canPlaceARC (Game g, int player, edgeRef index) {
	int canPlace = FALSE;
	int path = 0;
	Coord start = index.coord;
	Coord end = g->edges[index.coord.x][index.coord.y][index.index].end;
	Coord new;
	edgeRef adjacentARC;

	// check all directions for ARC grants
	while (path < 12 && canPlace == FALSE && getARCs(g, player) > 0) {
		if (path < 6) {
			new = start;
		} else {
			new = end;
		}

		if (path % 6 == 0) {
			// path is 0 or 6
			new.x++;
			new.y--;
		} else if (path % 6 == 1) {
			// path is 1 or 7
			new.x++;
		} else if (path % 6 == 2) {
			// path is 2 or 8
			new.x++;
			new.y++;
		} else if (path % 6 == 3) {
			// path is 3 or 9
			new.x--;
			new.y++;
		} else if (path % 6 == 4) {
			// path is 4 or 10
			new.x--;
		} else if (path % 6 == 5) {
			// path is 5 or 11
			new.x--;
			new.y--;
		}

		if (new.x >= 0 && new.y >= 0) {
		    if (g->nodes[new.x][new.y].valid == TRUE) {
			    if (path < 6) {
				    adjacentARC = getARCFromCoord(g, start, new);
			    } else {
				    adjacentARC = getARCFromCoord(g, end, new);
			    }

			    if (adjacentARC.coord.x >= 0 && adjacentARC.coord.x < NUM_X &&
				    adjacentARC.coord.y >= 0 && adjacentARC.coord.y < NUM_Y) {
				    // if the connected ARC is valid
				    if (g->edges[adjacentARC.coord.x][adjacentARC.coord.y][adjacentARC.index].valid == TRUE) {
					    // if the connected ARC belongs to the player
					    if (g->edges[adjacentARC.coord.x][adjacentARC.coord.y][adjacentARC.index].type == player &&
					        player != NO_ONE) {
						    canPlace = TRUE;
					    }
				    }
			    }
			}
		}
		path++;
	}

	if (getARCs(g, player) == 0) {
		if (g->nodes[end.x][end.y].type == player ||
			g->nodes[index.coord.x][index.coord.y].type == player) {
			canPlace = TRUE;
		}
	}

	return canPlace;
}

// return the ARC index based on the supplied start/end coords
static edgeRef getARCFromCoord (Game g, Coord start, Coord end) {
	edgeRef ret = {start, 0};
	int i = 0;
	while (i < 3) {
		if (compareEdge(g->edges[start.x][start.y][i].end, end) == TRUE) {
			ret.coord = start;
			ret.index = i;
		} else if (compareEdge(g->edges[end.x][end.y][i].end, start) == TRUE) {
			ret.coord = end;
			ret.index = i;
		}
		i++;
	}

	return ret;
}

// returns the coord of a campus, given a path
static Coord getCampusCoord (Game g, path p) {
	// RRLRL (0, NUM_Y - 4)
	// uses same directions as in static Coord transform
	Coord c = {5, 0};
	int direction = 1;
	int i = 0;
	char current = '\0';
	if (strlen(p) > 0) {
	    while (i < strlen(p)) {
		    current = p[i];
		    if (current == 'R') {
			    direction += 1;
		    } else if (current == 'L') {
			    direction += 5; // +5 instead of -1 because the % operator in C is remainder, not modulo
		    } else {
			    direction += 3;
		    }
		    direction = direction % 6;

		    c = transform(direction, c);
		    i++;
	    }
	}

	return c;
}

// returns the index of an ARC given the path
// my note: I'm pretty sure the path will lead to the end of the arc
// and to find the start of the arc you can just get rid of the last
// character in the path string
static edgeRef getARCFromPath(Game g, path p) {
	edgeRef index = {{5,0}, 0};
	Coord start, end;
	int i = 0;
	// if the strlen is 1 or 0 you can't have mutilatedP[0] or mutilatedO[-1]
	// so it segfaults
	//simplest solution is to hardcode these edge cases
	if (strlen(p) == 0) {
	    start = index.coord;
	    end = index.coord;
	} else if (strlen(p) == 1) {
	    start = index.coord;
	    end = getCampusCoord(g, p);
	} else {
	    char mutilatedP[strlen(p) - 1];
	    while (i < strlen(p) - 1) {
		    mutilatedP[i] = p[i];
		    i++;
	    }
	    //mutilatedP[strlen(p)] = '\0';
	    start = getCampusCoord(g, mutilatedP);
	    end = getCampusCoord(g, p);
	}

	i = 0;
	while (i < 3) {
		if (compareEdge(g->edges[start.x][start.y][i].end, end) == TRUE) {
			index.coord = start;
			index.index = i;
		} else if (compareEdge(g->edges[end.x][end.y][i].end, start) == TRUE) {
			index.coord = end;
			index.index = i;
		}
		i++;
	}

	return index;
}

static Coord transform(int direction, Coord initial) {
	// direction has values 0-5, 0 being straight right and moving clockwise from there
	Coord transformed = initial;

	// y transform
	if (direction == 4 || direction == 5) {
		// go upwards if it does not go out of bounds
		if (transformed.y >= 1) { transformed.y--; }
	} else if (direction == 1 || direction == 2) {
		// downwards if not out of bounds
		if (transformed.y < NUM_Y - 2) { transformed.y++; }
	}

	// x transform
	if (direction <= 1 || direction == 5) {
		if (transformed.x <= NUM_X - 1) { transformed.x++; }
	} else {
		if (transformed.x >= 1) { transformed.x--; }
	}

	return transformed;
}

static int compareEdge(Coord a, Coord b) {
	int result = FALSE;
	if (a.x == b.x && a.y == b.y) {
		result = TRUE;
	}
	return result;
}