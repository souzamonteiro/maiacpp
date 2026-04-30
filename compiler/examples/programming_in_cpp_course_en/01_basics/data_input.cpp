#include <iostream>
using namespace std;

int main (void) {
        int age;
        float height;
        char name[255];                                         // A char[n] variable holds a string of up to n characters.

        cout << "Enter your age: ";                             // We write data to the screen using the cout object.
        cin >> age;                                             // We read typed data using the cin object.
        cout << "You are " << age << " years old." << endl;    // We concatenate data to print using the << operator.

        cout << "Enter your height in meters: ";
        cin >> height;
        cout << "You are " << height << " meters tall." << endl;

        cout << "Enter your name: ";
        cin >> name;
        cout << "Hello " << name << "!";

        return 0;
}
