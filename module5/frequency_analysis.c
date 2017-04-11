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
#define LF_ASCII 10
#define SPACE_ASCII 32
#define INDEX_OF_A 0
#define INDEX_OF_E 4
#define INDEX_OF_I 8
#define INDEX_OF_O 14
#define INDEX_OF_U 20

int main (int argc, char *argv[]) {
	int frequencies[ALPHABET_SIZE] = {0};
	int totalLetterCount = 0;
	int upperCount = 0;
	int lowerCount = 0;
	int lfCharCount = 0;
	int spaceCount = 0;
	
	int i = 0;
	
	int inputChar = getchar(); //get a character for txt file

	//while EOF is not reached
	while (inputChar != STOP) {
		int index = -1;

		//first test for uppercase, then lowercase, then lf, then space
		if (inputChar >= UPPER_A && inputChar <= UPPER_Z) {
			index = inputChar - UPPER_A;
			upperCount++;
		} else if (inputChar >= LOWER_A && inputChar <= LOWER_Z) {
			index = inputChar - LOWER_A;
			lowerCount++;
		} else if (inputChar == LF_ASCII) {
			lfCharCount++;
		} else if (inputChar == SPACE_ASCII) {
			spaceCount++;
		}

		//if above code did anything to index
		if (index != STOP) {
			frequencies[index] += 1;
			totalLetterCount++;
		}

		inputChar = getchar();
	}

	printf("Total letter count: %d\n", totalLetterCount);
	printf("%d LF character occurrences\n", lfCharCount);
	printf("%d Space character occurrences\n", spaceCount);
	printf("%d Uppercase characters\n", upperCount);
	printf("%d Lowercase characters\n", lowerCount);

	double vowelProportion, vowelCount;
	vowelCount = (double) frequencies[INDEX_OF_A]
			   + (double) frequencies[INDEX_OF_E]
			   + (double) frequencies[INDEX_OF_I]
			   + (double) frequencies[INDEX_OF_O]
			   + (double) frequencies[INDEX_OF_U];
	vowelProportion = vowelCount / (double) totalLetterCount * 100;
	printf("Vowel Proportion: %.2f%%\n", vowelProportion);

	printf("Letter	Occurrences	Proportion\n");

	i = 0; //reset the iterator to 0

	//loop through frequencies array
	while (i < ALPHABET_SIZE) {
		char letter = LOWER_A + i;
		int count = frequencies[i];
		double proportion = (double) count / (double) totalLetterCount * 100;
		printf("%c	%d		%.5f%%\n", letter, count, proportion);
		i++;
	}

	return EXIT_SUCCESS;
}