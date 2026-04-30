#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
using namespace std;

int numero_vogais(char nome[]) {
    int i;
    int n = 0;
    
    for (i = 0; i < strlen(nome); i++) {
        if (tolower(nome[i]) == 'a' || tolower(nome[i]) == 'e' || tolower(nome[i]) == 'i' || tolower(nome[i]) == 'o' || tolower(nome[i]) == 'u') {
            n++;
        }
    }
    
    return n;
}

int main(void) {
    char nome[50];
    
    cout << "Digite seu nome: ";
    cin >> nome;
    
    cout << "Seu nome tem " << numero_vogais(nome) << " vogais.\n";
    
	return 0;
}