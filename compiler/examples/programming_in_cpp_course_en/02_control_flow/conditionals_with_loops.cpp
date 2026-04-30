#include <iostream>
using namespace std;

int main (void) {
        int a, b;
        char answer;
        int continue_loop = 1;

        while (continue_loop) {
        cout << "Enter the value of the first variable: ";
        cin >> a;
        cout << "Enter the value of the second variable: ";
        cin >> b;
    
        if (a < b) {
            cout << "The first variable is less than the second!\n";
        } else if (a > b) {
            cout << "The first variable is greater than the second!\n";
        } else {
            cout << "Both variables are equal!\n";
        }
    
        cout << "Continue (y/n): ";
        cin >> answer;
    
        switch (answer) {
            case 'y':
            case 'Y':
                continue_loop = 1;
                break;
            case 'n':
            case 'N':
                continue_loop = 0;
                break;
            default:
                cout << "Invalid option!\n";
        }
        }

        cout << "Program finished!\n";

        return 0;
}
