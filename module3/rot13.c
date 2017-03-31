/*
	Luke Fisk-Lennon
	Andrew Timkov
	UNSW Tutorials
	22/03/17
	A function which encodes a single character using ROT13
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define ROT_NUM 13

#define UPPER_MIN 65
#define UPPER_MAX 90
#define LOWER_MIN 97
#define LOWER_MAX 122

char encode (char letter);
char increaseChar (char letter, int max, int min);
void runTests (void);


int main (int argc, char *argv[]) {
	//runTests();
	
	return EXIT_SUCCESS;
}

char encode (char letter) {
	// Lowercase
	if (letter >= LOWER_MIN && letter <= LOWER_MAX) {
		letter = increaseChar(letter, LOWER_MIN, LOWER_MAX);
	}
	
	// Uppercase
	if (letter >= UPPER_MIN && letter <= UPPER_MAX) {
		letter = increaseChar(letter, UPPER_MIN, UPPER_MAX);
	}
	
	return letter;
}

char increaseChar (char letter, int min, int max) {
	int tempLetter = letter + ROT_NUM;
	
	if (tempLetter > max) {
		letter = min + tempLetter - max - 1;
	} else {
		letter += ROT_NUM;
	}
	
	return letter;
}

void runTests (void) {
	assert (encode ('a') == 'n');
	assert (encode ('A') == 'N');
	
	assert (encode ('n') == 'a');
	assert (encode ('N') == 'A');
	
	assert (encode ('z') == 'm');
	assert (encode ('m') == 'z');
	assert (encode ('Z') == 'M');
	assert (encode ('M') == 'Z');
	
	assert (encode ('g') == 't');
	assert (encode ('G') == 'T');
	 
	assert (encode ('t') == 'g');
	assert (encode ('T') == 'G');
	
	printf("All tests passed: you are awesome! :)\n");
	
	return;
}
