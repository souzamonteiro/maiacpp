#include <iostream>
using namespace std;

int main(void) {
	int i;
	char nome[255];
	int contador;
	
	cout << "Digite seu nome : ";
	cin >> nome;
	cout << endl;
	
	contador = 0;
	
	for (i = 0; i < strlen(nome); i++) {
		if ((nome[i] == 'a') || (nome[i] == 'e') || (nome[i] == 'i') || (nome[i] == 'o') || (nome[i] == 'u')) {
			contador++;
		}
	}
	
	cout << "Seu nome contem " << contador << " vogais.\n";
	
	return 0;
}