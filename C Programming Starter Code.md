#include <stdio.h>

// Function declarations
void printBanner(void);
int add(int a, int b);
int factorial(int n);

int main(void) {
    printBanner();

    int num1 = 12, num2 = 8;
    printf("Sum of %d and %d: %d\n", num1, num2, add(num1, num2));

    int factNum = 5;
    printf("Factorial of %d: %d\n", factNum, factorial(factNum));

    // Array and loop example
    int numbers[] = {10, 20, 30, 40, 50};
    int length = sizeof(numbers) / sizeof(numbers[0]);
    int total = 0;

    printf("Array elements: ");
    for (int i = 0; i < length; i++) {
        printf("%d ", numbers[i]);
        total += numbers[i];
    }
    printf("\nArray sum: %d\n", total);

    return 0;
}

void printBanner(void) {
    printf("===================================\n");
    printf("   C Programming Starter Template  \n");
    printf("===================================\n\n");
}

int add(int a, int b) {
    return a + b;
}

int factorial(int n) {
    if (n <= 1) {
        return 1;
    }
    return n * factorial(n - 1);
}