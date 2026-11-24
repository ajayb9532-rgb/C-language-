#include <stdio.h>

unsigned long long factorial(int n) {
    if (n <= 1) return 1;
    return (unsigned long long)n * factorial(n - 1);
}

int fibonacci(int n) {
    if (n <= 1) return n;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

int gcd(int a, int b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

int main(void) {
    printf("Factorial(6) = %llu\n", factorial(6));
    printf("Fibonacci(8) = %d\n", fibonacci(8));
    printf("GCD(48, 18)  = %d\n", gcd(48, 18));
    return 0;
}
