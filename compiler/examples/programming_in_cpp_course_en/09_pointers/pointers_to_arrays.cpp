#include <iostream>
#include <stdio.h>
using namespace std;

int main(void) {
    int a = 1;
    int b = 255;
    int c[3];
    int d[] = {4, 5, 6};
    int *p;
    int i;

    c[0] = 1;
    c[1] = 2;
    c[2] = 3;

    p = &a;
    cout << "Value pointed to by p: " << *p << ".\n";
    printf("The memory address pointed to by p is %ld.\n", p);
    p = &b;
    cout << "Value pointed to by p: " << *p << ".\n";
    printf("The memory address pointed to by p is %ld.\n", p);

    p = d;
    cout << "Value pointed to by p: " << *p << ".\n";
    printf("The memory address pointed to by p is %ld.\n", p);
    p++;
    cout << "Value pointed to by p: " << *p << ".\n";
    printf("The memory address pointed to by p is %ld.\n", p);
    *p = 7;
    
    for (i = 0; i < (sizeof(d) / sizeof(int)); i++) {
        cout << "d[" << i << "] = " << d[i] << ".\n";
    }

    return 0;
}
