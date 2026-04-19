#include <iostream>

int main() {
    int a = 0;
    int b = 0;
    int sum = 0;

    std::cin >> a >> b;
    sum += a;
    sum += b;

    std::cout << "SUM=" << sum << std::endl;
    return sum == 15 ? 0 : 1;
}
