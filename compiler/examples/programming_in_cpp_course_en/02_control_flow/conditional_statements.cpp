#include <iostream>
using namespace std;

int main (void) {
        int age;
        char gender;

        cout << "Enter your age: ";
        cin >> age;

        if (age > 18) {                                    // The if statement executes the corresponding
                cout << "You are older than 18." << endl;  // block of code if the condition is true.
        } else if (age < 18) {                             // An else if block executes if its condition
                cout << "You are younger than 18." << endl; // is evaluated as true.
        } else {                                           // An else block executes if none of the
                cout << "You are 18 years old." << endl;   // conditions were evaluated as true.
        }

        cout << "Enter your gender (M/F): ";
        cin >> gender;

        switch (gender) {                              // A switch statement compares a value against
                case 'm':                                  // multiple fixed case values.
                case 'M':                                  // A case matching the given value will have
                        cout << "You are male." << endl;       // its block of code executed, and execution
                        break;                                 // will fall through all subsequent cases
                case 'f':                                  // until a break statement is reached.
                case 'F':
                        cout << "You are female." << endl;
                        break;
                default:                                   // The default case executes if none of the
                        cout << "Gender undefined." << endl;   // cases match the switch value.
        }

        return 0;
}
