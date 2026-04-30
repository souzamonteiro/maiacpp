#include<iostream>
using namespace std;

float FahrenheitToCelsius(float t) {
        return (t - 32.0) * 5.0 / 9.0;
}
int FahrenheitToCelsius(int t) {
        return (t - 32.0) * 5.0 / 9.0;
}

int main(void) {
        int t1;
        float t2;

        cout << "Enter the temperature in Fahrenheit as an integer: ";
        cin >> t1;
        cout << "The temperature in Celsius is " << FahrenheitToCelsius(t1) << ".\n";

        cout << "Enter the temperature in Fahrenheit as a real number: ";
        cin >> t2;
        cout << "The temperature in Celsius is " << FahrenheitToCelsius(t2) << ".\n";

        return 0;
}
