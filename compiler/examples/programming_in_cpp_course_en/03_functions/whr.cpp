#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
using namespace std;

float calcWHR(float waist, float hip) {
    return waist / hip;
}

int main(void) {
    float waist;
    float hip;
    
    cout << "Enter your waist circumference in cm: ";
    cin >> waist;
    cout << "Enter your hip circumference in cm: ";
    cin >> hip;
    
    
    cout << "WHR: " << calcWHR(waist, hip) << ".\n";
    
        return 0;
}
