#include <iostream>
using namespace std;

void hello_world(void) {
    cout << "Hello World!\n";
}

void display_message(const char msg[]) {
    cout << msg << "\n";
}

float square(float x) {
    return x * x;
}

float power(float x, int y) {
    int i;
    float p;

    p = 1;
    for (i = 0; i < y; i++) {
        p = p * x;
    }

    return p;
}

int main(void) {
    hello_world();

    display_message("Hi there!");

    cout << "The square of 5 is " << square(5) << ".\n";

    cout << "The cube of 2 is " << power(2, 3) << ".\n";

    return 0;
}
