#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
using namespace std;

int count_vowels(char name[]) {
    int i;
    int n = 0;
    
    for (i = 0; i < strlen(name); i++) {
        if (tolower(name[i]) == 'a' || tolower(name[i]) == 'e' || tolower(name[i]) == 'i' || tolower(name[i]) == 'o' || tolower(name[i]) == 'u') {
            n++;
        }
    }
    
    return n;
}

int main(void) {
    char name[50];
    
    cout << "Enter your name: ";
    cin >> name;
    
    cout << "Your name has " << count_vowels(name) << " vowels.\n";
    
        return 0;
}
