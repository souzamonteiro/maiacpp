#include <iostream>
using namespace std;

void exibirMensagem(char texto[]) {
	cout << texto << endl;
}

int quadrado(int x) {
	return x * x;
}

int main(void) {
	char msg[] = "Alo Mundo!";
	
	exibirMensagem(msg);
	
	cout << "O quadrado de 5 e " << quadrado(5) << ".\n";
	
	return 0;
}