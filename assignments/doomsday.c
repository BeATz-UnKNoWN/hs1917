/*
 *  submitted 18/3/17
 *  doomsday.c
 *  A program to test a function dayOfWeek which determines which
 *  day of the week a particular date falls on.
 *  (only for dates after the start of the Gregorian calendar).
 *
 *  Program stub created by Richard Buckland on 17/03/14
 *  This program edited by Andrew Timkov 5169762
 *  Freely licensed under Creative Commons CC-BY-3.0
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
 
#define THURSDAY  0
#define FRIDAY    1
#define SATURDAY  2
#define SUNDAY    3
#define MONDAY    4
#define TUESDAY   5
#define WEDNESDAY 6

#define JANUARY   1
#define FEBRUARY  2
#define MARCH     3
#define APRIL     4
#define MAY       5
#define JUNE      6
#define JULY      7
#define AUGUST    8
#define SEPTEMBER 9
#define OCTOBER   10
#define NOVEMBER  11
#define DECEMBER  12

#define START_OF_GREGORIAN_CALENDAR 1582
#define DAYS_PER_WEEK 7

#define DOOMSDAY_DATE_JAN 3
#define DOOMSDAY_DATE_FEB 28
#define DOOMSDAY_DATE_MAR 7
#define DOOMSDAY_DATE_APR 4
#define DOOMSDAY_DATE_MAY 9
#define DOOMSDAY_DATE_JUN 6
#define DOOMSDAY_DATE_JUL 11
#define DOOMSDAY_DATE_AUG 8
#define DOOMSDAY_DATE_SEP 5
#define DOOMSDAY_DATE_OCT 10
#define DOOMSDAY_DATE_NOV 7
#define DOOMSDAY_DATE_DEC 12
 
#define TRUE 1
#define FALSE 0
 
int dayOfWeek (int doomsday, int leapYear, int month, int day);
int getReferenceDoomsday (int leapYear, int month);
void runTests (void);
 
int main (int argc, char *argv[]) {
    runTests();

    return EXIT_SUCCESS;
}
 
 
// given the doomsday for a year, and whether or not it is a
// leap year, this function return the day of the week for any
// given month and day in the year.
 
int dayOfWeek (int doomsday, int leapYear, int month, int day) {
    int dayOfWeek;
    int referenceDoomsday;
    int dayDifference;
    int daysAfterDoomsday;
 
    referenceDoomsday = getReferenceDoomsday(leapYear, month);

    //dayDifference is number of days after a memorable doomsday
    dayDifference = day - referenceDoomsday;

    //This should ensure daysAfterDoomsday is always positive
    //35 is still divisible by 7 so it doesn't affect the end value
    daysAfterDoomsday = (dayDifference + 35) % 7;

    dayOfWeek = (doomsday + daysAfterDoomsday) % 7;
 
    return dayOfWeek;
}

//returns one of the memory doomsdays, based on month
int getReferenceDoomsday (int leapYear, int month) {
    int referenceDoomsday = 0;

    if (month == JANUARY) {
        referenceDoomsday = DOOMSDAY_DATE_JAN;
        if (leapYear) {
            referenceDoomsday += 1;
        }
    } else if (month == FEBRUARY) {
        referenceDoomsday = DOOMSDAY_DATE_FEB;
        if (leapYear) {
            referenceDoomsday += 1;
        }
    } else if (month == MARCH) {
        referenceDoomsday = DOOMSDAY_DATE_MAR;
    } else if (month == APRIL) {
        referenceDoomsday = DOOMSDAY_DATE_APR;
    } else if (month == MAY) {
        referenceDoomsday = DOOMSDAY_DATE_MAY;
    } else if (month == JUNE) {
        referenceDoomsday = DOOMSDAY_DATE_JUN;
    } else if (month == JULY) {
        referenceDoomsday = DOOMSDAY_DATE_JUL;
    } else if (month == AUGUST) {
        referenceDoomsday = DOOMSDAY_DATE_AUG;
    } else if (month == SEPTEMBER) {
        referenceDoomsday = DOOMSDAY_DATE_SEP;
    } else if (month == OCTOBER) {
        referenceDoomsday = DOOMSDAY_DATE_OCT;
    } else if (month == NOVEMBER) {
        referenceDoomsday = DOOMSDAY_DATE_NOV;
    } else if (month == DECEMBER) {
        referenceDoomsday = DOOMSDAY_DATE_DEC;
    }

  return referenceDoomsday;
}

void runTests (void) {
    assert(dayOfWeek (THURSDAY,  FALSE,  4,  4) == THURSDAY);
    assert(dayOfWeek (FRIDAY,    FALSE,  6,  6) == FRIDAY);
    assert(dayOfWeek (MONDAY,    FALSE,  8,  8) == MONDAY);
    assert(dayOfWeek (WEDNESDAY, FALSE, 10, 10) == WEDNESDAY);
    assert(dayOfWeek (FRIDAY,    FALSE, 12, 12) == FRIDAY);
    assert(dayOfWeek (THURSDAY,  FALSE,  9,  5) == THURSDAY);
    assert(dayOfWeek (FRIDAY,    FALSE,  5,  9) == FRIDAY);
    assert(dayOfWeek (SUNDAY,    FALSE,  7, 11) == SUNDAY);
    assert(dayOfWeek (TUESDAY,   FALSE, 11,  7) == TUESDAY);
    assert(dayOfWeek (WEDNESDAY, FALSE,  3,  7) == WEDNESDAY);

    assert(dayOfWeek (THURSDAY,  FALSE,  4,  5) == FRIDAY);
    assert(dayOfWeek (SATURDAY,  FALSE,  6,  5) == FRIDAY);
    assert(dayOfWeek (MONDAY,    FALSE,  8,  9) == TUESDAY);
    assert(dayOfWeek (WEDNESDAY, FALSE, 10,  9) == TUESDAY);
    assert(dayOfWeek (FRIDAY,    FALSE, 12, 20) == SATURDAY);
    assert(dayOfWeek (THURSDAY,  FALSE,  9,  9) == MONDAY);
    assert(dayOfWeek (FRIDAY,    FALSE,  5,  5) == MONDAY);
    assert(dayOfWeek (SUNDAY,    FALSE,  7,  7) == WEDNESDAY);
    assert(dayOfWeek (TUESDAY,   FALSE, 11, 11) == SATURDAY);
    assert(dayOfWeek (THURSDAY,  FALSE,  3, 30) == SATURDAY);
 
    assert(dayOfWeek (TUESDAY,   FALSE,  2,  28) == TUESDAY);
    assert(dayOfWeek (TUESDAY,   FALSE,  2,  27) == MONDAY);
    assert(dayOfWeek (THURSDAY,  FALSE,  1,  3)  == THURSDAY);

    //tests made by students of hs1917
    assert (dayOfWeek (WEDNESDAY, FALSE, 8, 31) == FRIDAY); //Angela Finlayson
    assert (dayOfWeek (TUESDAY, TRUE, 8, 18) == FRIDAY); // Stanley Su
    assert (dayOfWeek (MONDAY, TRUE, 1, 1) == FRIDAY); // Alex Huang
    assert (dayOfWeek (SUNDAY, FALSE, 9,8591) == THURSDAY); //alex huang (only professional nerds get this one)
    assert (dayOfWeek (TUESDAY, TRUE, 2, 28) == MONDAY); // Felicia Ee
    assert (dayOfWeek (SATURDAY, FALSE, 1, 2) == FRIDAY);//Maksim Lisau
    assert (dayOfWeek (SATURDAY, FALSE, 3, 15) == SUNDAY); //Angeni Bai
    assert (dayOfWeek (WEDNESDAY, FALSE, 12, 19) == dayOfWeek (WEDNESDAY, TRUE, 12, 19) ); //Marcus Handley
    assert (dayOfWeek (FRIDAY, TRUE, 2, 29) == FRIDAY); // Alex Tan
    assert (dayOfWeek (MONDAY, TRUE, 1, 9) == SATURDAY); // Luke Fisk-Lennon
    assert (dayOfWeek (THURSDAY, TRUE, 2, 29) == THURSDAY); // Jarrod Li
    assert (dayOfWeek (FRIDAY, TRUE, 1, 3) == THURSDAY); //Mirela Tomicic
    assert (dayOfWeek (TUESDAY, FALSE, 3, 16) == THURSDAY); // Andrew Yu
    assert (dayOfWeek (SUNDAY, FALSE, 7, 11) == SUNDAY); //Samuel Hagan
    assert (dayOfWeek (TUESDAY, FALSE, 11, 12) == SUNDAY); //Tracey Chiu
    assert (dayOfWeek (MONDAY, TRUE, 12, 31) == SATURDAY); //Andrew Timkov
 
    printf("all tests passed - You are awesome!\n");
}