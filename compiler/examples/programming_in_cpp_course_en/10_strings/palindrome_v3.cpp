#include <iostream>
using namespace std;

int main(void) {
        int i;
        int j;
        char word[255];
        int is_palindrome;

        cout << "Enter a word: ";
        cin >> word;
        cout << endl;

        i = 0;
        j = strlen(word) - 1;

        is_palindrome = 1;

        while (i < strlen(word)) {
                if (word[i] != word[j]) {
                        is_palindrome = 0;
                        break;
                }
                if (i == j) {
                        break;
                }
                i++;
                j--;
        }

        if (is_palindrome) {
                cout << "The word is a palindrome!";
        } else {
                cout << "The word is not a palindrome!";
        }
        return 0;
}
