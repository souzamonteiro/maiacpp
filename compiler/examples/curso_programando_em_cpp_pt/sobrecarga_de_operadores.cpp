#include<iostream>
using namespace std;

class Vetor {
	public:
		int x;
		int y;
	Vetor (void) {
	}
	Vetor (int a, int b) {
		x = a;
		y = b;
	}
	Vetor operator + (Vetor param) {
		Vetor temp;
		temp.x = x + param.x;
		temp.y = y + param.y;
		return temp;
	}
};

int main(void) {
	Vetor a(3, 1);
	Vetor b(1, 2);
	Vetor c;
	
	c = a + b;
	
	cout << "c = " << c.x << ", " << c.y << ".\n";
	
	return 0;
}