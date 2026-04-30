#include <iostream>
using namespace std;

void maiusculo(char *origem, char *destino) {    // Podemos passar vetores para funcoes
	char *p;                                     // usanda tipo *nome ou tipo nome[].
	char *q;
	
	p = origem;
	q = destino;
	
	while (*p) {
		*q = toupper(*p); 
		p++;
		q++;
	}
	
	*q = '\0';
}

int main(void) {
	char texto[255];                              // Vetor de caracteres.
	char caixaAlta[255];
	char *p;
	int i;
	
	cout << "Escreva uma palavra: ";
	
	cin >> texto;
	
	texto[0] = '@';                               // Vetores podem ter suas posicoes acessadas utilizando
	                                              // Uma variavel ou valor inteiro como indice ou posicao.
	for (i = 0; i < strlen(texto); i++) {
		cout << texto[i] << endl;
	}
	
	p = texto;
	printf("Endereco apontado por p: %ld\n", p);  // A funcao printf permite exibir texto formatado na tela
	                                              // usando mascaras que sempre comecam com o caractere %.
	while (*p) {                                  // Todo vetor de caracteres possui como caractere final
		cout << *p;                               // o valor 0, tratado como FALSO peli C++.
		p++;
	}
	
	cout << endl;
	
	maiusculo(texto, caixaAlta);
	
	cout << "Texto em maiusculas: " << caixaAlta << endl;
	
	return 0;
}