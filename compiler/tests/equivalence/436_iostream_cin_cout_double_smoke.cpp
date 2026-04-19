#include <iostream>

double make_zero() {
    return 0.0;
}

int main() {
    double a = make_zero();
    std::cin >> a;
    std::cout << "A=" << a << std::endl;
    return 0;
}
