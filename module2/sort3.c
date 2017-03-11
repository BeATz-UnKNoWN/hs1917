/*
 * Andrew Timkov
 * UNSW Tutorials
 * Date: 11/3/17
 * Sort Program
 */

//includes
#include <stdio.h>
#include <stdlib.h>

//function declarations
void sortNumbers(int num1, int num2, int num3);
void printResults(int sorted1, int sorted2, int sorted3);

int main(int argc, char* argv[]) {
    int num1;
    int num2;
    int num3;

    scanf("%d %d %d", &num1, &num2, &num3);

    sortNumbers(num1, num2, num3);

    return EXIT_SUCCESS;
}

void sortNumbers(int num1, int num2, int num3) {
    int sorted1;
    int sorted2;
    int sorted3;

    if(num1 >= num2) {
        if(num3 >= num2) {
            sorted1 = num2;
            if(num3 >= num1) {
                sorted2 = num1;
                sorted3 = num3;
            } else {
                sorted2 = num3;
                sorted3 = num1;
            }
        } else {
            sorted1 = num3;
            sorted2 = num2;
            sorted3 = num1;
        }
    } else {
        if(num3 >= num1) {
            sorted1 = num1;
            if(num3 >= num2) {
                sorted2 = num2;
                sorted3 = num3;
            } else {
                sorted2 = num3;
                sorted3 = num2;
            }
        } else {
            sorted1 = num3;
            sorted2 = num1;
            sorted3 = num2;
        }
    }

    printResults(sorted1, sorted2, sorted3);

    return;
}

void printResults(int sorted1, int sorted2, int sorted3) {
    printf("%d\n", sorted1);
    printf("%d\n", sorted2);
    printf("%d\n", sorted3);

    return;
}