/*
	Luke Fisk-Lennon
	Andrew Timkov
	UNSW Tutorials
	22/03/17
	A function which swaps the value of two variables
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void swap (int *first, int *second);

int main (int argc, char *argv[]) {
	/*int first = 1;
	int second = 2;
	
	swap(&first, &second);
	
	assert(first == 2);
	assert(second == 1);
	
	printf("All tests passed: you are awesome! :)\n");
	
	printf("first is %d\n", first);
	printf("second is %d\n", second);*/
	
	return EXIT_SUCCESS;
}

void swap (int *first, int *second) {
	int tempFirst = *second;
	int tempSecond = *first;
	
	*first = tempFirst;
	*second = tempSecond;
	
	return;
}
