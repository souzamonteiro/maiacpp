// 07_memory — Simplified: scalar/array new/delete without RAII/classes
#include <stdio.h>

int main() {
    int* p = new int;
    if (p != 0) printf("PASS new_not_null\n");
    *p = 42;
    if (*p == 42) printf("PASS new_id\n");
    delete p;
    
    int* arr = new int[6];
    if (arr != 0) printf("PASS arr_not_null\n");
    for (int i = 0; i < 6; ++i) arr[i] = (i + 1) * (i + 1);
    if (arr[0] == 1 && arr[5] == 36) printf("PASS int_arr\n");
    delete[] arr;
    
    double* d = new double();
    if (d != 0) printf("PASS double_not_null\n");
    *d = 3.14;
    if (*d > 3.0 && *d < 4.0) printf("PASS double_val\n");
    delete d;
    
    printf("ALL PASS\n");
    return 0;
}
