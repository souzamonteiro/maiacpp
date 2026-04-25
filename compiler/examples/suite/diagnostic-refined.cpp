// diagnostic-refined.cpp — Bisect to find exact lowering failure point

#include <stdio.h>

// WORKS: Simple var declare + literal assignment
int t1() {
    int x = 5;
    return 1;
}

// Does this work? Var + literal assignment + return
int t2() {
    int x = 5;
    return x;
}

// Does this work? Two vars + literals, no operations
int t3() {
    int a = 10;
    int b = 20;
    return 1;
}

// Does this work? Var + literal, then return the var
int t4() {
    int a = 10;
    int b = 20;
    return a;
}

// Does binary op in expression fail?
int t5() {
    int result = 10 + 20;
    return 1;
}

// Does binary op in variable initialization fail?
int t6() {
    int a = 10;
    int b = a + 5;
    return 1;
}

// Does comparison in expression fail?
int t7() {
    int a = 10;
    int b = (a > 5) ? 1 : 0;
    return 1;
}

// Does if-condition (comparison) fail?
int t8() {
    int a = 10;
    if (a > 5) {
        return 1;
    }
    return 0;
}

// Does printf call fail?
int t9() {
    printf("test\n");
    return 1;
}

// Does printf with arg fail?
int t10() {
    int x = 42;
    printf("x=%d\n", x);
    return 1;
}

// Does if + printf combo fail?
int t11() {
    int a = 10;
    if (a > 5) printf("ok\n");
    return 1;
}

int main() {
    return 0;
}
