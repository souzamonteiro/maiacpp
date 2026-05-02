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
static double square(double x) { return x * x; }

static void swap_ref(int& a, int& b) {
    int tmp = a; a = b; b = tmp;
}

static int sum_cref(const int& x, const int& y) {
    return x + y;
}

static int clamp(int x, int lo, int hi) {
    if (x < lo) return lo;
    if (x > hi) return hi;
    return x;
}

typedef int (*IntOp)(int);
static int double_val(int x) { return x * 2; }
static int negate(int x) { return -x; }
static int apply(IntOp f, int x) { return f(x); }

int main() {
    if (factorial(0) == 1) printf("PASS fact_0\n");
    if (factorial(1) == 1) printf("PASS fact_1\n");
    if (factorial(5) == 120) printf("PASS fact_5\n");
    if (factorial(7) == 5040) printf("PASS fact_7\n");
    if (fib(0) == 0) printf("PASS fib_0\n");
    if (fib(1) == 1) printf("PASS fib_1\n");
    if (fib(7) == 13) printf("PASS fib_7\n");
    if (fib(10) == 55) printf("PASS fib_10\n");
    if (square(7) == 49) printf("PASS sq_int\n");
    if (square(5.0) == 25.0) printf("PASS sq_double\n");
    int p = 3, q = 8;
    swap_ref(p, q);
    if (p == 8 && q == 3) printf("PASS swap_ref\n");
    if (sum_cref(10, 10) == 20) printf("PASS sum_cref_10\n");
    if (sum_cref(50, 50) == 100) printf("PASS sum_cref_100\n");
    if (clamp(50, 0, 100) == 50) printf("PASS clamp_mid\n");
    if (clamp(-5, 0, 100) == 0) printf("PASS clamp_lo\n");
    if (clamp(150, 0, 100) == 100) printf("PASS clamp_hi\n");
    if (apply(double_val, 7) == 14) printf("PASS fptr_double\n");
    if (apply(negate, 5) == -5) printf("PASS fptr_negate\n");
    if (apply(square, 8) == 64) printf("PASS fptr_square\n");
    IntOp arr[3] = { double_val, negate, square };
    if (arr[0](3) == 6) printf("PASS fptr_arr_0\n");
    if (arr[1](7) == -7) printf("PASS fptr_arr_1\n");
    if (arr[2](9) == 81) printf("PASS fptr_arr_2\n");
    printf("ALL PASS\n");
    return 0;
}
