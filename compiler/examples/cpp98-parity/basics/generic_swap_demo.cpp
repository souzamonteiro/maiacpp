#include <iostream>

template <typename T>
void swap_values(T &left, T &right) {
    T temp = left;
    left = right;
    right = temp;
}

int main() {
    int left = 7;
    int right = 21;
    char first = 'A';
    char second = 'Z';

    swap_values(left, right);
    swap_values(first, second);

    std::cout << "ints=" << left << ',' << right << std::endl;
    std::cout << "chars=" << first << ',' << second << std::endl;
    return 0;
}