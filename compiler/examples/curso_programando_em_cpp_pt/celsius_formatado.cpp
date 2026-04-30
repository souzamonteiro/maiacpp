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
    
    cout << "Informe a temperatura em Fahrenheit: ";
    cin >> f;
    
    printf("Temperatura em Celsius: %.2f.\n", celsius(f));
    
	return 0;
}