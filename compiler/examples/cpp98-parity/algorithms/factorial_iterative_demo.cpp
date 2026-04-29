#include <iostream>

int factorial_iterative(int n) {
    int result = 1;
    for (int i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

int main() {
    std::cout << "fact6=" << factorial_iterative(6) << std::endl;
    std::cout << "fact3=" << factorial_iterative(3) << std::endl;
    return 0;
}