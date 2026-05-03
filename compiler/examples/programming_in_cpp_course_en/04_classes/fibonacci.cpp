#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
using namespace std;

int fib(int n) {
    if (n <= 1) {
        return n;
    }
    return fib(n - 1) + fib(n - 2);
}

class Fibonacci {
    public:
        Fibonacci() {
        }
        int nFibonacci(int n) {
            return fib(n);
        }
        void createSeries(int n) {
            int i;
            for (i = 1; i <= n; i++) {
                printf(" %d", fib(i));
            }
            printf("\n");
        }
};

int main(void) {
    Fibonacci fib_obj;
    int n;

    cout << "How many terms would you like to display? ";
    cin >> n;
    cout << endl;

    fib_obj.createSeries(n);

        return 0;
}
