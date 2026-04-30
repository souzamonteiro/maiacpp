#include <iostream>
using namespace std;

void to_uppercase(char *source, char *dest) {   // We can pass arrays to functions
        char *p;                                     // using the *name or name[] type.
        char *q;

        p = source;
        q = dest;

        while (*p) {
                *q = toupper(*p); 
                p++;
                q++;
        }

        *q = '\0';
}

int main(void) {
        char text[255];            // Character array.
        char upper_case[255];
        char *p;
        int i;

        cout << "Write a word: ";

        cin >> text;

        text[0] = '@';                                // Arrays can have their positions accessed using
                                                      // an integer variable or value as an index.
        for (i = 0; i < strlen(text); i++) {
                cout << text[i] << endl;
        }

        p = text;
        printf("Address pointed to by p: %ld\n", p);  // The printf function allows formatted text output
                                                      // using format masks that always start with the % character.
        while (*p) {                                  // Every character array has the value 0 as its
                cout << *p;                               // final character, treated as FALSE in C++.
                p++;
        }

        cout << endl;

        to_uppercase(text, upper_case);

        cout << "Text in uppercase: " << upper_case << endl;

        return 0;
}
