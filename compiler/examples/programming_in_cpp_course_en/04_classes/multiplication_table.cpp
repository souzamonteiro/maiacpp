#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
using namespace std;

class MultiplicationTable {
    public:
        MultiplicationTable() {
        }
        void createTable(int n) {
            int i;
            for (i = 1; i <= 10; i++) {
                cout << n << " x " << i << " = " << n * i << endl;
            }
        }
};

int main(void) {
    MultiplicationTable multTable;
    int n;

    cout << "Which multiplication table would you like to display? ";
    cin >> n;
    cout << endl;

    multTable.createTable(n);

        return 0;
}
