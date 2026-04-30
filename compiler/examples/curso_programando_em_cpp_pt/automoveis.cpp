#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
using namespace std;

class Automovel {
    public:
        string cor;
        int ano;
        string modelo;
        float cilindrada;
        float preco;
        
        Automovel() {
        }
        Automovel(string cor, int ano, string modelo, float cilindrada, float preco) {
            this->cor = cor;
            this->ano = ano;
            this->modelo = modelo;
            this->cilindrada = cilindrada;
            this->preco = preco;
        }
};

class Carro : public Automovel {
    public:
        Carro() : Automovel() {
        }
        Carro(string cor, int ano, string modelo, float cilindrada, float preco) : Automovel(cor, ano, modelo, cilindrada, preco) {
        }
};

class Caminhao : public Automovel {
    public:
        Caminhao() : Automovel() {
        }
        Caminhao(string cor, int ano, string modelo, float cilindrada, float preco) : Automovel(cor, ano, modelo, cilindrada, preco) {
        }
};

class Trator : public Automovel {
    public:
        Trator() : Automovel() {
        }
        Trator(string cor, int ano, string modelo, float cilindrada, float preco) : Automovel(cor, ano, modelo, cilindrada, preco) {
        }
};

int main(void) {
    Carro etios("Prata", 2021, "XL", 1.4, 50000);
    Caminhao actros("Vermelho", 2022, "X", 6.0, 500000);
    Trator mf3400("Azul", 2022, "MF 3400", 3.0, 75000);
    
    cout << "O trator " << mf3400.modelo << " ano " << mf3400.ano << " custa R$" << mf3400.preco << ".\n";
    
	return 0;
}
