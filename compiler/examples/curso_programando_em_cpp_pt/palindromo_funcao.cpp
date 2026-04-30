#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
using namespace std;

int e_palindromo(char palavra[]) {
    int i, j;
    int e_pali = 1;
    
    j = strlen(palavra) - 1;
    for (i = 0; i < strlen(palavra); i++) {
        if (palavra[i] != palavra[j]) {
            e_pali = 0;
        }
        j--;
    }
    
    return e_pali;
}

int main (void) {
    char palavra[50];
    
    cout << "Digite uma palavra: ";
    cin >> palavra;
    
    if (e_palindromo(palavra)) {
        cout << "A palavra é um palindromo!";
    } else {
        cout << "A palavra não é um palindromo!";
    }
    
	return 0;
}