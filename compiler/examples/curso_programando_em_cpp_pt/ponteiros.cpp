#include <iostream>
#include <string.h>
using namespace std;

int tamanho(char txt[]) {
    int n;

    n = 0;
    while (txt[n]) {
        n++;
    }

    return n;
}

void maiusculas(char *origem, char *destino) {
    char *p;
    char *q;

    p = origem; // p = nome
    q = destino;  // q = nomeMaiusculas

    // origem = "ana"
    // destino = ""
    // *p = origem -> *p = 'a'
    // *q = destino -> *q = ''
    // *q = toupper(*p) -> *q = 'A'
    // p++ -> *p = 'n'
    // q++ -> *q = ''
    // *q = toupper(*p) -> *q = 'N'
    // p++ -> *p = 'a'
    // q++ -> *q = ''
    // *q = toupper(*p) -> *q = 'A'
    // p++ -> *p = '\0'
    // q++ -> *q = ''
    // *q = '\0'
    while (*p) {
        *q = toupper(*p);
        p++;
        q++;
    }

    *q = '\0';
}

int main(void) {
    char nome[20];
    char nomeMaiusculas[20];
    char *p;

    int i;

    cout << "Digite seu nome: ";
    cin >> nome;

    cout << "Olá " << nome << "!\n";
    
    cout << ">>";
    for (i = 0; i < 20; i++) {
        cout << nome[i];
    }
    cout << "<<\n";
    
    cout << ">>";
    i = 0;
    while (nome[i] != '\0') {
        cout << nome[i];
        i++;
    }
    cout << "<<\n";

    cout << ">>";
    i = 0;
    while (nome[i]) {
        cout << nome[i];
        i++;
    }
    cout << "<<\n";
    
    cout << "Seu nome tem " << tamanho(nome) << " caracteres.\n";
    cout << "Seu nome tem " << strlen(nome) << " caracteres.\n";

    p = nome;
    cout << ">>";
    while (*p) {
        cout << *p;
        p++;
    }
    cout << "<<\n";

    maiusculas(nome, nomeMaiusculas);

    cout << "Seu nome em maiúculas é " << nomeMaiusculas << ".\n";

    return 0;
}
