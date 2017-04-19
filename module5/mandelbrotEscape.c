/*
 * Andrew Timkov
 * Roman Ozerski
 * UNSW Tutorials
 * Date: 2/4/17
 * escapeSteps
 * determines how quickly points escape the mandlebrot sequence
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "mandelbrot.h"

#define MAX_STEPS 256
#define MANDEL_POLARITY 2

typedef struct _complexNum {
    double real;
    double imaginary;
} complexNum;

complexNum squareComplex (complexNum z);
complexNum addComplex (complexNum z1, complexNum z2);

int escapeSteps (double x, double y) {
    complexNum c = {x, y};
    complexNum z = {0, 0};
    complexNum zSquared;

    int step = 0;
    int counter = 0;
    double distance = 0;

    while (step < MAX_STEPS) {
        if (distance < MANDEL_POLARITY) {
            zSquared = squareComplex(z);
            z = addComplex(zSquared, c);
            distance = sqrt(pow(z.real, 2) + pow(z.imaginary, 2));
            counter++;
        }
        step++;
    }

    return counter;
}

//(a+b)^2 = a^2 + 2ab + b^2
//i^2 is negative
complexNum squareComplex (complexNum z) {
    complexNum newZ;

    newZ.real = pow(z.real, 2) - pow(z.imaginary, 2);
    newZ.imaginary = 2 * z.real * z.imaginary; //2ab

    return newZ;
}

//add real numbers and add imaginary numbers
complexNum addComplex (complexNum z1, complexNum z2) {
    complexNum newZ;
    newZ.real = z1.real + z2.real;
    newZ.imaginary = z1.imaginary + z2.imaginary;

    return newZ;
}
