#include <iostream>
using namespace std;

void alomundo(void) {
    cout << "Alô Mundo!\n";
}

void exibeMensagem(char msg[]) {
    cout << msg << "\n";
}

float quadrado(float x) {
    return x * x;
}

float potencia(float x, int y) {
    int i;
    float p;

    p = 1;
    for (i = 0; i < y; i++) {
        p = p * x;
    }

    return p;
}

int main(void) {
    alomundo();

    exibeMensagem("Oi SENAI!");

    cout << "O quadrado de 5 é " << quadrado(5) << ".\n";

    cout << "O cubo de 2 é " << potencia(2, 3) << ".\n";

    return 0;
}