#include<iostream>
using namespace std;

float FahrenheitToCelsius(float t) {
	return (t - 32.0) * 5.0 / 9.0;
}
int FahrenheitToCelsius(int t) {
	return (t - 32.0) * 5.0 / 9.0;
}

int main(void) {
	int t1;
	float t2;
	
	cout << "Entre a temperatura em Fahrenheit como um numero inteiro: ";
	cin >> t1;
	cout << "A temperatura em Ceusius e " << FahrenheitToCelsius(t1) << ".\n";
	
	cout << "Entre a temperatura em Fahrenheit como um numero real: ";
	cin >> t2;
	cout << "A temperatura em Ceusius e " << FahrenheitToCelsius(t2) << ".\n";
	
	return 0;
}