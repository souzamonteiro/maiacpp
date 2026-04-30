#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
using namespace std;

float celsius(float f) {
    return (f - 32) / 1.8;
}

int main(void) {
    float f;
    
    cout << "Enter the temperature in Fahrenheit: ";
    cin >> f;
    
    printf("Temperature in Celsius: %.2f.\n", celsius(f));
    
        return 0;
}
