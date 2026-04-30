#include <iostream>
using namespace std;

int main (void) {
	int idade;
	char genero;
	
	cout << "Informe sua idade: ";
	cin >> idade;
	
	if (idade > 18) {                                 // A declaracao if executa o bloco de codigo correspondente
		cout << "Voce e maior que 18 anos." << endl;  // caso a condicao dada seja avaliada como verdadeira.
	} else if (idade < 18) {                          // Um bloco else if executa caso sua condicao
		cout << "Voce e menor que 18 anos." << endl;  // seja avaliada como verdadeira.
	} else {                                          // Um bloco else executa caso nenhuma das condicoes
		cout << "Voce tem 18 anos." << endl;          // seja avaliada como verdadeira.
	}
	
	cout << "Informe seu genero (M/F): ";
	cin >> genero;
	
	switch (genero) {                              // Uma declaracao switch compara um valor com
		case 'm':                                  // diversos casos de valores fixos.
		case 'M':                                  // Um caso que corresponda ao valor informado
			cout << "Voce e homem." << endl;       // no switch tera seu bloco de codigo executado
			break;                                 // e a execucao continuara por todos os demais
		case 'f':                                  // casos ate encontrar um comando break.
		case 'F':
			cout << "Voce e mulher." << endl;
			break;
		default:                                   // O caso default sera executado se nenhum dos
			cout << "Genero indefinido." << endl;  // casos corresponder ao valor da declacao switch.
	}
	
	return 0;
}