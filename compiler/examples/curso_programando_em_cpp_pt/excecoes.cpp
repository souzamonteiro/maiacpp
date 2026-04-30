#include<iostream>
using namespace std;

int main(void) {
    try {
    	//throw 20;
    	throw string("Oops!");
	} catch (int e) {
		cout << "Ocorreu um erro: " << e << ".\n";
	} catch (string e) {
		cout << "Ocorreu um erro: " << e << ".\n";
	}
	
	return 0;
}