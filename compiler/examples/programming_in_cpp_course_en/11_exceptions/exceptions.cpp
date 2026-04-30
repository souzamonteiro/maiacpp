#include<iostream>
using namespace std;

int main(void) {
    try {
        //throw 20;
        throw string("Oops!");
        } catch (int e) {
                cout << "An error occurred: " << e << ".\n";
        } catch (string e) {
                cout << "An error occurred: " << e << ".\n";
        }

        return 0;
}
