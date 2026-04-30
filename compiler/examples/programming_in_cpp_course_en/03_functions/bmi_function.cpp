#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
using namespace std;

float calcBMI(float weight, float height) {
    return weight / (height * height);
}

int main(void) {
    float weight;
    float height;
    
    cout << "Enter your weight in kg: ";
    cin >> weight;
    cout << "Enter your height in m: ";
    cin >> height;
    
    
    cout << "BMI: " << calcBMI(weight, height) << ".\n";
    
        return 0;
}
