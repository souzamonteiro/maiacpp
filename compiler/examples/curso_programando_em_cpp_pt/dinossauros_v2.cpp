#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
using namespace std;

class FormaDeVida {
    protected:
        string nome;
        
    public:
        FormaDeVida(void) {
        }
        FormaDeVida(string n) {
            nome = n;
        }
        void setNome(string n) {
            nome = n;
        }
        string getNome(void) {
            return nome;
        }
};

class Dinossauro : public FormaDeVida {
    public:
        Dinossauro(void) {
            nome = "";
        }
        Dinossauro(string n) {
            nome = n;
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

class Pterodactilo : public Dinossauro {
    public:
        Pterodactilo(string n) : Dinossauro(n) {
        }
};

class Tiranossauro : public Dinossauro {
    public:
        Tiranossauro(string n) : Dinossauro(n) {
        }
};

int main(void) {
    FormaDeVida planta("Filomena");
    Brontossauro dino("Dino");
    Pterodactilo peter("Peter");
    Tiranossauro rex("Rex");
    
    cout << "O nome do dinossauro dino e " << dino.getNome() << ".\n";
    cout << "O nome do dinossauro peter e " << peter.getNome() << ".\n";
    cout << "O nome do dinossauro rex e " << rex.getNome() << ".\n";
    
    dino.coma(planta);
    
    rex.coma(dino);

	return 0;
}