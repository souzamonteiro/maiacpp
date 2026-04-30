#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
using namespace std;

class Australopithecus {
    protected:
        string nome;
        
    public:
        Australopithecus() {
        }
        Australopithecus(string n) {
            nome = n;
        }
        void setNome(string n) {
            nome = n;
        }
        string getNome(void) {
            return nome;
        }
};

class HomoHabilis : public Australopithecus {
    public:
        HomoHabilis() : Australopithecus() {
        }
        HomoHabilis(string n) : Australopithecus(n) {
        }
        template <class T>
        void lute(T quem) {
            cout << getNome() << " lutou com " << quem.getNome() <<".\n";
        }
};

class HomoErectus : public HomoHabilis {
    public:
        HomoErectus() : HomoHabilis() {
        }
        HomoErectus(string n) : HomoHabilis(n) {
        }
};

class HomoSapiens : public HomoErectus {
    public:
        HomoSapiens() : HomoErectus() {
        }
        HomoSapiens(string n) : HomoErectus(n) {
        }
        void fale(string s) {
            cout << getNome() << " disse \"" << s << "\".\n";
        }
        void fale(string s, HomoErectus quem) {
            cout << getNome() << " disse \"" << s << "\" a " << quem.getNome() <<".\n";
        }
};

class HomoNeanderthalensis : public HomoErectus {
    public:
        HomoNeanderthalensis() : HomoErectus() {
        }
        HomoNeanderthalensis(string n) : HomoErectus(n) {
        }
};

int main(void) {
    HomoNeanderthalensis tadeu("Tadeu");
    HomoSapiens amadeu("Amadeu");
    
    amadeu.fale("Que dia lindo!");
    amadeu.fale("Quem é você?", tadeu);
    tadeu.lute(amadeu);
    
	return 0;
}
