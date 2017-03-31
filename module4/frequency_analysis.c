/*
 * Andrew Timkov
 * UNSW Tutorials
 * Date: 31/3/17
 * Frequency Analysis
 * A program to analyse the frequency of letter in a txt file
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define STOP -1

#define ALPHABET_SIZE 26
#define UPPER_A 65
#define UPPER_Z 90
#define LOWER_A 97
#define LOWER_Z 122

int main (int argc, char *argv[]) {
	int frequencies[ALPHABET_SIZE];
	int totalLetterCount = 0;
	
	int i = 0;

	//this loop sets all values in "frequencies" to 0
	while (i < ALPHABET_SIZE) {
		frequencies[i] = 0;
		i++;
	}
	
	int inputChar = getchar(); //get a character for txt file

	while (inputChar != STOP) {
		int index = -1;

		//first test for uppercase, then lowercase
		if (inputChar >= UPPER_A && inputChar <= UPPER_Z) {
			index = inputChar - UPPER_A;
		} else if (inputChar >= LOWER_A && inputChar <= LOWER_Z) {
			index = inputChar - LOWER_A;
		}

		if (index != STOP) {
			frequencies[index] += 1;
			totalLetterCount++;
		}

		inputChar = getchar();
	}

	printf("Total letter count: %d\n", totalLetterCount);
	printf("Letter	Occurrences	Proportion\n");


	i = 0; //reset the iterator to 0

	while (i < ALPHABET_SIZE) {
		char letter = LOWER_A + i;
		int count = frequencies[i];
		double proportion = (double) count / (double) totalLetterCount * 100;

		printf("%c	%d		%.2f%%\n", letter, count, proportion);
		i++;
	}

	return EXIT_SUCCESS;
}