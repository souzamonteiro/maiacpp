#include<iostream>
using namespace std;

int square(int x) {
    return x * x;
}

float square(float x) {
    return x * x;
}

int main(void) {
    int x1;
    float x2;

    cout << "Enter an integer: ";
    cin >> x1;

    cout << "Enter a real number: ";
    cin >> x2;

    cout << "The square of " << x1 << " is: " << square(x1) << "\n";
    cout << "The square of " << x2 << " is: " << square(x2) << "\n";
    
    return 0;
}
