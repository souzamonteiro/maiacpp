#include<iostream>
using namespace std;

class Retangulo {
	// Membros podem ter acesso public, protected ou private.
	// Membros com acesso public sao acessiveis a partir de
	// qualquer classe;
	// Membros com acesso protected sao acessiveis a partir da
	// propria classe ou classe derivada;
	// Membros com acesso private sao acessiveis apenas a
	// partir da propria classe.
	// O tipo de acesso padrao e private.
	int altura;
	int base;
	
	public:
		void setValores(int a, int b) {
			altura = a;
			base = b;
		}
		int calcArea() {
			return base * altura;
		}
};

int main(void) {
	Retangulo retangulo;
	
	retangulo.setValores(4, 5);
	
	cout << "A area do retangulo e " << retangulo.calcArea() << ".\n";
	
	return 0;
}