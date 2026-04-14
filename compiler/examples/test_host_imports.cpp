// Example: C++ host-import declarations compiled through MaiaCpp → MaiaC → WASM
//
// Functions named __namespace__funcName are treated as WAT imports by MaiaC.
// The generated JS wrapper (webc.sh) automatically builds the host environment.

void __console__log(const char *msg);
void __console__error(const char *msg);
double __Math__sqrt(double x);
double __Math__floor(double x);
double __Math__pow(double base, double exp);

#include <stdio.h>

int main() {
    __console__log("Hello from MaiaCpp!");
    __console__error("This is an error message");

    double root = __Math__sqrt(144.0);
    printf("sqrt(144) = %.0f\n", root);

    double flr = __Math__floor(3.7);
    printf("floor(3.7) = %.0f\n", flr);

    double pw = __Math__pow(2.0, 10.0);
    printf("2^10 = %.0f\n", pw);

    return 0;
}
