/*
 * Andrew Timkov
 * Roman Ozerski
 * UNSW Tutorials
 * Date: 18/4/17
 * Color Map
 */

#include <math.h>

#define MAX_STEPS 256

#define MIN_NUM 0
#define MAX_NUM 255
#define HALF 128

unsigned char stepsToRed (int steps) {
    int output = MAX_NUM * cos(steps * 3) - 100;
    if (steps < HALF) {
        output = MAX_NUM * sin(steps * 3) + 100;
    }
    return output * output * output;
} 

unsigned char stepsToBlue (int steps) {
    double root2;
    root2 = sqrt(2);
    int output = steps * 5;
    output *= 355;
    output *= root2;
    return output * output * output;
}

unsigned char stepsToGreen (int steps) {
    int output = 310;
    if (steps % 50 == 0) {
        output = 0;
    }
    return output * output * output;
}
