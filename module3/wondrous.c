/*
 * Andrew Timkov
 * UNSW Tutorials
 * Date: 19/3/17
 * The Wondrous Sequence
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define FALSE 0
#define TRUE 1

int printWondrous (int start);

int main (int argc, char *argv[]) {
	int returnValue = printWondrous(4);

	printf("number of terms: %d", returnValue);

	return EXIT_SUCCESS;
}

int printWondrous (int start) {
	int current;
	int counter = 1;
	current = start;

	while (current != 1) {
		printf("%d ", current);
		if (current % 2 == 0) {
			current /= 2;
		} else {
			current = current * 3 + 1;
		}
		counter++;
	}
	printf("1\n");

	return counter;
}