#include <iostream>
using namespace std;

int main (void) {
	int i;
	char c;
	
	i = 0;
	while (i < 10) {               // Uma declaracao while executa um bloco de codigo
		cout << i << endl;         // enquanto uma condicao for avaliada como verdadera.
		i++;                       // A variavel i e chamada de iteractor.
	}
	
	do {                           // Uma declaracao do executa um bloco de codigo
		cout << i << endl;         // enquanto uma condicao for avaliada como verdadera
		i++;                       // mas realiza a comparacao depois da excucao do
	} while (i < 10);              // bloco de codigo.
	
	for (i = 0; i < 10; i++) {     // Uma declaracao for combina a inicializacao,
		cout << i << endl;         // a comparacao e o incremento da variavel de
	}                              // controle em um unico comando.
	
	for (c = 'a'; c < 'z'; c++) {  // Podemos utilizar variaveis do tipo char
		cout << c << endl;         // como se fossem numericas. Porem os resultados
	}                              // podem ser diferentes do esperado ja que o compilador
	                               // sempre considera ela como caractere.
	return 0;
}