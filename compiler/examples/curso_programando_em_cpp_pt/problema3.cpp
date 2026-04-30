#include <iostream>
using namespace std;

int main(void) {
	int i;
	int j;
	char palavra[255];
	int ePalindromo;
	
	cout << "Digite um palavra: ";
	cin >> palavra;
	cout << endl;
	
	i = 0;
	j = strlen(palavra) - 1;
	
	ePalindromo = 1;
	
	while (i < strlen(palavra)) {
		if (palavra[i] != palavra[j]) {
			ePalindromo = 0;
			break;
		}
		if (i == j) {
			break;
		}
		i++;
		j--;
	}
	
	if (ePalindromo) {
		cout << "A palavra digitada e um palindromo!";
	} else {
		cout << "A palavra digitada nao e um palindromo!";
	}
	return 0;
}