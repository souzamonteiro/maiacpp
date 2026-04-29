#include <cstdarg>
#include <iostream>

int sum_many(int count, ...) {
    va_list args;
    int total = 0;

    va_start(args, count);
    for (int i = 0; i < count; ++i) {
        total += va_arg(args, int);
    }
    va_end(args);
    return total;
}

int main() {
    std::cout << "sum=" << sum_many(5, 2, 4, 6, 8, 10) << std::endl;
    return 0;
}