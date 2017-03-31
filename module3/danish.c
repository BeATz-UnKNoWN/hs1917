/*
 * Andrew Timkov
 * UNSW Tutorials
 * Date: 19/3/17
 * Danish Flag (with loops)
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define FLAG_LENGTH 12
#define FLAG_HEIGHT 5
#define FLAG_GAP_LOCATION 3

void showDanish (void);

int main (int argc, char *argv[]) {
	showDanish();

	return EXIT_SUCCESS;
}

void showDanish (void) {
	int row = 1;

	while (row <= FLAG_HEIGHT) {
		int col = 1;
		
		if (row != FLAG_GAP_LOCATION) {
			while (col <= FLAG_LENGTH) {
				if (col == FLAG_GAP_LOCATION) {
					printf(" ");
				} else {
					printf("*");
				}
				col++;
			}
		}
		row++;
		printf("\n");
	}

	return;
}