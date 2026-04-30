#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
using namespace std;

int numero_vogais_com_repeticao(char *nome) {
    char *p;
    int n = 0;
    
    p = nome;
    while (*p) {
        switch (tolower(*p)) {
            case 'a':
            case 'e':
            case 'i':
            case 'o':
            case 'u':
                n++;
                break;
            default:
                break;
        }
        p++;
    }
    
    return n;
}

int numero_vogais_sem_repeticao(char *nome) {
    char *p;
    int na = 0;
    int ne = 0;
    int ni = 0;
    int no = 0;
    int nu = 0;
    
    p = nome;
    while (*p) {
        switch (tolower(*p)) {
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
        p++;
    }
    
    return na + ne + ni + no + nu;
}

int main(void) {
    char nome[50];
    
    cout << "Digite seu nome: ";
    cin >> nome;
    
    cout << "Seu nome tem " << numero_vogais_com_repeticao(nome) << " vogais com repetição.\n";
    cout << "Seu nome tem " << numero_vogais_sem_repeticao(nome) << " vogais sem repetição.\n";
    
	return 0;
}