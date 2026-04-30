#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
using namespace std;

class Fibonacci {
    public:
        Fibonacci() {
        }
        //         1 2 3 4 5 6  7  8  9 10
        // F(10) = 1 1 2 3 5 8 13 21 34 55
        // F(n) = F(n - 1) + F(n - 2)
        // F(1) = F(2) = 1
        int nFibonacci(int n) {
            if (n <= 1) {
                return(n);
            }
            return nFibonacci(n - 1) + nFibonacci(n - 2);
        }
        string criarSerie(int n) {
            int i;
            int anterior;
            int atual;

            string serie = "";
            for (i = 1; i <= n; i++) {
                serie.append(" ");
                serie.append(std::to_string(nFibonacci(i)));
            }
            return serie;
        }
};

int main(void) {
    Fibonacci fibonacci;
    int n;

    cout << "Quantos termos você deseja exibir? ";
    cin >> n;
    cout << endl;

    cout << fibonacci.criarSerie(n) << endl;

	return 0;
}