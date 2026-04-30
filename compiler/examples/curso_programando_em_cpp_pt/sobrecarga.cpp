#include<iostream>
using namespace std;

int quadrado(int x) {
    return x * x;
}

float quadrado(float x) {
    return x * x;
}

int main(void) {
    int x1;
    float x2;

    cout << "Digite um número inteiro: ";
    cin >> x1;

    cout << "Digite um número real: ";
    cin >> x2;

    cout << "O quadrado de " << x1 << " é: " << quadrado(x1) << "\n";
    cout << "O quadrado de " << x2 << " é: " << quadrado(x2) << "\n";
    
    return 0;
}