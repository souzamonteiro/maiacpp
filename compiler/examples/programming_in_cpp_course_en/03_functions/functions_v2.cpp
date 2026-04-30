#include <iostream>
using namespace std;

void display_message(char text[]) {
        cout << text << endl;
}

int square(int x) {
        return x * x;
}

int main(void) {
        char msg[] = "Hello World!";

        display_message(msg);

        cout << "The square of 5 is " << square(5) << ".\n";

        return 0;
}
