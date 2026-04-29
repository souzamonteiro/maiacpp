#include <iostream>

typedef int (*transform_fn)(int);

int increment(int value) {
    return value + 1;
}

int square(int value) {
    return value * value;
}

int halve_floor(int value) {
    return value / 2;
}

int apply_pipeline(int seed, transform_fn first, transform_fn second, transform_fn third) {
    return third(second(first(seed)));
}

int main() {
    std::cout << "pipeline=" << apply_pipeline(5, increment, square, halve_floor) << std::endl;
    return 0;
}