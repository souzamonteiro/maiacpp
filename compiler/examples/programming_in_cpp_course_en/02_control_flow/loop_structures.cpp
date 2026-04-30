#include <iostream>
using namespace std;

int main (void) {
        int i;
        char c;

        i = 0;
        while (i < 10) {               // A while statement executes a block of code
                cout << i << endl;         // while a condition is evaluated as true.
                i++;                       // The variable i is called an iterator.
        }

        do {                           // A do statement executes a block of code
                cout << i << endl;         // while a condition is evaluated as true,
                i++;                       // but checks the condition after the block
        } while (i < 10);              // of code has been executed.

        for (i = 0; i < 10; i++) {     // A for statement combines initialization,
                cout << i << endl;         // condition check and iterator increment
        }                              // in a single command.

        for (c = 'a'; c < 'z'; c++) {  // We can use char variables
                cout << c << endl;         // as if they were numeric, though results
        }                              // may differ from expectations since the
                                       // compiler always treats them as characters.
        return 0;
}
