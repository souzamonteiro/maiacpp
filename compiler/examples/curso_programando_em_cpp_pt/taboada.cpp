#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
using namespace std;

class Taboada {
    public:
        Taboada() {
        }
        void criarTaboada(int n) {
            int i;
            for (i = 1; i <= 10; i++) {
                cout << n << " x " << i << " = " << n * i << endl;
            }
        }
};

int main(void) {
    Taboada taboada;
    int n;

    cout << "Qual a taboada que você deseja exibir? ";
    cin >> n;
    cout << endl;

    taboada.criarTaboada(n);

	return 0;
}