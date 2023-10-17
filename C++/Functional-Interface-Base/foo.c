/* foo.c: a source file that implements the foo module */
#include "foo.h" /* include the header file of the foo module */
#include <math.h> /* include a standard library header file */

/* a static variable that is only visible within this file */
static int count = 0;

/* a static function that is only visible within this file */
static void increment(void)
{
    count++;
}

/* a function definition that implements a function */
double distance(struct point p1, struct point p2)
{
    increment(); /* call the static function */
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2)); /* return the distance between two points */
}

/* main.c: a source file that uses the foo module */
#include "foo.h" /* include the header file of the foo module */
#include <stdio.h> /* include a standard library header file */

int main()
{
    struct point p1 = {0, 0}; /* create a point variable and initialize it */
    struct point p2 = {3, 4}; /* create another point variable and initialize it */
    double d = distance(p1, p2); /* call the function from the foo module and store the result */
    printf("%f\n", d); /* print the result */
    return 0;
}