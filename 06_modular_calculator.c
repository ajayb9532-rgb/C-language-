#include <stdio.h>
#include "calc.h"

int main(void) {
    double x = 24.0, y = 6.0, quotient = 0.0;

    printf("Add:      %.2f + %.2f = %.2f\n", x, y, add(x, y));
    printf("Subtract: %.2f - %.2f = %.2f\n", x, y, subtract(x, y));
    printf("Multiply: %.2f * %.2f = %.2f\n", x, y, multiply(x, y));

    if (divide(x, y, &quotient)) {
        printf("Divide:   %.2f / %.2f = %.2f\n", x, y, quotient);
    } else {
        printf("Divide:   Error (division by zero)\n");
    }

    return 0;
}
