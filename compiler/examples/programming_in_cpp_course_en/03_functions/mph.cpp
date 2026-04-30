#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
using namespace std;

int main(void) {
    float mph;
    float kmh;
    
    cout << "Enter the speed in km/h: ";
    cin >> kmh;
    
    mph = kmh / 1.61;
    
    cout << "Speed in mph: " << mph << ".\n";
    
        return 0;
}
