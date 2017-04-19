/*
 * Andrew Timkov z5169762
 * UNSW Tutorials
 * Date: 10/4/17
 * Reverse a string
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

char *reverse (char *message);
void testReverse (void);

int main (int argc, char *argv[]) {
	
	//black box test
	/*char *pointer = reverse("dlroW olleH");
	while (*pointer != '\0') {
		printf("%c", *pointer);
		pointer++;
	}*/

	//unit test
	testReverse();

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

void testReverse (void) {
	assert(strcmp(reverse("test"), "tset") == 0);
	assert(strcmp(reverse("Hello World"), "dlroW olleH") == 0);
	assert(strcmp(reverse("___l33t___"), "___t33l___") == 0);
	assert(strcmp(reverse("racecar"), "racecar") == 0);
	assert(strcmp(reverse("!emosewA erA uoY"), "You Are Awesome!") == 0);
	assert(strcmp(reverse("qwerty"), "ytrewq") == 0);
	assert(strcmp(reverse("..,,??~~"), "~~??,,..") == 0);

	printf("All tests passed!\n");
	
	return;
}