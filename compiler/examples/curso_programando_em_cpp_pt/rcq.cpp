#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
using namespace std;

float calcRCQ(float c, float q) {
    return c / q;
}

int main(void) {
    float c;
    float q;
    
    cout << "Informe a circunferência de sua cintura em cm: ";
    cin >> c;
    cout << "Informe a circunferência de seu quadril em cm: ";
    cin >> q;
    
    
    cout << "RCQ: " << calcRCQ(c, q) << ".\n";
    
	return 0;
}