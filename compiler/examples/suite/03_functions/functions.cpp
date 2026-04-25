// 03_functions — Exercises C++98 function features:
//   recursion (factorial, Fibonacci), function overloading,
//   pass-by-reference, function pointers
#include <stdio.h>

static int factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

static int fib(int n) {
    if (n <= 1) return n;
    return fib(n - 1) + fib(n - 2);
}

static int square(int x) { return x * x; }
static void swap_ref(int& a, int& b) {
    int tmp = a; a = b; b = tmp;
}

typedef int (*IntOp)(int);
static int double_val(int x) { return x * 2; }
static int apply(IntOp f, int x) { return f(x); }

int main() {
    if (factorial(5) == 120) printf("PASS fact_5\n");
    if (factorial(7) == 5040) printf("PASS fact_7\n");
    if (fib(7) == 13) printf("PASS fib_7\n");
    if (fib(10) == 55) printf("PASS fib_10\n");
    if (square(7) == 49) printf("PASS sq_int\n");
    int p = 3, q = 8;
    swap_ref(p, q);
    if (p == 8 && q == 3) printf("PASS swap_ref\n");
    if (apply(double_val, 7) == 14) printf("PASS fptr_double\n");
    printf("ALL PASS\n");
    return 0;
}
