#include<iostream>
using namespace std;

class FormaDeVida {
	public:
		FormaDeVida(void) {
		}
};

class Dinossauro : public FormaDeVida {
	protected:
		string nome;
		
	public:
		Dinossauro(void) {
			nome = "";
		}
		Dinossauro(string n) {
			nome = n;
		}
		void setNome(string n){
			nome = n;
		}
		string getNome(void) {
			return nome;
		}
		template <class T>
		void coma(T algo) {
			cout << nome << " comeu " << algo.getNome() << ".\n";
		}
};

class Brontossauro : public Dinossauro {
	public:
		Brontossauro(string n) : Dinossauro(n) {
		}
};

class Piterodactilo : public Dinossauro {
	public:
		Piterodactilo(string n) : Dinossauro(n) {
		}
};

class Tiranossauro : public Dinossauro {
	public:
		Tiranossauro(string n) : Dinossauro(n) {
		}
};

int main(void) {
	Brontossauro dino("Dino");
	Piterodactilo piter("Piter");
	Tiranossauro rex("Rex");
	
	cout << "O nome do dinossauro dino e " << dino.getNome() << ".\n";
	cout << "O nome do dinossauro piter e " << piter.getNome() << ".\n";
	cout << "O nome do dinossauro rex e " << rex.getNome() << ".\n";
	
	rex.coma<Dinossauro>(dino);
	
	return 0;
}