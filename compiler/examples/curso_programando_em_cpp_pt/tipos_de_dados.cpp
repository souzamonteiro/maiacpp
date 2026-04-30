#include <iostream>
using namespace std;

int main (void) {
	int x;                   // Numero inteiro entre -2147483648 e 2147483647 (se a maquina for de 64 bits).
	float y;                 // Numero com precisao simples entre 1.2E-38 e 3.4E+38.
	double z;                // Numero com precisao dupla entre 2.3E-308 e 1.7E+308.
	char a;                  // Um unico caractere ASCII.
	char b[] = "Alo Mundo";  // Cadeia de carecteres.
	
	x = 1;
	y = 2;
	z = 3;
	
	a = 'A';
	
	cout << x << endl;  // Utilizamos endl para pular uma linha na tela.
	cout << y << endl;
	cout << z << endl;
	cout << a << endl;
	cout << b << endl;
	
	return 0;
}