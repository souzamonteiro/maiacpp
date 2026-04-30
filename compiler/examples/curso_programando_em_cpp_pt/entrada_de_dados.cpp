#include <iostream>
using namespace std;

int main (void) {
	int x;
	float y;
	char nome[255];                                         // Uma variavel char[n] contem um texto de tamanho ate n.
	
	cout << "Informe sua idade: ";                          // Escrevemos dados na tela utilizando o objeto cout.
	cin >> x;                                               // Obtemos dados digitados utilizando o objeto cin.
	cout << "Voce tem " << x << " anos de idade." << endl;  // Concatenamos dados para escrever na tela utilizando o operador <<.
	
	cout << "Informe sua altura em metros: ";
	cin >> y;
	cout << "Voce tem " << y << " metros de altura." << endl;

	cout << "Informe seu nome: ";
	cin >> nome;
	cout << "Ola " << nome << "!";

	return 0;
}