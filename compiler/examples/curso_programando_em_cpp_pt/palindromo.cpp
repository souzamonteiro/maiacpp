#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
using namespace std;

int main (void) {
    char palavra[50];
    int i, j;
    int e_palindromo = 1;
    
    cout << "Digite uma palavra: ";
    cin >> palavra;
    
    // palavra = 'ovo'
    // i = 0
    // j = 2
    // palavra[0] = 'o'
    // palavra[2] = 'o'
    // palavra[0] != palavra[2] = false (0)
    //
    // palavra = 'bola'
    // i = 0
    // j = 3
    // palavra[0] = 'b'
    // palavra[3] = 'a'
    // palavra[0] != palavra[3] = true (1)
    // e_palindromo = 0
    j = strlen(palavra) - 1;
    for (i = 0; i < strlen(palavra); i++) { // Crescente.
        if (palavra[i] != palavra[j]) {
            e_palindromo = 0;
        }
        j--; // Decrescente.
    }
    
    if (e_palindromo) {
        cout << "A palavra é um palindromo!";
    } else {
        cout << "A palavra não é um palindromo!";
    }
    
	return 0;
}