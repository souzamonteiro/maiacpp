#include<iostream>
using namespace std;

class Retangulo {
	int altura;
	int base;
	
	public:
		Retangulo(int a, int b) {
			altura = a;
			base = b;
		}
		~Retangulo() {
		}
		
		void setValores(int a, int b) {
			altura = a;
			base = b;
		}
		
		int calcArea() {
			return base * altura;
		}
};

int main(void) {
	Retangulo retangulo(4, 5);
	
	cout << "A area do retangulo e " << retangulo.calcArea() << ".\n";
	
	return 0;
}