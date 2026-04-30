#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
using namespace std;

float calcIMC(float p, float h) {
    return p / (h * h);
}

int main(void) {
    float p;
    float h;
    
    cout << "Informe seu peso em kg: ";
    cin >> p;
    cout << "Informe sua altura em m: ";
    cin >> h;
    
    
    cout << "IMC: " << calcIMC(p, h) << ".\n";
    
	return 0;
}