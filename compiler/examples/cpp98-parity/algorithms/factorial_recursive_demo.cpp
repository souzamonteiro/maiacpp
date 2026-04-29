#include <iostream>

int factorial_recursive(int n) {
    if (n <= 1) {
        return 1;
    }
    return n * factorial_recursive(n - 1);
}

int main() {
    std::cout << "fact5=" << factorial_recursive(5) << std::endl;
    std::cout << "fact7=" << factorial_recursive(7) << std::endl;
    return 0;
}