#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
using namespace std;

int main(void) {
    float mph;
    float kmh;
    
    cout << "Informe a velociade em Km/h: ";
    cin >> kmh;
    
    mph = kmh / 1.61;
    
    cout << "Velocidad em mph: " << mph << ".\n";
    
	return 0;
}