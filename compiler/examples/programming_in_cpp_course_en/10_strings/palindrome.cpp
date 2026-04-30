#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
using namespace std;

int main (void) {
    char word[50];
    int i, j;
    int is_palindrome = 1;
    
    cout << "Enter a word: ";
    cin >> word;
    
    // word = 'eye'
    // i = 0
    // j = 2
    // word[0] = 'e'
    // word[2] = 'e'
    // word[0] != word[2] = false (0)
    //
    // word = 'ball'
    // i = 0
    // j = 3
    // word[0] = 'b'
    // word[3] = 'l'
    // word[0] != word[3] = true (1)
    // is_palindrome = 0
    j = strlen(word) - 1;
    for (i = 0; i < strlen(word); i++) { // Ascending.
        if (word[i] != word[j]) {
            is_palindrome = 0;
        }
        j--; // Descending.
    }
    
    if (is_palindrome) {
        cout << "The word is a palindrome!";
    } else {
        cout << "The word is not a palindrome!";
    }
    
        return 0;
}
