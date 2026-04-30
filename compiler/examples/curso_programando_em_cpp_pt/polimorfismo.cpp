#include<iostream>
using namespace std;

class Poligono {
	protected:
		int altura;
		int largura;
	public:
		void setValores(int a, int b) {
			altura = a;
			largura = b;
		}
		virtual int calcArea() {
			return 0;
		}
};

class Retangulo : public Poligono {
	public:
		int calcArea() {
			return largura * altura;
		}
};

class Triangulo : public Poligono {
	public:
		int calcArea() {
			return largura * altura / 2;
		}
};

int main(void) {
	Retangulo retangulo;
	Triangulo triangulo;
	
	retangulo.setValores(4, 3);
	triangulo.setValores(4, 3);
	
	cout << "A area do retangulo e " << retangulo.calcArea() << ".\n";
	cout << "A area do triangulo e " << triangulo.calcArea() << ".\n";
	
	return 0;
}