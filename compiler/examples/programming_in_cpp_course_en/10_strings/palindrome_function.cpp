#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
using namespace std;

int is_palindrome(char word[]) {
    int i, j;
    int result = 1;
    
    j = strlen(word) - 1;
    for (i = 0; i < strlen(word); i++) {
        if (word[i] != word[j]) {
            result = 0;
        }
        j--;
    }
    
    return result;
}

int main (void) {
    char word[50];
    
    cout << "Enter a word: ";
    cin >> word;
    
    if (is_palindrome(word)) {
        cout << "The word is a palindrome!";
    } else {
        cout << "The word is not a palindrome!";
    }
    
        return 0;
}
