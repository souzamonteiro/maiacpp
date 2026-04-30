#include <iostream>
using namespace std;

int main (void) {
        int a = 1;
        int b = 2;
        int c;

        c = a + b;                        // Addition.
        cout << "a + b = " << c << endl;

        c = a - b;                        // Subtraction.
        cout << "a - b = " << c << endl;

        c = a * b;                        // Multiplication.
        cout << "a * b = " << c << endl;

        c = a / b;                        // Division.
        cout << "a / b = " << c << endl;

        c = a % b;                        // Remainder (modulo).
        cout << "a % b = " << c << endl;

        c = a < b;                        // Less than.
        cout << "a < b = " << c << endl;

        c = a <= b;                       // Less than or equal.
        cout << "a <= b = " << c << endl;

        c = a > b;                        // Greater than.
        cout << "a > b = " << c << endl;

        c = a >= b;                       // Greater than or equal.
        cout << "a >= b = " << c << endl;

        c = a == b;                       // Equal to.
        cout << "a == b = " << c << endl;

        c = a != b;                       // Not equal to.
        cout << "a != b = " << c << endl;

        b = 0;

        c = a && b;                       // Logical AND.
        cout << "a && b = " << c << endl;

        c = a || b;                       // Logical OR.
        cout << "a || b = " << c << endl;

        a++;                              // Increment (+1).
        cout << "a = " << a << endl;

        b--;                              // Decrement (-1).
        cout << "b = " << b << endl;

        return 0;
}
