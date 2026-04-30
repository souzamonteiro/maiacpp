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
        float ar;
        float direcao;
        
        Automovel() {
        }
        Automovel(string cor, int ano, string modelo, float cilindrada, float preco, float ar, float direcao) {
            this->cor = cor;
            this->ano = ano;
            this->modelo = modelo;
            this->cilindrada = cilindrada;
            this->preco = preco;
            this->ar = ar;
            this->direcao = direcao;
        }
};

class Carro : public Automovel {
    public:
        Carro() : Automovel() {
        }
        Carro(string cor, int ano, string modelo, float cilindrada, float preco, float ar, float direcao) : Automovel(cor, ano, modelo, cilindrada, preco, ar, direcao) {
        }
};

class Caminhao : public Automovel {
    public:
        Caminhao() : Automovel() {
        }
        Caminhao(string cor, int ano, string modelo, float cilindrada, float preco, float ar, float direcao) : Automovel(cor, ano, modelo, cilindrada, preco, ar, direcao) {
        }
};

class Trator : public Automovel {
    public:
        Trator() : Automovel() {
        }
        Trator(string cor, int ano, string modelo, float cilindrada, float preco, float ar, float direcao) : Automovel(cor, ano, modelo, cilindrada, preco, ar, direcao) {
        }
};

int main(void) {
    Carro etios("Prata", 2021, "XL", 1.4, 50000, 2000, 3000);
    Caminhao actros("Vermelho", 2022, "X", 6.0, 500000, 20000, 30000);
    Trator mf3400("Azul", 2022, "MF 3400", 3.0, 75000, 3000, 4000);
    int opcaoTipo;
    char desejaAr;
    char desejaDirecao;
    float precoTotal = 0;
    string modelo = "";
    
    cout << "Monte seu auto móvel:\n";
    cout << "Você quer comprar um carro (1), caminhão (2) ou trator (3)? ";
    cin >> opcaoTipo;
    cout << "\nVocê quer um veículo com ar-condicionada (s/n)? ";
    cin >> desejaAr;
    cout << "\nVocê quer um veículo com direção hidráulica (s/n)? ";
    cin >> desejaDirecao;
    
    switch (opcaoTipo) {
        case 1:
            modelo = etios.modelo;
            precoTotal = etios.preco;
            
            if (desejaAr == 's') {
                precoTotal += etios.ar;
            }
            if (desejaDirecao == 's') {
                precoTotal += etios.direcao;
            }
            break;
        case 2:
            modelo = actros.modelo;
            precoTotal = actros.preco;
            
            if (desejaAr == 's') {
                precoTotal += actros.ar;
            }
            if (desejaDirecao == 's') {
                precoTotal += actros.direcao;
            }
            break;
        case 3:
            modelo = mf3400.modelo;
            precoTotal = mf3400.preco;
            
            if (desejaAr == 's') {
                precoTotal += mf3400.ar;
            }
            if (desejaDirecao == 's') {
                precoTotal += mf3400.direcao;
            }
            break;
        default:
            cout << "Opção inválida!";
            
            return 0;
    }
    
    cout << "O veículo " << modelo << " custa R$" << precoTotal << ".\n";
    
    
	return 0;
}
