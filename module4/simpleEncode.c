/* * simpleEncode.c *
 * reads in a permutation of the alphabet then encodes
 * lower case letters using that permutation 
 * module 4 template code you asked for - might need some cleaning up... * 
 * Created by Julian Saknussemm on 11/09/1752 * Licensed under Creative Commons BY 3.0. *
 * Edited by Andrew Timkov 
 * 
 */ 
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define STOP -1 
#define ALPHABET_SIZE 26
#define FIRST_ASCII 97
#define LAST_ASCII 122

char encode (int plainChar, char *permutation);
void testEncode (void);

int main (int argc, char *argv[]) {
	testEncode();
	char permutation[ALPHABET_SIZE];
	scanf ("%s", permutation); 
	// getchar() reads and returns one character from the keyboard 
	// returns -1 when the input is finished / end-of-file is reached 
	// signal this from the kayboard by pressing ^D in linux/^Z windows 
	int plainCharacter = getchar(); 
	while (plainCharacter != STOP) { 
		int encodedCharacter = encode (plainCharacter, permutation); 
		printf ("%c", encodedCharacter); 
		plainCharacter = getchar(); 
	} 

	return EXIT_SUCCESS; 
} 

void testEncode (void) { 
	assert (encode ('A',"abcdefghijklmnopqrstuvwxyz") == 'A'); 
	assert (encode ('?',"abcdefghijklmnopqrstuvwxyz") == '?'); 
	assert (encode (' ',"abcdefghijklmnopqrstuvwxyz") == ' '); 
	assert (encode ('\n',"abcdefghijklmnopqrstuvwxyz") == '\n'); 
	assert (encode ('a',"abcdefghijklmnopqrstuvwxyz") == 'a'); 
	assert (encode ('m',"abcdefghijklmnopqrstuvwxyz") == 'm'); 
	assert (encode ('z',"abcdefghijklmnopqrstuvwxyz") == 'z'); 
	assert (encode ('a',"bcdefghijklmnopqrstuvwxyza") == 'b'); 
	assert (encode ('m',"bcdefghijklmnopqrstuvwxyza") == 'n'); 
	assert (encode ('z',"bcdefghijklmnopqrstuvwxyza") == 'a'); 
	assert (encode ('a',"qwertyuiopasdfghjklzxcvbnm") == 'q'); 
	assert (encode ('b',"qwertyuiopasdfghjklzxcvbnm") == 'w'); 
	assert (encode ('z',"qwertyuiopasdfghjklzxcvbnm") == 'm');

	return;
} 

char encode (int plainChar, char *permutation) { 
	int index = plainChar - FIRST_ASCII;
	int newChar;

	if (plainChar >= FIRST_ASCII && plainChar <= LAST_ASCII) {
		newChar = permutation[index];
	} else {
		newChar = plainChar;
	}

	return newChar;
}