#include <iostream>
using namespace std;

int main (void) {
        int x;                   // Integer between -2147483648 and 2147483647 (on a 64-bit machine).
        float y;                 // Single-precision floating point between 1.2E-38 and 3.4E+38.
        double z;                // Double-precision floating point between 2.3E-308 and 1.7E+308.
        char a;                  // A single ASCII character.
        char b[] = "Hello World";  // Character string.

        x = 1;
        y = 2;
        z = 3;

        a = 'A';

        cout << x << endl;  // We use endl to move to a new line on screen.
        cout << y << endl;
        cout << z << endl;
        cout << a << endl;
        cout << b << endl;

        return 0;
}
