/* foo.h: a header file that defines the interface of the foo module */
#ifndef FOO_H /* a macro that prevents multiple inclusion */
#define FOO_H

/* a macro that defines a constant */
#define PI 3.14

/* a struct type that defines a data structure */
struct point {
    int x;
    int y;
};

/* a function prototype that declares a function */
double distance(struct point p1, struct point p2);

#endif