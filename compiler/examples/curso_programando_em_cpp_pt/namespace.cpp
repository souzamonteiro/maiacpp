#include<iostream>
using namespace std;

namespace numeric {
	float golden = 1.1680;
	float pi = 3.1416;
}

int main(void) {
	cout << "O numero dourado e " << numeric::golden << ".\n";
	cout << "O numero pi e " << numeric::pi << ".\n";
	
	return 0;
}