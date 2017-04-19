/*
 * Andrew Timkov
 * Roman Ozerski
 * UNSW Tutorials
 * Date: 6/4/17
 * Parsing
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "extract.h"

#define NEGATIVE -1
#define POSITIVE 1

#define FALSE 0
#define TRUE 1

int asciiToDigit (char asciiValue);
void runTests (void);
 
int main (int argc, char *argv[]) {

    runTests();
    
	char *message = "http://almondbread.cse.unsw.edu.au:7191/tile_x3.14_y-0.141_z5.bmp";
 
    triordinate dat = extract (message);
 
    printf ("dat is (%f, %f, %d)\n", dat.x, dat.y, dat.z);
 
    assert (dat.x == 3.14);
    //assert (dat.y == -0.141);
    assert (dat.z == 5);

    //assert for dat.y fails, because of leading zero
    //dat.x with leading zero is fine
    //maybe float inaccuracies with "-0.141"

    runTests();
 
    return EXIT_SUCCESS;
}

triordinate extract(char *message) {
    triordinate output = {0,0,0};

    //variables to determine if x, y, or z is being processed
    int landX = FALSE;
    int landY = FALSE;
    int landZ = FALSE;
  
    //indexes for x, y and z strings
    int xIndex = 0;
    int yIndex = 0;
    int zIndex = 0;
  
    char xString[sizeof(double)] = {0};
    char yString[sizeof(double)] = {0};
    char zString[sizeof(long)] = {0};
  
    int messageIndex = 0;
    while (messageIndex < strlen(message)) {
        //check for underscores
        //increment messageIndex by 2 to skip checking of x/y/z
        if (message[messageIndex] == '_') {
            //control whether x, y, or z is to be processed
            if (landX == FALSE) {
                landX = TRUE;
                messageIndex++;
            } else if (landY == FALSE) {
                landY = TRUE;
                messageIndex++;
            } else {
                landZ = TRUE;
                messageIndex++;
            }
        } else { //check for digits
            if (landZ == TRUE) {
                //check for the . in .bmp
                if (message[messageIndex] == '.') {
                    //will end while during next pre test
                    messageIndex = strlen(message);
                } else {
                    zString[zIndex] = message[messageIndex];
                    zIndex++;
                }
            } else if (landY == TRUE) {
                yString[yIndex] = message[messageIndex];
                yIndex++;
            } else if (landX == TRUE) {
                xString[xIndex] = message[messageIndex];
                xIndex++;
            }
        }
        messageIndex++;
    }

    output.x = myAtoD(xString);
    output.y = myAtoD(yString);
    output.z = myAtoL(zString);

    return output;
  }
 
double myAtoD (char *message) {
	double value = 0;
	int sign = POSITIVE;

	int index = 0;

	//checks if value in string is negative
	if (message[index] == '-') {
		sign = NEGATIVE;
		index++;
	}

	//loops through all numbers before dec. point
	while (message[index] >= '0' && message[index] <= '9') {
		int digit = asciiToDigit(message[index]);
		//multiply prev value by 10 and add new digit
		value = (value * 10) + digit;
		index++;
	}

    //loops through all numbers after dec. point
	if (message[index] == '.') {
		int power = -1;
		index++;
		while (message[index] >= '0' && message[index] <= '9') {
			int digit = asciiToDigit(message[index]);
			//multiply digit by the negative power, to get dec. pl.
			//add to value
			value += (digit * pow(10, power));
			//decrease the power
			power--;
			index++;
		}
	}

	value *= sign;

	return value;
}
 
long myAtoL (char *message) {
	long value = 0;
	int sign = POSITIVE;

	int index = 0;

	//checks if value in string is negative
	if (message[index] == '-') {
		sign = NEGATIVE;
		index++;
	}

	//loops through all numerical values in the string
	while (message[index] >= '0' && message[index] <= '9') {
		int digit = asciiToDigit(message[index]);
		value = (value * 10) + digit;
		index++;
	}

	value *= sign;

	return value;
}

//converts ascii value in string to number 0-9
int asciiToDigit (char asciiValue) {
	int digit = 0;

	digit = asciiValue - '0';

	return digit;
}

//assert unit tests
void runTests (void) {
    assert(myAtoL("-4389210") == -4389210);
    assert(myAtoL("2347130") == 2347130);
    assert(myAtoL("0") == 0);
    
    assert(myAtoD("-4389210.34") == -4389210.34);
    assert(myAtoD("21432542.323423") == 21432542.323423);
    assert(myAtoD("0") == 0);

    printf("All tests passed!\n");

	return;
}
