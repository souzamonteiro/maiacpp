#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
using namespace std;

int numero_vogais(char nome[]) {
    int i;
    int na = 0;
    int ne = 0;
    int ni = 0;
    int no = 0;
    int nu = 0;
    
    for (i = 0; i < strlen(nome); i++) {
        switch (tolower(nome[i])) {
            case 'a':
                if (na == 0) {
                    na++;
                }
                break;
            case 'e':
                if (ne == 0) {
                    ne++;
                }
                break;
            case 'i':
                if (ni == 0) {
                    ni++;
                }
                break;
            case 'o':
                if (no == 0) {
                    no++;
                }
                break;
            case 'u':
                if (nu == 0) {
                    nu++;
                }
                break;
            default:
                break;
        }
    }
    
    return na + ne + ni + no + nu;
}

int main(void) {
    char nome[50];
    
    cout << "Digite seu nome: ";
    cin >> nome;
    
    cout << "Seu nome tem " << numero_vogais(nome) << " vogais.\n";
    
	return 0;
}