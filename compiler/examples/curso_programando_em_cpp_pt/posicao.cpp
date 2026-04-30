#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
using namespace std;

int main(void) {
    float s;
    float s0;
    float v;
    float t;
    
    cout << "Informe a posição inicial do móvel em m: ";
    cin >> s0;
    cout << "Informe a velocidade do móvel em m/s: ";
    cin >> v;
    cout << "Informe o tempo de deslocamento do móvel em s: ";
    cin >> t;
    
    s = s0 + v * t;
    
    printf("Posição atual do móvel: %.2f m.\n", s);
    
	return 0;
}