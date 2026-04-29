#include <iostream>

int power_recursive(int base, int exponent) {
    if (exponent == 0) {
        return 1;
    }
    return base * power_recursive(base, exponent - 1);
}

int main() {
    std::cout << "pow(3,4)=" << power_recursive(3, 4) << std::endl;
    std::cout << "pow(5,0)=" << power_recursive(5, 0) << std::endl;
    return 0;
}