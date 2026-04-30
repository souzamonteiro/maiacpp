#include <iostream>
using namespace std;

int main (void) {
	int a, b;
	char resposta;
	int continua = 1;
	
	while (continua) {
    	cout << "Informe o valor da primeira variável: ";
    	cin >> a;
    	cout << "Informe o valor da segunda variável: ";
    	cin >> b;
    	
    	if (a < b) {
    	    cout << "O valor da primeira variável é menor do que o da segunda!\n";
    	} else if (a > b) {
    	    cout << "O valor da primeira variável é maior do que o da segunda!\n";
    	} else {
    	    cout << "Os valores das variável são iguais!\n";
    	}
    
        cout << "Deseja cotinuar (s/n): ";
    	cin >> resposta;
    	
    	switch (resposta) {
    	    case 's':
    	    case 'S':
    	        continua = 1;
    	        break;
    	    case 'n':
    	    case 'N':
    	        continua = 0;
    	        break;
    	    default:
    	        cout << "Opção inválida!\n";
    	}
	}
	
	cout << "Programa finalizado!\n";
	
	return 0;
}