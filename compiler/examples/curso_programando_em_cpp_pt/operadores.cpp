#include <iostream>
using namespace std;

int main (void) {
	int a = 1;
	int b = 2;
	int c;

	c = a + b;                        // Adicao.
	cout << "a + b = " << c << endl;
	
	c = a - b;                        // Subtracao.
	cout << "a - b = " << c << endl;
	
	c = a * b;                        // Multiplicacao.
	cout << "a * b = " << c << endl;
	
	c = a / b;                        // Divisao.
	cout << "a / b = " << c << endl;
	
	c = a % b;                        // Resto.
	cout << "a % b = " << c << endl;
	
	c = a < b;                        // Menor que.
	cout << "a < b = " << c << endl;
	
	c = a <= b;                        // Menor ou igual que.
	cout << "a <= b = " << c << endl;
	
	c = a > b;                        // Maior que.
	cout << "a > b = " << c << endl;
	
	c = a >= b;                        // Maior ou igual que.
	cout << "a >= b = " << c << endl;
	
	c = a == b;                        // Igual a.
	cout << "a == b = " << c << endl;
	
	c = a != b;                        // Diferente de.
	cout << "a != b = " << c << endl;
	
	b = 0;
	
	c = a && b;                        // Operacao logica E.
	cout << "a && b = " << c << endl;
	
	c = a || b;                        // Operacao logica OU.
	cout << "a || b = " << c << endl;
	
	a++;                               // Incremento (+1).
	cout << "a = " << a << endl;
	
	b--;                               // Decremento (-1).
	cout << "b = " << b << endl;
	
	return 0;
}