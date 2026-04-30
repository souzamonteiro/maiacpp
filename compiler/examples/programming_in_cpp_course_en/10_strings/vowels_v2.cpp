#include <iostream>
using namespace std;

int main(void) {
        int i;
        char name[255];
        int count;

        cout << "Enter your name: ";
        cin >> name;
        cout << endl;

        count = 0;

        for (i = 0; i < strlen(name); i++) {
                if ((name[i] == 'a') || (name[i] == 'e') || (name[i] == 'i') || (name[i] == 'o') || (name[i] == 'u')) {
                        count++;
                }
        }

        cout << "Your name contains " << count << " vowels.\n";

        return 0;
}
