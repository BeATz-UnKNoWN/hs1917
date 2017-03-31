    /*
     * Test if a point is inside a triangle.
     * Julian Saknussemm
     *
     * Given Three points of a triangle, and another arbitrary point this program determines if that point lies inside
     * the triangle.
     *
     * This is determined by satisfying the following rule:
     * A point P(x,y) is inside triangle A(x0,y0), B(x1,y1), C(x2,y2)
     * iff
     * P is on the same side of the line AB as C
     * P is on the same side of the line BC as A
     * and
     * P is on the same side of the line AC as B
     *
     * A special case exits for a vertical line (inf gradient) when testing the side of the line
     */
     
     /*
     * MAIN CHANGES:
     * Fix Indentation
     * Only one return per function
     * Return EXIT_SUCCESS instead of 0
     * Rename functions
     * Rename variables
     * Use compact braces consistently
     * Make spacing in parameters of function consistent
     * Code statements on new line after if condition
     * Add constants
     */
     
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define FALSE 0
#define TRUE 1

#define OVER 1
#define ON 0
#define UNDER -1

int pointsOnSameSideOfLine (double px,
                            double py,
                            double m,
                            double b,
                            double lx,
                            double ly);
int yRelativeToLine (double px,
                     double py,
                     double m,
                     double b);
int isPointInTriangle (double x0,
                       double y0,
                       double x1,
                       double y1,
                       double x2,
                       double y2,
                       double px,
                       double py);
void runTests(void);
    
int main (int argc, char* argv[]) {
    double x0, y0, x1, y1, x2, y2;
    double px, py;
    int returnValue;
     
    int totalScanfReturnValue = 0;

    runTests();
     
    // get input
    printf("Triangle Vertex A (x,y): ");
    totalScanfReturnValue += scanf("%lf,%lf", &x0, &y0);
    printf("Triangle Vertex  B (x,y): ");
    totalScanfReturnValue += scanf("%lf,%lf", &x1, &y1);
    printf("Triangle Vertex  C (x,y): ");
    totalScanfReturnValue += scanf("%lf,%lf", &x2, &y2);
    printf("Test Point (x,y): ");
    totalScanfReturnValue += scanf("%lf,%lf", &px, &py);
     
    // print error or execute code
    if (totalScanfReturnValue != 8 ) {
        printf("\nIncorrect inputs: please try again.\n");
        returnValue = EXIT_FAILURE;
    } else {
        printf("Point (%.2lf,%.2lf) is ", px, py);
        if (isPointInTriangle(x0, y0, x1, y1, x2, y2, px, py)) {
            printf("inside");
        } else {
            printf("outside");
        }
        printf(" the Triangle\n");
        returnValue = EXIT_SUCCESS;
    }
    
    return returnValue;
}

//sub y into equation to get relative position to the line
int yRelativeToLine (double px, double py, double m, double b) {
    int returnValue;
    
    if (py < m * px + b) {
        returnValue = UNDER;
    }
    else if (py == m * px + b) {
        returnValue = ON;
    } else {
        returnValue = OVER;
    }
    
    return returnValue;
}
    
// two points lie on the same side of a line
//pointsOnSameSideOfLine
int pointsOnSameSideOfLine (double px, double py, double m,
                            double b,  double lx, double ly) {
    return (yRelativeToLine(px, py, m, b) ==
            yRelativeToLine(lx, ly, m, b));
}

//isPointInTriangle
int isPointInTriangle (double x0, double y0, double x1, double y1,
                       double x2, double y2, double px, double py) {
     
    int line1, line2, line3;
    // line eqns
    
    double m01 = (y1 - y0) / (x1 - x0);
    // b: y - y1 = m( x - x1 ), x = 0
    // hence b: y = m*-x1 + y1
    // b is y intercept
    double yInt01 = m01 * -x1 + y1;
    double m02, m12, yInt02, yInt12;
    m02 = (y2 - y0) / (x2 - x0);
    m12 = (y2 - y1) / (x2 - x1);
    yInt02 = m02 * -x2 + y2;
    yInt12 = m12 * -x2 + y2;
     
    // vertical line checks
    // the if statements check for infinite gradient
    // ie, infinite rise over zero run
    if (x1 == x0) {
        line1 = ((px <= x0) == (x2 <= x0));
    } else {
        line1 = pointsOnSameSideOfLine(px, py, m01, yInt01, x2, y2);
    }
     
    if (x1 == x2) {
        line2 = ((px <= x2) == (x0 <= x2));
    } else {
        line2 = pointsOnSameSideOfLine(px, py, m12, yInt12, x0, y0);
    }
     
    if (x2 == x0) {
        line3 = ((px <= x0) == (x1 <= x0));
    } else {
        line3 = pointsOnSameSideOfLine(px, py, m02, yInt02, x1, y1);
    }
    
    return line1 && line2 && line3;
}

void runTests (void) {
    //test point is a vertex of the triangle
    assert (isPointInTriangle(0, 0, 1, 0, 0.5, 1, 0, 0) == FALSE);
    
    //test the origin in a triangle
    assert (isPointInTriangle(0, 2, -2, -2, 2, -2, 0, 0) == TRUE);
    
    //test if point is outside triangle
    assert (isPointInTriangle(2, 2, 3, 2, 3, 3, 0, 0) == FALSE);
    
    //test if point is inside triangle
    assert (isPointInTriangle(0, 0, 1, 0, 0.5, 1, 0.5, 0.5) == TRUE);
    
    //a quite random triangle
    assert (isPointInTriangle(50, 99, -1, -1, 1784, 519, 1, 1) == TRUE);
    
    printf("All tests passed: you are awesome!\n");
    return;
}