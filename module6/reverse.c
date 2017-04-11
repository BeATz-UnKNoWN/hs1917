/*
 * Andrew Timkov
 * UNSW Tutorials
 * Date: 10/4/17
 * Reverse a string
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *reverse (char *message);

int main (int argc, char *argv[]) {
	char *pointer = reverse("dlroW olleH");

	while (*pointer != '\0') {
		printf("%c", *pointer);
		pointer++;
	}

	return EXIT_SUCCESS;
}

char *reverse (char *message) {
	//+1 is to account for null terminator
	char *reversed = malloc(strlen(message) + 1);

	//-1 to leave end location for null terminator
	int index = strlen(message) - 1;
	//write in null terminator as last element
	reversed[strlen(message)] = '\0';

	//while index is still a valid index number
	while (index >= 0) {
		reversed[index] = *message;
		//add to the pointer, to point to next element
		message++;
		//decrement index
		index--;
	}

	return reversed;
}