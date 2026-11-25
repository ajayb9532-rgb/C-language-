#include "calc.h"

double add(double a, double b) { return a + b; }
double subtract(double a, double b) { return a - b; }
double multiply(double a, double b) { return a * b; }

int divide(double a, double b, double *result) {
    if (b == 0.0) return 0;
    *result = a / b;
    return 1;
}
