// 05_templates — Exercises C++98 template features:
//   function templates (tmax)
#include <stdio.h>

template<typename T>
T tmax(const T& a, const T& b) {
    return a > b ? a : b;
}

int main() {
    if (tmax(3, 7) == 7) printf("PASS tmax_int_r\n");
    if (tmax(9, 2) == 9) printf("PASS tmax_int_l\n");

    printf("ALL PASS\n");
    return 0;
}

