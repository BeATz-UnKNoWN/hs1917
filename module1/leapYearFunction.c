//Andrew Timkov
//Luke Fisk-Lennon
//UNSW Tutorials
//11/3/2017

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define START_OF_GREGORIAN_CALENDAR 1582
#define TRUE 1
#define FALSE 0

//function declarations
void runTests(void);
int getYear(void);
int isLeapYear(int year);
void printIsLeapYear(int year, int isYearLeap);

int main(int argc, char *argv[]) {
    runTests();
    
    int year = 0;
    int isYearLeap;
    
    year = getYear();
    isYearLeap = isLeapYear(year);
    
    printIsLeapYear(year, isYearLeap);
    
    return EXIT_SUCCESS;
}

int getYear(void) {
    int year;
    
    printf("please enter the year you are interested in\n");
    scanf("%d", &year);
    
    assert(year >= START_OF_GREGORIAN_CALENDAR);
    
    return year;
}

int isLeapYear(int year) {
    int isYearLeap;
    if (year % 4 == 0) {
        isYearLeap = TRUE;
        if (year % 400 == 0) {
            isYearLeap = TRUE;
        } else {
            if (year % 100 == 0) {
                isYearLeap = FALSE;
            } else {
                isYearLeap = TRUE;
            }
        }
    } else {
        isYearLeap = FALSE;
    }
    
    return isYearLeap;
}

void printIsLeapYear(int year, int isYearLeap) {
    if (isYearLeap) {
        printf("%d is a leap year!\n", year);
    } else {
        printf("%d is not a leap year!\n", year);
    }
    
    return;
}

void runTests(void) {
    assert(isLeapYear(2017) == FALSE);
    assert(isLeapYear(4000) == TRUE);
    assert(isLeapYear(1800) == FALSE);
    assert(isLeapYear(2016) == TRUE);
    assert(isLeapYear(1582) == FALSE);
    
    printf("All test cases passed!\n");
    printf("You are awesome!\n");
    
    return;
}