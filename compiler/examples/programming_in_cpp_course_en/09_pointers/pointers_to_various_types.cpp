#include <iostream>
using namespace std;

int main(void) {
        int a;
        float b;
        char c;
        char d[] = "Hello World!";

        int *pi;
        float *pf;
        char *pc;

        a = 1;
        b = 2;
        c = 'x';

        pi = &a;
        pf = &b;
        pc = &c;

        printf("Address pointed to by pi: %ld, value at the address pointed to by pi: %d\n", pi, *pi);
        printf("Address pointed to by pf: %ld, value at the address pointed to by pf: %f\n", pf, *pf);
        printf("Address pointed to by pc: %ld, value at the address pointed to by pc: %c\n", pc, *pc);

        pc = d;
        printf("Address pointed to by pc: %ld, value at the address pointed to by pc: %c\n", pc, *pc);
        pc++;
        printf("Address pointed to by pc: %ld, value at the address pointed to by pc: %c\n", pc, *pc);
        (*pc)++;
        printf("Address pointed to by pc: %ld, value at the address pointed to by pc: %c\n", pc, *pc);

        cout << d << endl;

        return 0;
}
