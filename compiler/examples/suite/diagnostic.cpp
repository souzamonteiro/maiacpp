// diagnostic.cpp — Identify exact lowering failures in MaiaCpp
// Test each C++98 construct in isolation to find stub-fallback triggers

#include <stdio.h>

// Test 1: Simple printf call in main
int test_simple_printf() {
    printf("test1\n");
    return 1;
}

// Test 2: Variable declaration + assignment
int test_var_assign() {
    int x = 5;
    return 1;
}

// Test 3: Simple arithmetic
int test_simple_arith() {
    int a = 10, b = 20;
    int c = a + b;
    return 1;
}

// Test 4: If statement
int test_if() {
    int x = 5;
    if (x > 0) {
        return 1;
    }
    return 0;
}

// Test 5: If with comparison in condition
int test_if_cond() {
    int a = 10, b = 5;
    if (a == b) {
        return 0;
    }
    return 1;
}

// Test 6: Printf with variable
int test_printf_var() {
    int x = 42;
    printf("Value: %d\n", x);
    return 1;
}

// Test 7: Printf with expression
int test_printf_expr() {
    int a = 10, b = 5;
    printf("Sum: %d\n", a + b);
    return 1;
}

// Test 8: If-printf combo (like in original tests)
int test_if_printf() {
    int a = 10, b = 5;
    if (a > b) printf("PASS\n");
    return 1;
}

// Test 9: Multiple if-printf (like in original)
int test_multi_if_printf() {
    int a = 10, b = 5;
    if (a > b)  printf("test1\n");
    if (a < 20) printf("test2\n");
    if (b == 5) printf("test3\n");
    return 1;
}

// Test 10: Compound statement block
int test_compound() {
    int x = 0;
    x += 5;
    x *= 2;
    return x;
}

int main() {
    printf("DIAGNOSTIC START\n");
    
    // Call each test to see which ones stub
    test_simple_printf();
    test_var_assign();
    test_simple_arith();
    test_if();
    test_if_cond();
    test_printf_var();
    test_printf_expr();
    test_if_printf();
    test_multi_if_printf();
    test_compound();
    
    printf("DIAGNOSTIC END\n");
    return 0;
}
