#include<iostream>
using namespace std;

template <class T>
T tmax(T a, T b) {
	return a > b ? a : b;
}

int main(void) {
	cout << "O maior valor entre " << 1 << " e " << 2 << " e " << tmax<int>(1, 2) << ".\n";
	cout << "O maior valor entre " << 3.2 << " e " << 3.7 << " e " << tmax<float>(3.2, 3.7) << ".\n";
	
	return 0;
}