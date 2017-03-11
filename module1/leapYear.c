//Andrew Timkov
//Luke
//UNSW Tutorials
//8/3/2017

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define START_OF_GREGORIAN_CALENDAR 1582
#define TRUE 1
#define FALSE 0

int main(int argc, char *argv[]) {
    int year = 0;
    int isLeapYear;
    
    printf("please enter the year you are interested in\n");
    scanf("%d", &year);
    
    assert(year >= START_OF_GREGORIAN_CALENDAR);
    
    if (year % 4 == 0) {
        isLeapYear = TRUE;
        if (year % 400 == 0) {
            isLeapYear = TRUE;
        } else {
            if (year % 100 == 0) {
                isLeapYear = FALSE;
            } else {
                isLeapYear = TRUE;
            }
        }
    } else {
        isLeapYear = FALSE;
    }
    
    if (isLeapYear) {
        printf("%d is a leap year!\n", year);
    } else {
        printf("%d is not a leap year!\n", year);
    }
    
    return EXIT_SUCCESS;
}