#include <iostream>

typedef int (*step_fn)(int);

int add_two(int value) {
    return value + 2;
}

int times_three(int value) {
    return value * 3;
}

int minus_four(int value) {
    return value - 4;
}

int main() {
    step_fn steps[3];
    int value = 1;

    steps[0] = add_two;
    steps[1] = times_three;
    steps[2] = minus_four;

    for (int i = 0; i < 3; ++i) {
        value = steps[i](value);
        std::cout << "step" << i << '=' << value << std::endl;
    }

    return 0;
}