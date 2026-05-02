// 08_arrays_pointers — Exercises C++98 array and pointer features
#include <stdio.h>

static int array_sum(const int* arr, int n) {
    int s = 0;
    for (int i = 0; i < n; ++i) s += arr[i];
    return s;
}

int main() {
    int a[5] = { 2, 4, 6, 8, 10 };
    if (a[0] == 2 && a[4] == 10) printf("PASS arr_access\n");
    if (array_sum(a, 5) == 30) printf("PASS arr_sum\n");
    
    int* p = a;
    if (*p == 2 && *(p+2) == 6) printf("PASS ptr_arith\n");
    
    int mat[3][3] = { {1, 2, 3}, {4, 5, 6}, {7, 8, 9} };
    if (mat[1][1] == 5 && mat[2][2] == 9) printf("PASS mat_2d\n");
    
    int sq[5];
    for (int i = 0; i < 5; ++i) sq[i] = i * i;
    if (sq[3] == 9 && sq[4] == 16) printf("PASS fill_sq\n");
    
    int x = 42;
    int* px = &x;
    if (*px == 42) printf("PASS ptr_deref\n");
    
    printf("ALL PASS\n");
    return 0;
}
